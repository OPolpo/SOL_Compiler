func base(x: int;):int
	var k:int;

	func f1(x:int;):int
	begin f1
		x=x+1;
		return x;
	end f1

	func f2(y:int;):int
	var x:int;

		func ff2():int
		begin ff2
			write k;
			write x;
			return f1(20);
		end ff2

	begin f2
		x=3;

		write "ff2 ";
		write ff2();
		
		return y+x+k;
	end f2

begin base
	k=4;
	write "f1 ";
	write f1(x);
	write x;
	write "f2 ";
	write f2(x);

	
	return f1(f2(x));
end base