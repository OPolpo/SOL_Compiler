func tester():int
type game:struct(board: vector [7] of vector [6] of int; properclose: bool; player: int;);
var k:int;
	v:game;
begin tester
	read k;
	write k;
	read k;
	write k;
	read ["a.txt"] v;	
	write v;
	read ["b.txt"] v;	
	write v;
	read k;
	write k;
	return 100;
end tester