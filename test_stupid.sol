--test :P
func tester():int
var a,b,c:int;
	str:string;
	boo:bool;
	fgh:char;
const sss:string="2";
	pippo:int=99;
	func tester1():int
	var a1,b1,c1:int;
	
		func tester2():int
		var a2,b2,c2:int;
		begin tester2
			a=0;
			a2=tester1();
			return a;
		end tester2

	begin tester1
		a=0;
		return a;
	end tester1

begin tester
	str="";
	b=2;
	c=3;
	write (pippo);
	a=0;
	str="ciao";
	boo = str == "cio";
	return a;
end tester