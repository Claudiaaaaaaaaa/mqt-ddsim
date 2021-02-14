#include "CircuitSimulator.hpp"

std::map<std::string, unsigned int> CircuitSimulator::Simulate(const unsigned int shots) {
    bool has_nonmeasurement_nonunitary = false;
    bool has_measurements = false;
    bool measurements_last = true;
    std::map<unsigned int, unsigned int> measurement_map;

    for (auto &op : *qc) {
        if (op->isClassicControlledOperation() || (op->isNonUnitaryOperation() && op->getType() != qc::Measure)) {
            has_nonmeasurement_nonunitary = true;
        }
        if (op->getType() == qc::Measure) {
            has_measurements = true;

            auto& quantum = op->getControls();
            auto& classic = op->getTargets();

            if (quantum.size() != classic.size()) {
                throw std::runtime_error("Measurement: Sizes of quantum and classic register mismatch.");
            }

            for (unsigned int i = 0; i < quantum.size(); ++i) {
                measurement_map[quantum[i].qubit] = classic[i];
            }

        }
        if (has_measurements && op->isUnitary()) {
            measurements_last = false;
        }
    }

    // easiest case: all gates are unitary --> simulate once and sample away on all qubits
    if (!has_nonmeasurement_nonunitary && !has_measurements) {
        single_shot(false);
        return MeasureAllNonCollapsing(shots);
    }

    // single shot is enough, but the sampling should only return actually measured qubits
    if (!has_nonmeasurement_nonunitary && measurements_last) {
        single_shot(true);
        std::map<std::string, unsigned int> m_counter;
        const auto n_qubits = qc->getNqubits();
        const auto n_cbits = qc->getNcbits();

        // MeasureAllNonCollapsing returns a map from measurement over all qubits to the number of occurrences
        for( const auto& item : MeasureAllNonCollapsing(shots)) {
            std::string result_string(qc->getNcbits(), '0');

            for (auto const& m : measurement_map) {
                // m.first is the qubit, m.second the classical bit
                result_string[n_cbits - m.second - 1] = item.first[n_qubits - m.first - 1];
            }

            m_counter[result_string] += item.second;
        }

        return m_counter;
    }

    // there are nonunitaries (or intermediate measurement_map) and we have to actually do multiple single_shots :(
    std::map<std::string, unsigned int> m_counter;

    for (unsigned int i = 0; i < shots; i++) {
        const auto result = single_shot(false);
        const auto n_cbits = qc->getNcbits();

        std::string result_string(qc->getNcbits(), '0');

        // result is a map from the cbit index to the Boolean value
        for (const auto& r : result) {
            result_string[n_cbits - r.first - 1] = r.second ? '1' : '0';
        }
        m_counter[result_string]++;
    }

    return m_counter;
}

std::map<int, bool> CircuitSimulator::single_shot(const bool ignore_nonunitaries) {
    single_shots++;
    const unsigned short n_qubits = qc->getNqubits();

    std::array<short, qc::MAX_QUBITS> line{};
    line.fill(qc::LINE_DEFAULT);

    root_edge = dd->makeZeroState(n_qubits);
    dd->incRef(root_edge);

    unsigned long op_num = 0;
    std::map<int, bool> classic_values;

    const int approx_mod = std::ceil(static_cast<double>(qc->getNops()) / (step_number + 1));

    for (auto &op : *qc) {
        if (op->isNonUnitaryOperation()) {
            if (ignore_nonunitaries) {
                continue;
            }
            if (auto *nu_op = dynamic_cast<qc::NonUnitaryOperation *>(op.get())) {
                if (op->getType() == qc::Measure) {
                    auto quantum = nu_op->getControls();
                    auto classic = nu_op->getTargets();

                    assert(quantum.size() == classic.size()); // this should not happen do to check in Simulate

                    for (unsigned int i = 0; i < quantum.size(); ++i) {
                        auto result = MeasureOneCollapsing(quantum[i].qubit);
                        assert(result == '0' || result == '1');
                        classic_values[classic[i]] = (result == '1');
                    }
                } else if (op->getType() == qc::Barrier) {
                    continue;
                } else {
                    throw std::runtime_error("Unsupported non-unitary functionality.");
                }
            } else {
                throw std::runtime_error("Dynamic cast to NonUnitaryOperation failed.");
            }
            dd->garbageCollect();
        } else {
            if (op->isClassicControlledOperation()) {
                if (auto *cc_op = dynamic_cast<qc::ClassicControlledOperation *>(op.get())) {
                    const auto start_index = static_cast<unsigned short>(cc_op->getParameter().at(0));
                    const auto length = static_cast<unsigned short>(cc_op->getParameter().at(1));
                    const unsigned int expected_value = cc_op->getExpectedValue();
                    unsigned int actual_value = 0;
                    for (unsigned int i = 0; i < length; i++) {
                        actual_value |= (classic_values[start_index + i] ? 1u : 0u) << i;
                    }

                    //std::clog << "expected " << expected_value << " and actual value was " << actual_value << "\n";

                    if (actual_value != expected_value) {
                        continue;
                    }
                } else {
                    throw std::runtime_error(" Dynamic cast to ClassicControlledOperation failed.");
                }
            }
            /*std::clog << "[INFO] op " << op_num << " is " << op->getName() << " on " << op->getTargets().at(0)
                      << " #controls=" << op->getControls().size()
                      << " statesize=" << dd->size(root_edge) << "\n";//*/

            auto dd_op = op->getDD(dd, line);
            auto tmp = dd->multiply(dd_op, root_edge);
            dd->incRef(tmp);
            dd->decRef(root_edge);
            root_edge = tmp;

            if (step_fidelity < 1.0) {
                if ((op_num + 1) % approx_mod == 0 && approximation_runs < step_number) {
                    [[maybe_unused]] const unsigned int size_before = dd->size(root_edge);
                    const double ap_fid = ApproximateByFidelity(step_fidelity, true);
                    approximation_runs++;
                    final_fidelity *= ap_fid;
                    /*std::clog << "[INFO] Appromation run finished. "
                              << "op_num=" << op_num
                              << "; previous size=" << size_before
                              << "; attained fidelity=" << ap_fid
                              << "; global fidelity=" << final_fidelity
                              << "; #runs=" << approximation_runs
                              << "\n";//*/
                }
            }
            dd->garbageCollect();
        }
        op_num++;
    }
    return classic_values;
}