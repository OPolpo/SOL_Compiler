--test :P
func tester2():int
var v: vector [2] of vector [2] of int; 
k:  vector [2] of int; 
j: int;
	
begin tester2
	v = vector(vector(1,2),vector(3,4));
	foreach k in v do
		write "k: ";
		write k;
		write "\n";
		foreach j in k do
			write "j: ";
			write j;
			write "\n";
		endforeach;
	endforeach;
	return j;
end tester2