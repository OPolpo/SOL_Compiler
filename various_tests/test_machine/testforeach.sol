--test chess
func chess():bool
var chessboard: vector [2] of int;
	cell: int;
begin chess
	--write "qua";
	chessboard = vector(0,1);
	foreach cell in chessboard do
		write "inner";
		foreach cell in chessboard do
			write "inner-inner";
		endforeach;
	endforeach;
	--write "here";
	write chessboard;
	return true;
end chess

