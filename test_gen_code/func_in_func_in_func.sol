func alpha(x: int;):int
	var i:int;
func a(po:int;):string
	func b(poo:int;):string
		func c(po:int;):string
			func d(poo:int;):string
			begin d
				return a(10000000);
			end d
		begin c
			return a(100000);
		end c
	begin b
		return a(1000);
	end b
begin a
	return a(10);
end a

begin alpha
	i= 1;
	return alpha(10);
end alpha