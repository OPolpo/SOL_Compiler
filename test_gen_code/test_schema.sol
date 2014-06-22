func prog(aa,xx: int; b: string; c: struct(nome,cognome: string; anno:int;); d:real; e: vector [4] of char;): int
	type	TO1: int; 
			T2: string;
			T3, T4: T2;
			T5: vector [10] of T3;
			typeint: struct(nome,cognome: string; anno:int; a: struct(b,c:int;););
	var i, j: int; 
		z: int;
		x, y: real;
		s, t: T5;
		a: vector [100] of int;
		r: real;
		person: struct(name, surname: string; birth: int;);
		book: struct(author: struct(name, surname: string;); title: string; edition: struct(editor: string; year: int;););
		numbers: vector [10] of int;
		matrix: vector [3] of vector [5] of int;
		people: vector [4] of struct(name, surname: string; birth: int;);

	const 	MAX: int = 100;
			name: T2 = "alpha";
			VECT: vector [5] of real = vector(2.0, 3.12, 4.67, 1.1, 23.0);
			MAT: vector [2] of vector [5] of real = vector(VECT, vector(x, y, toreal(z), 10.0, x+y+toreal(z)));
	
begin prog 
	x = toreal(i+j)*(r-toreal(i));
	j = toint(x+y-1.25);
	return 9;
end prog