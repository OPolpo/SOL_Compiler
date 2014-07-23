func tester():int
var i:int;
	s:string;
	r:struct(a:int;b:char;c:struct(d:int;e:real;););
	rs:struct(a:int;b:char;c:int;d:char;);
	v: vector[5] of string;
	vi: vector[5] of int;
	vc: vector[5] of char;
	vr: vector[5] of real;

begin tester
	r = struct(9,'v',struct (7,4.5));
	rs = struct(8,'s',7898,'t');
	v = vector("2","ciao","prova","pippo","pluto");
	vi = vector(123456789,2,3,4,5);
	vc = vector('a','b','c','d','e');
	vr = vector(3.6,3.5,6.8,9.9,4.5);
	write i + 25;
	write rs;
	write r;
	write vi;
	write vc;
	write vr;
	write v;
	return 100;
end tester