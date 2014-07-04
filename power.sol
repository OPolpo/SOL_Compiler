func call():real
	--power
	func pow(base: real; exponent:int;): real
		var res:real;
			i:int;
			rational:bool;
	begin pow
		res=base;
		if (exponent == 0) then
			return 1.0;
		elsif(exponent < 0) then
			exponent = -exponent;
			rational = true;
		endif;
		for i = 0 to exponent do 
			res = res * base;
		endfor;
		if (rational) then
			return 1.0 / res;
		else
			return res;
		endif;
	end pow
begin call
	write pow(2.0,10);
	return 1.0;
end call
	
