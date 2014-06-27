func tester():int
var i:int;
	s:string;
	r:struct(a:int;b:string;c:struct(d:int;e:string;););
	v: vector[5] of string;
	vi: vector[5] of int;
	vc: vector[5] of char;
	vr: vector[5] of real;

begin tester
	r = struct(9,"ilaria",struct (7,"andrea"));
	v = vector("2","ciao","prova","pippo","pluto");
	vi = vector(123456789,2,3,4,5);
	vc = vector('a','b','c','d','e');
	vr = vector(3.6,3.5,6.8,9.9,4.5);
	write i + 25;
	write vi;
	write vc;
	write vr;
	write v;
	return 100;
end tester