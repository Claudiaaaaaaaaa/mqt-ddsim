#include "DDMinimizer.hpp"

#include "CircuitOptimizer.hpp"
#include "CircuitSimulator.hpp"
#include "QuantumComputation.hpp"
#include "operations/OpType.hpp"

#include <cstddef>
#include <iostream>
#include <chrono>
#include <fstream>
#include <memory>
#include <algorithm>
#include <tuple>
#include <map>
#include <cctype>
#include <random>
#include <utility>
#include <vector>
#include <sstream>
#include <string>
#include <set>


using namespace qc;
using namespace std;



namespace qc {
/***
 * Public Methods
 ***/

void optimizeInputPermutation(qc::QuantumComputation qc){
    /*TODO
    qc::Permutation perm = DDMinimizer::createControlBasedPermutation(qc);
    qc.initialLayout = perm;
    qc::CircuitOptimizer::elidePermutations(qc); 
    */
}

void DDMinimizer::parseOptions(bool gateBased,  bool controlBased, bool allPermutations, std::string file, int qubits){
    std::map<std::string, std::pair<int, int>> fileNames = {
    //{"ae_indep_qiskit_", {2, 7}},
    {"dj_indep_qiskit_", {2, 7}},
    {"ghz_indep_qiskit_", {2, 10}},
    {"graphstate_indep_qiskit_", {3, 7}},
    {"grover-noancilla_indep_qiskit_", {2, 7}},
    {"grover-v-chain_indep_qiskit_", {2, 7}},
    {"portfolioqaoa_indep_qiskit_", {3, 7}},
    {"portfoliovqe_indep_qiskit_", {3, 7}},
    {"qaoa_indep_qiskit_", {3, 10}},
    {"qftentangled_indep_qiskit_", {2, 7}},
    {"qft_indep_qiskit_", {2, 10}},
    {"qnn_indep_qiskit_", {2, 7}},
    {"qpeexact_indep_qiskit_", {2, 10}},
    {"qpeinexact_indep_qiskit_", {2, 7}},
    {"qwalk-noancilla_indep_qiskit_", {3, 7}},
    {"qwalk-v-chain_indep_qiskit_", {3, 7}},
    {"random_indep_qiskit_", {2, 7}},
    {"realamprandom_indep_qiskit_", {2, 7}},
    {"su2random_indep_qiskit_", {2, 7}},
    {"twolocalrandom_indep_qiskit_", {2, 7}},
    {"vqe_indep_qiskit_", {3, 10}},
    {"wstate_indep_qiskit_", {2, 7}}
    };

    if(file.compare(" ") != 0){
        std::cout << "file: " + file + "\n";
        auto it = fileNames.find(file);
            if (it != fileNames.end()) {
                std::pair<int, int> minMaxQubits = it->second;
                 if(qubits != 0){
                    if(qubits < minMaxQubits.first || qubits > minMaxQubits.second)
                         std::cout << std::to_string(qubits) + " not valid for " + file + ". Please check list for available versions again.\n";
                    }
                    else{
                        DDMinimizer::runInputCompariston(qubits, qubits, gateBased, controlBased, allPermutations, file);      
                    }
            }
            else {
                std::cout << file + " not found. Please check list of available circuits again. \n";
                return;
           }  
    }
    else{
        //run all simulations
        for(const auto& entry : fileNames){
                DDMinimizer::runInputCompariston(entry.second.first, entry.second.second, gateBased, controlBased, allPermutations, entry.first); 
        }
    }
}

void DDMinimizer::runInputCompariston(int from, int to, bool gateBased,  bool controlBased, bool allPermutations, std::string file){
    std::string qasmString;
 
    if(gateBased){
        std::string fname_gate = "BenchmarkQuasm/results/" + file+ "gates.txt";
        std::cout << "Start measuring of gate based permutations for " + fname_gate + "\n"; 
        std::cout.flush();
        std::ofstream out_gate(fname_gate);
        for(int k = from; k <= to; k++){
            if(file.compare("random_indep_qiskit_") == 0 && k == 6){
                continue;
            }
            try {
                std::string fn = "BenchmarkQuasm/qasm/" + file + std::to_string(k) + ".qasm";
                qasmString = DDMinimizer::readFileIntoString(fn);
            } catch (const std::exception& e) {
                std::cout << e.what() << "\n";
            }

            out_gate << "Start Simulation of " + file + std::to_string(k) + "__________________________________________________\n";            
            auto qc = QuantumComputation::fromQASM(qasmString);
            auto qcc = QuantumComputation::fromQASM(qasmString);
            out_gate << qc;
            qc::Permutation perm = DDMinimizer::createGateBasedPermutation(out_gate, qc);
            qcc.initialLayout = perm; 
            qc::CircuitOptimizer::elidePermutations(qcc);
            DDMinimizer::runLayoutComparison(out_gate, qc, qcc, perm, file, k);
        } 
        out_gate.close();
        DDMinimizer::removeAnsi(fname_gate);
    }


    if(controlBased){ 
        std::string fname_control = "BenchmarkQuasm/results/" + file+ "controls.txt";
        std::cout << "Start measuring of cotrol based permutations for " + fname_control + "\n"; 
        std::cout.flush();
        std::ofstream out_control(fname_control);
        for(int k = from; k <= to; k++){
             if(file.compare("random_indep_qiskit_") == 0 && k == 6){
                continue;
            }
            try {
                std::string fn = "BenchmarkQuasm/qasm/" + file + std::to_string(k) + ".qasm";
                qasmString = DDMinimizer::readFileIntoString(fn);
                std::cout << "Version: " + std::to_string(k) + ", file: " + fn + "\n";
            } catch (const std::exception& e) {
                std::cout << e.what() << "\n";
            }
            out_control << "Start Simulation of " + file + std::to_string(k) + "__________________________________________________\n";            
            auto qc = QuantumComputation::fromQASM(qasmString);
            auto qcc = QuantumComputation::fromQASM(qasmString);
            out_control << qc;
            qc::Permutation perm = DDMinimizer::createControlBasedPermutation(out_control, qc);
            qcc.initialLayout = perm; 
            qc::CircuitOptimizer::elidePermutations(qcc);
            DDMinimizer::runLayoutComparison(out_control, qc, qcc, perm, file, k);
        }
         out_control.close();
        DDMinimizer::removeAnsi(fname_control);
    } 
    
    if(allPermutations){
        std::string fname_all = "BenchmarkQuasm/results/" + file+ "all.txt";
        std::cout << "Start measuring all permutations for " + fname_all + "\n";
        std::cout.flush();
        std::ofstream out_all(fname_all);
        for(int k = from; k <= to && k <= 6; k++){
            if(file.compare("random_indep_qiskit_") == 0 && k == 6){
                continue;
            }
            try {
                std::string fn = "BenchmarkQuasm/qasm/" + file + std::to_string(k) + ".qasm";
                qasmString = DDMinimizer::readFileIntoString(fn);
            } catch (const std::exception& e) {
                std::cout << e.what() << "\n";
            }
            out_all << "Start Simulation of " + file + std::to_string(k) + "__________________________________________________\n";            
            auto qc = QuantumComputation::fromQASM(qasmString);
            out_all << qc;
            DDMinimizer::runAllComparisons(out_all, qc, file, k);
        }
        out_all.close();
        DDMinimizer::removeAnsi(fname_all);
    } 
}

void DDMinimizer::runLayoutComparison(std::ofstream& out, qc::QuantumComputation& qc, qc::QuantumComputation& qcc, qc::Permutation perm, std::string file,  int qubits){
    out << left << setfill(' ') << setw(30) << "Computed Permutation: " << right << setfill('.') << setw(30) <<DDMinimizer::permToString(perm) << "\n";
    out << left << setfill(' ') << setw(30) << "Original Permutation: " << right << setfill('.') << setw(30) <<DDMinimizer::permToString(qc.initialLayout) << "\n";

    auto qcc_unique_ptr = std::make_unique<qc::QuantumComputation>(std::move(qcc));        
    CircuitSimulator ddsim_qcc(std::move(qcc_unique_ptr), ApproximationInfo(1, 1, ApproximationInfo::FidelityDriven));
    auto qc_unique_ptr = std::make_unique<qc::QuantumComputation>(std::move(qc));        
    CircuitSimulator ddsim_qc(std::move(qc_unique_ptr), ApproximationInfo(1, 1, ApproximationInfo::FidelityDriven));
    

    std::chrono::duration<double> duration_qc;
    std::chrono::duration<double> duration_qcc;
    std::size_t size_max_qc = 0;
    std::size_t size_max_qcc = 0;
    std::size_t size_qc = 0;
    std::size_t size_qcc = 0;  
    
    auto start1_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end1_qc = std::chrono::high_resolution_clock::now();
    
    auto start1_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end1_qcc = std::chrono::high_resolution_clock::now();
    auto start2_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end2_qcc = std::chrono::high_resolution_clock::now();

    auto start2_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end2_qc = std::chrono::high_resolution_clock::now();
    auto start3_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end3_qc = std::chrono::high_resolution_clock::now();
    
    auto start3_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end3_qcc = std::chrono::high_resolution_clock::now();
    auto start4_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end4_qcc = std::chrono::high_resolution_clock::now();

    auto start4_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end4_qc = std::chrono::high_resolution_clock::now();
    auto start5_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end5_qc = std::chrono::high_resolution_clock::now();

    auto start5_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end5_qcc = std::chrono::high_resolution_clock::now();
    auto start6_qcc = std::chrono::high_resolution_clock::now();
    ddsim_qcc.simulate(1); 
    auto end6_qcc = std::chrono::high_resolution_clock::now();
    
    auto start6_qc = std::chrono::high_resolution_clock::now();
    ddsim_qc.simulate(1); 
    auto end6_qc = std::chrono::high_resolution_clock::now();

    duration_qc = (end3_qc - start3_qc + end4_qc - start4_qc + end5_qc - start5_qc + end6_qc - start6_qc) / 4.0;
    size_max_qc = ddsim_qc.getMaxNodeCount(); 
    size_qc = ddsim_qc.getActiveNodeCount();

    duration_qcc = (end3_qcc - start3_qcc + end4_qcc - start4_qcc + end5_qcc - start5_qcc + end6_qcc - start6_qcc) / 4.0;
    size_max_qcc = ddsim_qcc.getMaxNodeCount();
    size_qcc = ddsim_qcc.getActiveNodeCount();

    std::string graphvizString_qc = ddsim_qc.exportDDtoGraphvizString(true, true, true, true, true);
    std::string graphvizString_qcc = ddsim_qcc.exportDDtoGraphvizString(true, true, true, true, true);

    std::string fname_qc = "BenchmarkQuasm/dot_computed/" + file + std::to_string(qubits) + ".dot";
    std::ofstream out_qc(fname_qc);
    out_qc << graphvizString_qc;
    out_qc.close();
    std::string fname_qcc = "BenchmarkQuasm/dot_computed/" + file + std::to_string(qubits) + ".dot";
    std::ofstream out_qcc(fname_qcc); 
    out_qcc << graphvizString_qcc;
    out_qcc.close();

    out << std::left << std::setfill(' ') << std::setw(30) << "Identity Duration: " << std::scientific << std::setprecision(4) << duration_qc.count() << "s" << std::right << std::setfill('.') << std::setw(30) << "\n";
    out << std::left << std::setfill(' ') << std::setw(30) << "Permuted Duration: " << std::scientific << std::setprecision(4) << duration_qcc.count() << "s" << std::right << std::setfill('.') << std::setw(30) << "\n";
    out << std::left << std::setfill(' ') << std::setw(30) << "Identity active_nodes: " << DDMinimizer::formatSize_t(size_qc)  << std::right << std::setfill('.') << std::setw(39) << "\n";
    out << std::left << std::setfill(' ') << std::setw(30) << "Permuted active_nodes: " << DDMinimizer::formatSize_t(size_qcc) << std::right << std::setfill('.') << std::setw(39) << "\n";
    out << std::left << std::setfill(' ') << std::setw(30) << "Identity max_nodes: " << DDMinimizer::formatSize_t(size_max_qc)  << std::right << std::setfill('.') << std::setw(39) << "\n";
    out << std::left << std::setfill(' ') << std::setw(30) << "Permuted max_nodes: " << DDMinimizer::formatSize_t(size_max_qcc) << std::right << std::setfill('.') << std::setw(39) << "\n";
    out << "________________________________________________________________________________________________\n\n";
}


void DDMinimizer::runAllComparisons(std::ofstream& out, qc::QuantumComputation& qc, std::string file,  int qubits){
    std::size_t bits = qc.getNqubits();
    std::size_t permutation_count = DDMinimizer::factorial(bits);
    std::vector<qc::Permutation> permutations = DDMinimizer::createAllPermutations(qc);

    /*prepare all variables for simulation and benchmarking:

    measurements for the size of the decision diagram:
    -min and max, as well as the coresponding index/position of the permutation in the permutatin vector for the active and max nodes

    measurement for the simulation:
    -min and max duration of the simulation , as well as the coresponding index/position of the permutation in the permutatin vector

    maps to collect all measurements for one permutation and sort them later by active nodes and max nodes for analysis
    -perm_by_active_nodes:active noes -> (index, max nodes, time)
    -perm_by_max_nodes: max nodes -> (index, active nodes, time)
    -perm_by_time: time -> (index, active nodes, max nodes)

    map to collect all the GraphViz strings of the decision diagramm of the permutation after simulation (end state)
    -permutationToGraphViz: index in permutation vector -> GraphvizString
    */
    std::size_t min_active_nodes = std::numeric_limits<std::size_t>::max();
    std::size_t max_active_nodes = 0;
    std::size_t min_active_nodes_pos = 0;
    std::size_t max_active_nodes_pos = 0;
    std::size_t current_active_nodes = 0;

    std::size_t min_max_nodes = std::numeric_limits<std::size_t>::max();
    std::size_t max_max_nodes = 0;
    std::size_t min_max_nodes_pos = 0;
    std::size_t max_max_nodes_pos = 0;
    std::size_t current_max_nodes = 0;

    std::chrono::duration<double> min_time(std::numeric_limits<double>::max());
    std::chrono::duration<double> max_time(std::numeric_limits<double>::lowest());
    std::size_t min_time_pos = 0;
    std::size_t max_time_pos = 0;
    std::chrono::duration<double> duration;

    std::map<size_t, std::vector<std::tuple<size_t, size_t, std::chrono::duration<double>>>> perm_by_active_nodes;
    std::map<size_t, std::vector<std::tuple<size_t, size_t, std::chrono::duration<double>>>> perm_by_max_nodes;
    std::map<std::chrono::duration<double>, std::vector<std::tuple<size_t, size_t, size_t>>> perm_by_time;
    std::map<size_t, std::string> permutationToGraphViz;

    /* Simulate the circuit using each permutation
    */
    for (size_t i = 0; i < permutation_count; i++) { 
    //Create copy of the Quantum Computation, set the initial layout as the current permutation, apply the permutation to the qubits,
    //create the simulator and with that the decision diagram
    QuantumComputation qcc = qc;
    qcc.initialLayout = permutations[i]; 
    qc::CircuitOptimizer::elidePermutations(qcc); 
    auto qc_unique_ptr = std::make_unique<qc::QuantumComputation>(std::move(qcc));        
    CircuitSimulator ddsim(std::move(qc_unique_ptr), ApproximationInfo(1, 1, ApproximationInfo::FidelityDriven));

    //Save the time before and after the simulation and run the simulation once
    auto start1_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end1_qc = std::chrono::high_resolution_clock::now();
    auto start2_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end2_qc = std::chrono::high_resolution_clock::now();
    auto start3_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end3_qc = std::chrono::high_resolution_clock::now();
    auto start4_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end4_qc = std::chrono::high_resolution_clock::now();
    auto start5_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end5_qc = std::chrono::high_resolution_clock::now();
    auto start6_qc = std::chrono::high_resolution_clock::now();
    ddsim.simulate(1); 
    auto end6_qc = std::chrono::high_resolution_clock::now();

    //collect measruements as in execution time, active nodes, max nodes
    //set min and max values and save positions
    duration = (end3_qc - start3_qc + end4_qc - start4_qc + end5_qc - start5_qc  + end6_qc - start6_qc) / 4.0;
    if (duration < min_time) {
        min_time = duration;
        min_time_pos = i;
    }
    if (duration > max_time) {
        max_time = duration;
        max_time_pos = i;
    }
    current_active_nodes = ddsim.getActiveNodeCount(); 
    if  (current_active_nodes < min_active_nodes) {
        min_active_nodes = current_active_nodes;
        min_active_nodes_pos = i;
    }
    if  (current_active_nodes > max_active_nodes) {
        max_active_nodes= current_active_nodes;
        max_active_nodes_pos = i;
    }
    current_max_nodes = ddsim.getMaxNodeCount(); 
    if  (current_max_nodes < min_max_nodes) {
        min_max_nodes = current_max_nodes;
        min_max_nodes_pos = i;
    }
    if  (current_max_nodes > max_max_nodes) {
        max_max_nodes= current_max_nodes;
        max_max_nodes_pos = i;
    }

    //save the permutations with the corresponding measurements in the maps
    //save one by active nodes, one by max nodes and one by time
    perm_by_active_nodes[current_active_nodes].emplace_back(i, current_max_nodes, duration);
    perm_by_max_nodes[current_max_nodes].emplace_back(i, current_active_nodes, duration);
    perm_by_time[duration].emplace_back(i, current_active_nodes, current_max_nodes);

    //generate graphviz string and save it to access it later
    std::string graphvizString = ddsim.exportDDtoGraphvizString(true, true, true, true, true);
    permutationToGraphViz[i] = graphvizString;
    }

    //sort the vector in the maps by the not grouped by nodes count and then by time within the group
    for (auto& entry : perm_by_active_nodes) {
    std::sort(entry.second.begin(), entry.second.end(), 
    [](const std::tuple<size_t, size_t, std::chrono::duration<double>>& a, const std::tuple<size_t, size_t, std::chrono::duration<double>>& b) {
        // First compare by max_nodes
        if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
        // If max_nodes are equal, then compare by execution time
        return std::get<2>(a) < std::get<2>(b);
    });
    }
    for (auto& entry : perm_by_max_nodes) {
    std::sort(entry.second.begin(), entry.second.end(), 
    [](const std::tuple<size_t, size_t, std::chrono::duration<double>>& a, const std::tuple<size_t, size_t, std::chrono::duration<double>>& b) {
        // First compare by max_nodes
        if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
        // If max_nodes are equal, then compare by execution time
        return std::get<2>(a) < std::get<2>(b);
    });
    }
    
    //Print statistics of min and max time, active nodes and max nodes with the corresponding permutation
    out << left << setfill(' ') << setw(20) << "Total Permutations: " << permutation_count <<  "\n";
    out << left << setfill(' ') << setw(20) << "Min time: " << std::scientific << std::setprecision(4) << min_time.count() << "s" << right << setfill('.') << setw(30) << " Permutation " << DDMinimizer::formatSize_t(min_time_pos) << ": "<< DDMinimizer::permToString(permutations[min_time_pos]) << "\n";
    out << left << setfill(' ') << setw(20) << "Max time: " << std::scientific << std::setprecision(4) << max_time.count() << "s" << right << setfill('.') << setw(30) << " Permutation " << DDMinimizer::formatSize_t(max_time_pos) << ": "<< DDMinimizer::permToString(permutations[max_time_pos]) << "\n";
    out << left << setfill(' ') << setw(20) << "Min active nodes: "<< DDMinimizer::formatSize_t(min_active_nodes) << " nodes" << right << setfill('.') << setw(33) << " Permutation " << DDMinimizer::formatSize_t(min_active_nodes_pos) << ": "<< DDMinimizer::permToString(permutations[min_active_nodes_pos]) << "\n";
    out << left << setfill(' ') << setw(20) << "Min max nodes: "<< DDMinimizer::formatSize_t(min_max_nodes) << " nodes" << right << setfill('.') << setw(33) << " Permutation " << DDMinimizer::formatSize_t(min_max_nodes_pos) << ": "<< DDMinimizer::permToString(permutations[min_max_nodes_pos]) << "\n";
    out << left << setfill(' ') << setw(20) << "Max active nodes: "<< DDMinimizer::formatSize_t(max_active_nodes) << " nodes" << right << setfill('.') << setw(33) << " Permutation " << DDMinimizer::formatSize_t(max_active_nodes_pos) << ": "<< DDMinimizer::permToString(permutations[max_active_nodes_pos]) << "\n";
    out << left << setfill(' ') << setw(20) << "Max max nodes: "<< DDMinimizer::formatSize_t(max_active_nodes_pos) << " nodes" << right << setfill('.') << setw(33) << " Permutation " << DDMinimizer::formatSize_t(max_max_nodes_pos) << ": "<< DDMinimizer::permToString(permutations[max_max_nodes_pos]) << "\n";
    out << "________________________________________________________________________________________________\n\n";
    
    
    //write the graphvizStrings of the permutation with minimal and maximal active nodes into files
    std::string graphvizString = permutationToGraphViz[min_active_nodes_pos];
    std::string fnameMin = "BenchmarkQuasm/dot/" + file + "min_active" + std::to_string(qubits) + ".dot";
    std::ofstream out1(fnameMin);
    out1 << graphvizString;
    out1.close();

    std::string graphvizString2 = permutationToGraphViz[max_active_nodes_pos];
    std::string fnameMax = "BenchmarkQuasm/dot/" + file + "max_active" + std::to_string(qubits) + ".dot";
    std::ofstream out2(fnameMax);
    out2 << graphvizString2;
    out2.close();

    std::string graphvizString3 = permutationToGraphViz[min_max_nodes_pos];
    std::string fnameMinMax = "BenchmarkQuasm/dot/" + file + "min_max" + std::to_string(qubits) + ".dot";
    std::ofstream out3(fnameMinMax);
    out3 << graphvizString3;
    out3.close();

    std::string graphvizString4 = permutationToGraphViz[max_max_nodes_pos];
    std::string fnameMaxMax = "BenchmarkQuasm/dot/" + file + "max_max" + std::to_string(qubits) + ".dot";
    std::ofstream out4(fnameMaxMax);
    out4 << graphvizString4;
    out4.close();

    size_t index;
    size_t max_nodes = 0;
    size_t active_nodes = 0;
    std::chrono::duration<double> time;
    std::chrono::duration<double> sum_time(0);

    for (const auto& entry : perm_by_active_nodes) {
    //save the vecotr of all permutation indices (also simulation time and max_nodes) with the same active_nodes count 
    auto& vec = entry.second;
    out << vec.size() << " permutations have " << entry.first << " active nodes.\n";
    //iterate over all the permutations that have the same active_nodes/ are in the vector and print the max_nodes, simulation time and the permutation itself
    for (const auto& permutation : vec) {
        index =  std::get<0>(permutation);
        max_nodes=  std::get<1>(permutation);
        time =  std::get<2>(permutation);
        sum_time += time;
        out << DDMinimizer::measurementToString({true,false,true}, index, max_nodes, active_nodes, time);
        out << DDMinimizer::permToString(permutations[index]) <<"\n";
        }
    out << "\n...........................\n";
    out << "Average time: " << std::scientific << std::setprecision(4) << (sum_time / vec.size()).count() << "s\n\n";
    sum_time = std::chrono::duration<double>(0);
    }
    out << "________________________________________________________________________________________________\n\n";
    for (const auto& entry : perm_by_max_nodes) {
        //save the vecotr of all permutation indices (also simulation time and active_nodes) with the same max_nodes count 
        const auto& vec = entry.second;
        out << vec.size() << " permutations have " << entry.first << " max nodes.\n";
        //iterate over all the permutations that have the same max_nodes/ are in the vector and print the active_nodes, simulation time and the permutation itself
        for (const auto& permutation : vec) {
            index = std::get<0>(permutation);
            active_nodes = std::get<1>(permutation);
            time = std::get<2>(permutation);
            sum_time += time;
            out << DDMinimizer::measurementToString({false,true,true}, index, max_nodes, active_nodes, time );;
            out << DDMinimizer::permToString(permutations[index]) <<"\n";
        }
        out << "\n...........................\n";
        out << "Average time: " << std::scientific << std::setprecision(4) << (sum_time / vec.size()).count() << "s\n\n";
        sum_time = std::chrono::duration<double>(0);
    }
    out << "________________________________________________________________________________________________\n\n";
    for (const auto& entry : perm_by_time) {
        time = entry.first; 
        const auto& perm = entry.second;
        out << "execution time of: " << std::scientific << std::setprecision(4) << time.count() << "s: ";
        //Each time should only have one entry. I think
        if (!perm.empty()) { 
        auto& permutation = perm.front(); 
        index = std::get<0>(permutation);
        active_nodes = std::get<1>(permutation);
        max_nodes = std::get<2>(permutation);
        out << DDMinimizer::measurementToString({true,true,false}, index, max_nodes, active_nodes, time );
        out << DDMinimizer::permToString(permutations[index]);
    }
        out << "\n";
    }
    out << "________________________________________________________________________________________________\n\n";
}

qc::Permutation DDMinimizer::createGateBasedPermutation(std::ofstream& out, qc::QuantumComputation& qc){
    return qc.initialLayout;
}


qc::Permutation DDMinimizer::createControlBasedPermutation(std::ofstream& out, qc::QuantumComputation& qc){
    std::map<Qubit, std::set<Qubit>> controlToTargets;

    for (const auto& op : qc.ops) {
        if(!op->isStandardOperation()){
            continue;
        }
        Controls controls = op->getControls();
        std::set<Qubit> targets = {op->getTargets().begin(), op->getTargets().end()};

        for (const auto& control : controls) {
            if (controlToTargets.find(control.qubit) == controlToTargets.end()) {
            // If the control does not exist in the map, add it with the current set of targetInts
                controlToTargets[control.qubit] = targets;
            } else {
            // If the control exists, insert the new targets into the existing set
                controlToTargets[control.qubit].insert(targets.begin(), targets.end());
            }
        }
    }
    std::size_t bits = qc.getNqubits();
    std::vector<Qubit> logicalQubits(bits);
    std::vector<Qubit> physicalQubits(bits);
    std::map<Qubit, int> qubitWeights;
    for (Qubit i = 0; i < bits; ++i) {
          physicalQubits[i] = i;
          logicalQubits[i] = i;
          qubitWeights.insert({i, 1});
    }

    //trying the approach of  control > target
    int weight = 1;
    for (const auto& pair : controlToTargets) {
    weight = qubitWeights[pair.first];

    for (const auto& target : pair.second) {
        weight = std::max(qubitWeights[target], weight+1);
    }
    weight++;
    qubitWeights[pair.first] = weight;
    }

    sort(physicalQubits.begin(), physicalQubits.end(), [&qubitWeights](const Qubit& a, const Qubit& b) {
        auto weightA = qubitWeights.find(a) != qubitWeights.end() ? qubitWeights.at(a) : 0;
        auto weightB = qubitWeights.find(b) != qubitWeights.end() ? qubitWeights.at(b) : 0;
        return weightA < weightB;
    });

    for(int i = 0; i < bits; i++){
        out << "Q: " << i << " w: " << qubitWeights[i] << " | ";
    }
    out << "\n";   
    
    qc::Permutation perm;

    for (std::size_t m = 0; m < bits; m++) {
               perm[logicalQubits[m]] = physicalQubits[m];
          }
    return perm;
}


std::vector<qc::Permutation> DDMinimizer::createAllPermutations(qc::QuantumComputation& qc) {
     std::size_t bits = qc.getNqubits();
     std::size_t permutation_count = DDMinimizer::factorial(bits);

     // vector with all possible Permutations
     std::vector<qc::Permutation> permutations;
     qc::Permutation perm;

     // prepare generation of all permutations
     // create vectors that can be shuffeled
     std::vector<Qubit> logicalQubits(bits);
     std::vector<Qubit> physicalQubits(bits);

     //vector that contains all the layouts;
     std::vector<std::vector<Qubit>> layouts;
     
     for (std::size_t i = 0; i < bits; ++i) {
          physicalQubits[i] = i;
          logicalQubits[i] = i;
     }

     std::sort(physicalQubits.begin(), physicalQubits.end());
     do {
          layouts.push_back(physicalQubits);
     } while (std::next_permutation(physicalQubits.begin(), physicalQubits.end()));


     //create all layouts
     //loop over all permutations
     for(std::size_t i = 0; i < permutation_count; i++){
          //loop over all logical bits and create one layout
          for (std::size_t m = 0; m < bits; m++) {
               perm[logicalQubits[m]] = layouts[i][m];
          }
          permutations.push_back(perm);
     } 
     
     return permutations;
}

std::size_t DDMinimizer::factorial(std::size_t n) {
    std::size_t result = 1;
    for (std::size_t i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

std::string DDMinimizer::readFileIntoString(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::stringstream buffer;
    std::string line;
        while (std::getline(fileStream, line)) {
            buffer << line << '\n';
    }
    return buffer.str();
}

std::string DDMinimizer::permToString(Permutation perm){
    std::ostringstream oss;
    for (const auto& entry : perm) {
        oss << entry.first << " -> " << entry.second << "|";
    }
    return oss.str();
}

/*
@param code 1: active, 2: max, 3: time
*/
std::string DDMinimizer::measurementToString(std::vector<bool> code, size_t index, size_t max_nodes, size_t active_nodes, std::chrono::duration<double> time){
    std::ostringstream oss;
    oss << "Index: " << DDMinimizer::formatSize_t(index) <<" | ";

    if(code[0]){
        oss << "Max_nodes: " << DDMinimizer::formatSize_t(max_nodes) << " | ";
    }
    if(code[1]){
        oss << "Active_nodes: " << DDMinimizer::formatSize_t(active_nodes) << " | ";
    }
    if(code[2]){
        oss << "Time: " << std::scientific << std::setprecision(4) << time.count() << "s | ";
    }
    return oss.str();
}

std::string DDMinimizer::formatSize_t(size_t t){
    std::ostringstream oss;
    if(t > 99){
        oss << t;
    }
    else if(t > 9){
        oss << t << " ";
    }
    else{
        oss << t << "  ";
    }
    return oss.str();
}

void DDMinimizer::removeAnsi(std::string filePath){
    // Open input file in read mode
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Read the contents of the file into a string using stringstream
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();

    const std::regex ansi_escape(R"(\x1B\[[0-?]*[ -/]*[@-~])");
    // Remove ANSI escape codes
    std::string cleanedContents = std::regex_replace(buffer.str(), ansi_escape, "");

    // Open the same file in write mode to overwrite it
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cout << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    // Write the cleaned contents back to the file
    outputFile << cleanedContents;
    outputFile.close();
}



};// namespace qc