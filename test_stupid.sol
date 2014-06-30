--test :P
func tester(d:real;):int
var a,b,c:int;
	str:string;
	boo:bool;
	fgh:char;
	vect : vector [4] of int;
const sss:string="2";
	pippo:char='a';
begin tester
	vect = vector(1,2,3,4);
	write vect;
	str="";
	b=2;
	c=3;
	write (pippo);
	a=0;
	b=0;
	c=0;
	boo=true;
	write (sss);
	str="ciao";
	boo = str == "cio";
	return a;
end tester