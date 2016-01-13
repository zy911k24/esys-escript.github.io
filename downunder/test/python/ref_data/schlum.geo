lc=10.000000;
Point(1)={-200.000000, -200.000000, 0, lc};
Point(2)={200.000000, -200.000000, 0, lc};
Point(3)={200.000000, 200.000000, 0, lc};
Point(4)={-200.000000, 200.000000, 0, lc};
Line(1) = {1,2} ;
Line(2) = {3,2} ;
Line(3) = {3,4} ;
Line(4) = {4,1} ;
Line Loop(5) = {4,1,-2,3} ; 
Plane Surface(6) = {5} ; 
Point(5)={-27.500000,0.000000,0.000000,1.000000};
Point(6)={-22.500000,0.000000,0.000000,1.000000};
Point(7)={-17.500000,0.000000,0.000000,1.000000};
Point(8)={-12.500000,0.000000,0.000000,1.000000};
Point(9)={-7.500000,0.000000,0.000000,1.000000};
Point(10)={-2.500000,0.000000,0.000000,1.000000};
Point(11)={2.500000,0.000000,0.000000,1.000000};
Point(12)={7.500000,0.000000,0.000000,1.000000};
Point(13)={12.500000,0.000000,0.000000,1.000000};
Point(14)={17.500000,0.000000,0.000000,1.000000};
Point(15)={22.500000,0.000000,0.000000,1.000000};
Point(16)={27.500000,0.000000,0.000000,1.000000};
out0[]=Extrude {0, 0, -300.000000} { Surface {6};};
Point{5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} In Surface{6};
Physical Volume("volume-1") = {1} ;
Physical Surface("Top") = { -6 };
Physical Surface("Bottom") = { -out0[0] };
Physical Surface("Left") = { -out0[2] };
Physical Surface("Right") = { -out0[4] };
Physical Surface("Front") = { -out0[5] };
Physical Surface("Back") = { -out0[3] };
Field[1] = Box;
Field[1].VIn=lc;
Field[1].VOut=5*lc;
Field[1].XMax=100.000000;
Field[1].XMin=-100.000000;
Field[1].YMax=100.000000;
Field[1].YMin=-100.000000;
Field[1].ZMax=0;
Field[1].ZMin=-200.000000;
Field[2] = Attractor;
Field[2].NodesList = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
Field[3] = Threshold;
Field[3].IField = 2;
Field[3].LcMin = lc / 5;
Field[3].LcMax = 100*lc;
Field[3].DistMin = 70;
Field[3].DistMax = 100;
Field[4] = Min;
Field[4].FieldsList = {1, 3};
Background Field = 4;
Mesh.CharacteristicLengthExtendFromBoundary = 0;