func tester():int
var --k:int;
	v:struct(board: vector [7] of vector [6] of int; properclose: bool; player: int;);
begin tester
	--v = struct(vector(vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0)),true,1);
	--write ["a.txt"] v;
	--write v;
	read ["a.txt"] v;
	write v;
	return 100;
end tester