// Benchmark was created by MQT Bench on 2024-03-19
// For more information about MQT Bench, please visit https://www.cda.cit.tum.de/mqtbench/
// MQT Bench version: 1.1.0
// Qiskit version: 1.0.2

OPENQASM 2.0;
include "qelib1.inc";
qreg node[3];
qreg coin[1];
creg meas[4];
u2(pi/8,-pi) node[0];
p(pi/8) node[1];
p(pi/8) node[2];
u2(pi/8,-pi) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
u1(pi/8) node[0];
ccx coin[0],node[2],node[1];
u3(pi,-pi,-pi/8) node[1];
cx coin[0],node[2];
u3(pi,-pi,-pi/8) node[2];
u3(pi,-pi,-pi/8) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
u1(pi/8) node[0];
ccx coin[0],node[2],node[1];
u3(pi,-pi,-pi/8) node[1];
cx coin[0],node[2];
u3(pi,-pi,-pi/8) node[2];
u2(-7*pi/8,-pi) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
u1(pi/8) node[0];
ccx coin[0],node[2],node[1];
u3(pi,-pi,-pi/8) node[1];
cx coin[0],node[2];
u3(pi,-pi,-pi/8) node[2];
u3(pi,-pi,-pi/8) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
u1(pi/8) node[0];
ccx coin[0],node[2],node[1];
u3(pi,-pi,-pi/8) node[1];
cx coin[0],node[2];
u3(pi,-pi,-pi/8) node[2];
u2(-7*pi/8,-pi) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
u1(pi/8) node[0];
ccx coin[0],node[2],node[1];
u3(pi,-pi,-pi/8) node[1];
cx coin[0],node[2];
u3(pi,-pi,-pi/8) node[2];
u3(pi,-pi,-pi/8) coin[0];
cx coin[0],node[1];
p(-pi/8) node[1];
cx coin[0],node[1];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
p(pi/8) node[2];
cx node[1],node[2];
p(-pi/8) node[2];
cx coin[0],node[2];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx node[1],node[0];
p(pi/8) node[0];
cx node[2],node[0];
p(-pi/8) node[0];
cx coin[0],node[0];
h node[0];
ccx coin[0],node[2],node[1];
x node[1];
cx coin[0],node[2];
x node[2];
x coin[0];
barrier node[0],node[1],node[2],coin[0];
measure node[0] -> meas[0];
measure node[1] -> meas[1];
measure node[2] -> meas[2];
measure coin[0] -> meas[3];