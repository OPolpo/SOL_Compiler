func tester():int
type game:struct(board: vector [3] of vector [2] of int; properclose: bool; player: int;);
var --k:int;
	v:game;
	b:bool;
	i,j:int;
	i2:vector [2] of int;
	i6:vector [3] of vector [2] of int;
begin tester
	--v = struct(vector(vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(1, 2, 1, 2, 1, 2), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0), vector(0, 0, 0, 0, 0, 0)),true,1);
	--v = struct(vector(vector(3,4),vector(2,3),vector(6,3)), true, 7);
	--write ["a.txt"] v;
	--write v;
	i=11;
	j=wr i;
	write j;
	j = rd ["b.txt"] int + 1;
	write j;
	--write v;

	--read ["a.txt"] v;
----write v;
--read i;
----b=v.properclose;
--write i;
--i=v.board[0][0];
--write i;
--i2=v.board[1];
--write i2;
--i6=v.board;
--write i6;

	return -1;
end tester