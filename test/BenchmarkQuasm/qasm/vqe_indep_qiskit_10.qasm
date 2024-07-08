// Benchmark was created by MQT Bench on 2024-03-19
// For more information about MQT Bench, please visit https://www.cda.cit.tum.de/mqtbench/
// MQT Bench version: 1.1.0
// Qiskit version: 1.0.2

OPENQASM 2.0;
include "qelib1.inc";
qreg q[10];
creg meas[10];
ry(-3.141235478528481) q[0];
ry(1.5706979045022578) q[1];
ry(-2.362236077972896e-05) q[2];
ry(-3.1399208277229347) q[3];
ry(-0.0011147396600802526) q[4];
ry(-1.1042563024883283) q[5];
ry(2.9801510328183927) q[6];
ry(3.0545670522151607) q[7];
ry(-1.3668142934358305) q[8];
ry(-2.9083999118928885) q[9];
cx q[8],q[9];
cx q[7],q[8];
cx q[6],q[7];
cx q[5],q[6];
cx q[4],q[5];
cx q[3],q[4];
cx q[2],q[3];
cx q[1],q[2];
cx q[0],q[1];
ry(1.599653982977371) q[0];
ry(-0.003280589550612297) q[1];
ry(0.0034061099135506466) q[2];
ry(-1.1679350002528048) q[3];
ry(pi) q[4];
ry(-2.7302797356431983) q[5];
ry(-2.6925756675472736) q[6];
ry(-0.941592140077449) q[7];
ry(-0.8734936311655543) q[8];
ry(-2.300413555921887) q[9];
cx q[8],q[9];
cx q[7],q[8];
cx q[6],q[7];
cx q[5],q[6];
cx q[4],q[5];
cx q[3],q[4];
cx q[2],q[3];
cx q[1],q[2];
cx q[0],q[1];
ry(1.5993512590035681) q[0];
ry(1.5708506148951475) q[1];
ry(0.0006026837145775064) q[2];
ry(-1.5710638349849695) q[3];
ry(1.1673330914635904) q[4];
ry(-1.9859733062653429) q[5];
ry(2.8994628483268934) q[6];
ry(-1.6023537028276675) q[7];
ry(-1.493367754442654) q[8];
ry(0.5041276741646578) q[9];
barrier q[0],q[1],q[2],q[3],q[4],q[5],q[6],q[7],q[8],q[9];
measure q[0] -> meas[0];
measure q[1] -> meas[1];
measure q[2] -> meas[2];
measure q[3] -> meas[3];
measure q[4] -> meas[4];
measure q[5] -> meas[5];
measure q[6] -> meas[6];
measure q[7] -> meas[7];
measure q[8] -> meas[8];
measure q[9] -> meas[9];