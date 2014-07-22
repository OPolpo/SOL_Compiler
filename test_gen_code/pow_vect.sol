--test :P
func tester2():int
	var v: vector [3] of int;
		r: vector [3] of real;
		k:int;
		i:int;

	func pow(base: real; exponent:int;): real
		var res:real;
			i:int;
			rational:bool;
	begin pow
		res=1.0;
		rational = false;
		if (exponent == 0) then
			return 1.0;
		elsif(exponent < 0) then
			exponent = -exponent;
			rational = true;
		elsif(exponent == 5) then --test
			exponent = -exponent; --test
			rational = true; --test
		endif;
		for i = 1 to exponent do 
			res = res * base;
		endfor;
		if (rational) then
			return 1.0 / res;
		else
			return res;
		endif;
		return res;
	end pow

begin tester2
	read v;
	write v;
	i=0;
	foreach k in v do
		r[i]=pow(toreal(k), 2);
		i=i+1;
	endforeach;
	write r;
	return i;
end tester2