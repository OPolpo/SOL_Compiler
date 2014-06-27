func tester():int
var i:int;
	s:string;
	r:struct(a:int;b:string;c:struct(d:int;e:string;););
	v: vector[5] of string;
	vi: vector[5] of int;
	vc: vector[5] of char;

begin tester
	r = struct(9,"ilaria",struct (7,"andrea"));
	v = vector("2","ciao","prova","pippo","pluto");
	vi = vector(1,2,3,4,5);
	vc = vector('a','b','c','d','e');
	write i + 25;
	write vi;
	write vc;
	write v;
	return 100;
end tester