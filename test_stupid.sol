--test :P
func tester():int
var a,b,c:int;
	str:string;
	boo:bool;
	fgh:char;
	vect : vector [4] of int;
	sw : vector [2] of string;
	MAT: vector [2] of vector [5] of int;
const sss:string="2";
	pippo:char='a';
begin tester
sw  = vector("abc","def");

	--vect = vector(3,6,4,7);
	--MAT = vector(vector(1,2,3,4,5), vector(2,4,6,8,10));
	write "stringa";
	write "aloha";
	write vector("abc","def");
	write sw;
	--write vect;
	--write MAT;
	str="";
	write (pippo);
	
	--write (sss);
	
	return a;
end tester

--(char *) $29 = 0x0000000100200010 "abc"
--(char *) $36 = 0x0000000100103a10 "def"
--$63 = 0x0000000100400058 "\x10"
