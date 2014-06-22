func tester():int
var i,j: int;
	v1: vector [10] of int;
	v2: vector [10] of vector [20] of int;
	lab:string;
begin tester
	lab = "reference";
	write v1[i+j];
	write v2[i-j][i+j];
	lab = "assignment";
	v1[i+j] = i*j;
	v2[i-j][i+j] = i*j;
	return 100;
end tester