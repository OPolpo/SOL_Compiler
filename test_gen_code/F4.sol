--test FOX FORCE 4 XD
func FF4():bool
	var board: vector [7] of vector [6] of int;
		i,j:int;
		userchoice:int;
		player:int;
		dummy:bool;
		ok, okfull, first:bool;

	func insertoken(cols:int; player:int;):bool
		var i:int;
		begin insertoken
			for i=0 to 5 do
				if board[cols-1][i] == 0 then
					board[cols-1][i] = player;
					return true;
				endif;
			endfor;
			return false;
		end insertoken
	
	func printboard():bool
		var i,j:int;
		begin printboard
			--write "-----------------------------------------------------------\n";
			for j=0 to 5 do 
				write "||-------------------------------------------------------||\n";
				write "||       |       |       |       |       |       |       ||\n";
				write "||";
				for i=0 to 6 do 
					write "   ";
					if (board[i][5-j] == 0) then
						write " ";
					else
						write board[i][5-j];
					endif;
					write "   |";
				endfor;
				write "|\n";
				write "||       |       |       |       |       |       |       ||\n";
	
			endfor;
			write "-----------------------------------------------------------\n";
			--write "-----------------------------------------------------------\n";
	
			return true;
		end printboard

	func printboard2():bool
	var i,j:int;
	begin printboard2
		--write "-----------------------------------------------------------\n";
		for j=0 to 5 do 
			write "||-------------------------------------------------------||\n";
			--write "||       |       |       |       |       |       |       ||\n";
			write "||";
			for i=0 to 6 do 
				if (board[i][5-j] == 0) then
					write "       |";
				else
					if(board[i][5-j] == 1) then
						write "   *   |";
					else
						write "  ###  |";
					endif;
				endif;
			endfor;
			write "|\n";
			write "||";

			for i=0 to 6 do 
				if (board[i][5-j] == 0) then
					write "       |";
				else
					if(board[i][5-j] == 1) then
						write "  ***  |";
					else
						write " ##### |";
					endif;
				endif;
			endfor;
			write "|\n";
			write "||";

			for i=0 to 6 do 
				if (board[i][5-j] == 0) then
					write "       |";
				else
					if(board[i][5-j] == 1) then
						write " ***** |";
					else
						write "  ###  |";
					endif;
				endif;
			endfor;
			write "|\n";
			--write "||       |       |       |       |       |       |       ||\n";
	
		endfor;
		write "-----------------------------------------------------------\n";
		--write "-----------------------------------------------------------\n";
	
		return true;
	end printboard2
	
	func cls():bool
		begin cls
			write("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			return true;
		end cls

	func printheader(player:int;):bool
		begin printheader
			dummy = gamename();
			write "-----------------------------------------------------------\n";
			write "------------------      ";
			if (player == 1) then
				write "PLAYER  ONE";
			else
				write "PLAYER  TWO";
			endif;
			write "      ------------------\n";
			return true;
		end printheader
	
	func gamename():bool
		begin gamename
			write "             _______  _______                ___   \n";
			write "            (  ____ \(  ____ \              /   )  \n";
			write "            | (    \/| (    \/             / /) |  \n";
			write "            | (__    | (__       _____    / (_) (_ \n";
			write "            |  __)   |  __)     (_____)  (____   _)\n";
			write "            | (      | (                      ) (  \n";
			write "            | )      | )                      | |  \n";
			write "            |/       |/                       (_)  \n\n\n\n\n";
			return true;
		end gamename

	func checkwin():bool
		var res:bool;

		func checkrows():bool
			var i, j, count:int;
			begin checkrows
				for i=0 to 5 do
				count = 0;
					for j=1 to 6 do
						if(board[j][i] != 0 and board[j][i] == board[j-1][i]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;
				return false;
			end checkrows
		func checkcols():bool
			var i, j, count:int;
			begin checkcols
				for j=0 to 6 do
				count = 0;
					for i=1 to 5 do
						if(board[j][i] != 0 and board[j][i] == board[j][i-1]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;
				return false;
			end checkcols
		func checkdiag():bool
			var i, d, count:int;
			begin checkdiag
				for d=1 to 3 do
				count = 0;
					for i=1 to 5 - (d-1) do
						if(board[i+d][i] != 0 and board[i+d][i] == board[i-1+d][i-1]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;
				for d=1 to 3 do
				count = 0;
					for i=1 to 5 - (d-1) do
						if(board[6-(i+d)][i] != 0 and board[6-(i+d)][i] == board[6-(i-1+d)][i-1]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;
				for d=1 to 3 do
				count = 0;
					for i=1 to 5 - (d-1) do
						if(board[i][i+(d-1)] != 0 and board[i][i+(d-1)] == board[i-1][i+(d-1)-1]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;
				for d=1 to 3 do
				count = 0;
					for i=1 to 5 - (d-1) do
						if(board[6-i][i+(d-1)] != 0 and board[6-i][i+(d-1)] == board[6-(i-1)][i+(d-1)-1]) then
							count = count + 1;
							if count == 3 then
								return true;
							endif;
						else
							count = 0;
						endif;
					endfor;
				endfor;

				return false;
			end checkdiag

		begin checkwin
			res = checkrows() or checkcols() or checkdiag();
			return res;
		end checkwin

	func printwinner(player:int;):bool
		begin printwinner
			write "PLAYER ";
			write player;
			write " WINS\n";
			return false;
		end printwinner

	func printmenu():bool
		begin printmenu
			return true;
		end printmenu

	func refreshscreen():bool
	begin refreshscreen
		dummy = cls();
		dummy = printheader(player);
		dummy = printboard2();
		return true;
	end refreshscreen

	begin FF4
		for j=0 to 6 do 
			for i=0 to 5 do 
				--write i;
				board[i][j]=0;
			endfor;
		endfor;
		player = 1;
		dummy = cls();
		dummy = printheader(player);
		dummy = printboard();
		while true do
			dummy = refreshscreen();
			write "select the column\n";
			read userchoice;
			ok=true;
			okfull=true;
			first = true;
			while(not ok or not okfull or first) do
				first=false;
				if (not ok) then
					dummy = refreshscreen();
					write "Please, insert a number between 1 and 7\n";
					read userchoice;
				endif;
				if(userchoice>=1 and userchoice<=7) then
					ok = true;
				else
					ok = false;
				endif;
				if(ok) then
					if(insertoken(userchoice, player)) then
						okfull = true;
						if checkwin() then
							dummy = refreshscreen();
							dummy = printwinner(player);
							return true;
						endif;
					else
						okfull = false;
						dummy = refreshscreen();
						write "This column is full, try again\n";
						read userchoice;
					endif;
				endif;
			endwhile;

			if player == 1 then
				player = 2;
			else
				player = 1;
			endif;
		endwhile;
		return true;
	end FF4

