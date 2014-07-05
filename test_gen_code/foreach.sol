--test :P
func tester2():int
var v: vector [1] of int; k,sum: int;
	
begin tester2
	read v;
	sum = 0;
	foreach k in v do
		sum = sum + k;
		write sum;
	endforeach;
	return sum;
end tester2