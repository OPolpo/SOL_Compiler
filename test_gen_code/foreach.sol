--test :P
func tester2():int
var v: vector [2] of int; k,sum: int;
	
begin tester2
	read v;
	sum = 0;
	foreach k in v do
		sum = sum + k;
		write "sum: ";
		write sum;
		write " ";
	endforeach;
	return sum;
end tester2