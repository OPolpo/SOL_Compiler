--test :P
func tester2():int
var v: vector [10] of int; k,sum: int;
	
begin tester2
	foreach k in v do
		sum = sum + k;
	endforeach;
	return 1;
end tester2