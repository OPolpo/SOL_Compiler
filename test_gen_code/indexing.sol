func tester():int
var i,j: int;
	v1: vector [10] of int;
	v2: vector [10] of vector [20] of int;
	lab:string;
begin tester

for i=0 to 9 do
	v1[i] = i;
endfor;
write v1;


	for i=0 to 9 do
		for j=0 to 19 do
			v2[i][j] = i+j;
			--write 10;
		endfor;
	endfor;

	i = 3;
	j=3;
	write v2;
	write "reference";
	write v1[i+j];
	write v2[i-j][i+j];
	
	write "assignment";
	v1[i+j] = i*j;
	v2[i-j][i+j] = i*j;
	write v1;
	write v2;
	
	return 100;
end tester