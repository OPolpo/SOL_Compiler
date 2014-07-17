func tester():int
--type game:struct(board: vector [7] of vector [6] of int; properclose: bool; player: int;);
var --k:int;
	--v:game;
	a:struct(board: vector [7] of vector [6] of int; properclose: bool; player: int;);
begin tester
	--a = struct(vector(vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0)),true,1);
	--write ["a.txt"] v;
	a.properclose=true;
	write a.properclose;
	a.properclose=false;
	write a.properclose;
	--read ["a.txt"] v;	
	--write v;
--	v.properclose=false;
--	write v;
	return 100;
end tester