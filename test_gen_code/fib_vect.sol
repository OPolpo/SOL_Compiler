--test :P
func tester2():int
	var v: vector [5] of int;
		r: vector [5] of int;
		i, k:int;

	func fib(n:int;): int
	begin fib
		write n;
		write "\n";
		if (n == 0) then
			return 0;
		elsif(n == 1) then
			return 1;
		else
			return fib(n-2) + fib(n-1);
		endif;
	end fib

begin tester2
	read v;
	i=0;
	foreach k in v do
		r[i]=fib(k);
		i=i+1;
	endforeach;
	write r;
	return i;
end tester2