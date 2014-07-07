func call():int
	var readedNumb:real;
	const msgReqNumb:string = "Please, insert a real number\n";
	--power
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
		endif;
		for i = 1 to exponent do 
			res = res * base;
		endfor;
		if (rational) then
			return 1.0 / res;
		else
			return res;
		endif;
	end pow
begin call
	write msgReqNumb;
	read readedNumb;
	write "the power of ";
	write readedNumb;
	write " is: ";
	write pow(readedNumb,2);
	write "\n";
	return 0;
end call
	
