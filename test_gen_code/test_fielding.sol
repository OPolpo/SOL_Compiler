func tester():int
var i,j: int;
	r: struct(a: real; b: int;);
	r1: struct(a: real; r2: struct(c: char; d: vector [10] of string;);); 
	r3: struct(a: real; r4: struct(c: char; d: int;););
	lab:string;
begin tester
	lab = "reference";
	write (i+r.b);
	write r1.r2.d;
	lab = "assignment";
	r.b = i+j;
	r3.r4.d = i+j;
	return 100;
end tester