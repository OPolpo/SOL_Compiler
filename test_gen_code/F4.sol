--test Volpe Forza 4 XD
func F4():bool
	var board: vector [7] of vector [6] of int;
		i,j:int;
		userchoice:int;
		player:int;
		dummy:bool;

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
			write "-----------------------------------------------------------\n";
			for j=0 to 5 do 
				write "||-------------------------------------------------------||\n";
				write "||       |       |       |       |       |       |       ||\n";
				write "||";
				for i=0 to 6 do 
					write "   ";
					write board[i][5-j];
					write "   |";
				endfor;
				write "|\n";
				write "||       |       |       |       |       |       |       ||\n";
	
			endfor;
			write "-----------------------------------------------------------\n";
			write "-----------------------------------------------------------\n";
	
			return true;
		end printboard
	
	func cls():bool
		begin cls
			write("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			return true;
		end cls

	func printheader(player:int;):bool
		begin printheader
			write "-----------------------------------------------------------\n";
			write "------------------      ";
			if (player == 1) then
				write "PLAYER  ONE";
			else
				write "PLAYER  TWO";
			endif;
			write "      ------------------\n";
			write "-----------------------------------------------------------\n";
			return true;
		end printheader

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

	begin F4
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
			write "select the column\n";

			read userchoice;
			if (userchoice>0 and userchoice<8) then
				if(insertoken(userchoice, player)) then
					if checkwin() then
						dummy = printwinner(player);
						return true;
						--return true;
					endif;			
					--dummy = cls();
					dummy = printheader(player);
					dummy = printboard();
					dummy=false;
				else
					write "This column is full\n";
				endif;
			else
				write "Please, insert a number between 1 and 7\n";
			endif;
			if player == 1 then
				player = 2;
			else
				player = 1;
			endif;
		endwhile;
		return true;
	end F4

