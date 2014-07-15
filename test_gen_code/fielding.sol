func tester():int
var i,j: int;
	r: struct(a: real; b: int;);
	r1: struct(a: real; r2: struct(c: char; d: vector [10] of string;);); 
	r3: struct(a: real; r4: struct(c: char; d: int;););
	lab:string;
begin tester
	lab = "reference";
	r = struct(3.14, toint(3.14));
	r1 = struct(3.6, struct('2', vector("aa","ss","dd","ff","gg","hh","jj","kk","ll","zz")));
	i=1;
	j=2;
	r3 = struct(2.55, struct('w',4));

	write (i+r.b);
	write r1.r2.d;
	lab = "assignment";
	r.b = i+j;
	r3.r4.d = i+j;

	write r.b;
	write r3.r4.d;

	return 100;
end tester