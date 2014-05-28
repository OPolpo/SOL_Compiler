func prog(): int
	func f1(n:int;): typeint 
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
	end f1

	func f2(c:bool;): bool 
	var person: struct(name, surname: string; birth: int;);
		book: struct(author: struct(name, surname: string;); title: string; edition: struct(editor: string; year: int;););
		numbers: vector [10] of int;
		matrix: vector [3] of vector [5] of int;
		people: vector [4] of struct(name, surname: string; birth: int;);

		ok:bool; 
	begin f1 
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
		matrix[3][4] = matrix[3][8];
	end f1
	
begin prog 
	
book = struct(struct("Carl Gustav", "Jung"), "Man and his symbols",
struct("ETA", 1992));
	x = toreal(i+j)*(r-toreal(i));
	j = toint(x+y-1.25);
end prog