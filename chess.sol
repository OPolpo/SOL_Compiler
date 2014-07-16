--test chess
func chess():bool
var chessboard: vector [10] of vector [10] of bool;
	i,j:int;
begin chess
	for j=0 to 9 do 
		for i=0 to 9 do 
			write i;
			chessboard[i][j]=true;
		endfor;
	endfor;
	for j=0 to 9 do 
		for i=0 to 9 do 
			write chessboard[i][j];
			write " ";
		endfor;
		write "\n";
	endfor;
	return true;
end chess

