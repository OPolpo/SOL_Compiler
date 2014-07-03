--test :P
func f2(): bool 
	var --person: struct(name, surname: string; birth: int;);
		--book: struct(author: struct(name, surname: string;); title: string; edition: struct(editor: string; year: int;););
		--numbers: vector [10] of int;
		--matrix: vector [3] of vector [5] of int;
		--people: vector [4] of struct(name, surname: string; birth: int;);
		--num:int;
		ok:bool; 
	begin f2
	----read num;
	----write num;

	----read ok;
	----write ok;
	----person = struct("John", "Smith", 1987);
	-----write person;
	----read person;
	--write person.name;
	--write person.surname;
	--write person.birth;
	--numbers= vector (1,2,3,4,5,6,7,8,9,0);
	----read numbers;
	--write numbers;
	--
	--
	--matrix = vector(vector(1,2,3,4,5), 
	--		vector(2,4,6,8,10),
	--		vector(3,6,9,12,15));
	--write ["pippo.ff"] matrix;
	--write matrix;
	--
	--book = struct(struct("Carl Gustav", "Jung"), "Man and his symbols", struct("ETA", 1992));
	--write ["pippo.txt"] book;
	----read ["pippo.txt"] book;
	----people = vector(struct("John", "Smith", 1983), 
	----				struct("Rosy", "White", 1960),
	----				struct("Louis", "Green", 1998), 
	----				struct("Ann", "Black", 2001));
	----write people;
	----ok = person.name == "John";
	read ok;
	write ok;
	ok=true;
		return ok;
	end f2
