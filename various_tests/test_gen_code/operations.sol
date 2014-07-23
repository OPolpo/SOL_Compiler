func operations():int
var g:bool;
	k:int;
	label:string;

	func logical():int
	var a,b,c:bool;
	begin logical
		a= (b and c) or g;
		return 1000;
	end logical

	func relational():int
	var b:bool;
		i,j:int;
		x,y:real;
		v:vector[100] of int;
	begin relational
		label="== and !=";
		b= (i == j or j!=k);
		label=">, >=, <, <=";
		b= (i>j or x<y);
		label="in";
		b= i in v;
		return 1000;
	end relational

	func arithmetic():int
	var i, j, k: int;
		x, y: real;
	begin arithmetic
		i = (i + 5) * (j - k);
		x = (y + 3.14) / (x + y);
		return 10000;
	end arithmetic

	func negation():int
	var i, j, k: int;
		a,b: bool;
	begin negation
		b = i > j * k and not (a or j == -k);
		return 10000;
	end negation

begin operations
	return 2000;
end operations