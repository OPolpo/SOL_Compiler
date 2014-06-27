func tester():int
var i:int;
	s:string;
	r:struct(a:int;b:string;c:struct(d:int;e:string;););

begin tester
	r = struct(9,"ilaria",struct (7,"andrea"));
	write i + 25;
	write [ s ] r;
	return 100;
end tester