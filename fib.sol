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
