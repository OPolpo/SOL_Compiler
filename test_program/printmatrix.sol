--test print printmatrix
func printmatrix():bool
var matrix: vector [10] of vector [10] of bool;
	i,j:int;
begin printmatrix
	for j=0 to 9 do 
		for i=0 to 9 do 
			--write i;
			matrix[i][j]=true;
		endfor;
	endfor;
	for j=0 to 9 do 
		for i=0 to 9 do 
			write matrix[i][j];
			write " ";
		endfor;
		write "\n";
	endfor;
	return true;
end printmatrix

