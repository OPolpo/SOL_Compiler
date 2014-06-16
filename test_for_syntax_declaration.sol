func prog(a,x: int; b: string; c: struct(nome,cognome: string; anno:int;); d:real; e: vector [4] of char;): int
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
	const 	MAX: int = 100;
			name: T2 = "alpha";
			VECT: vector [5] of real = vector(2.0, 3.12, 4.67, 1.1, 23.0);
			MAT: vector [2] of vector [5] of real = vector(VECT, vector(x, y, toreal(z), 10.0, x+y+toreal(z)));

	func f1(n:int;): bool 
		var c:char;
			i:int;
			r:real;
			s:string;
			ok:bool; 
	begin f1 
		c = 'a';
		i = 25;
		r = 64.15;
		s = "alpha";
		ok = true;
		return c > 'a';
	end f1

	func f2(c:bool;): bool 
	var person: struct(name, surname: string; birth: int;);
		book: struct(author: struct(name, surname: string;); title: string; edition: struct(editor: string; year: int;););
		numbers: vector [10] of int;
		matrix: vector [3] of vector [5] of int;
		people: vector [4] of struct(name, surname: string; birth: int;);

		ok:bool; 
	begin f2
		person = struct("John", "Smith", 1987);
		book = struct(struct("Carl Gustav", "Jung"), "Man and his symbols", struct("ETA", 1992));
		numbers= vector (1,2,3,4,5,6,7,8,9,0);
		matrix = vector(vector(1,2,3,4,5), 
				vector(2,4,6,8,10),
				vector(3,6,9,12,15));
		people = vector(struct("John", "Smith", 1983), 
						struct("Rosy", "White", 1960),
						struct("Louis", "Green", 1998), 
						struct("Ann", "Black", 2001));
		ok = person.name == "John";
		return ok;
	end f2
	
begin prog 
	x = toreal(i+j)*(r-toreal(i));
	j = toint(x+y-1.25);
	return 9;
end prog