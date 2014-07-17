--test FOX FORCE 4 XD
func FF4():bool
	type gametype: struct(board: vector [7] of vector [6] of int; properclose: bool; player: int;);
	var --board: vector [7] of vector [6] of int;
		i,j:int;
		userchoice:int;
		--player:int;
		dummy:bool;
		ok, okfull, first:bool;
		game:gametype;
		altro:gametype;

	func insertoken(cols:int; player:int;):bool
		var i:int;
		begin insertoken
			for i=0 to 5 do
				if game.board[cols-1][i] == 0 then
					game.board[cols-1][i] = game.player;
					return true;
				endif;
			endfor;
			return false;
		end insertoken

	func printboard():bool
	var i,j:int;
		board: vector [7] of vector [6] of int;
		
	begin printboard
		board=game.board;

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
		write "|    1   |   2   |   3   |   4   |   5   |   6   |   7    |\n";
		write "-----------------------------------------------------------\n";
	
		return true;
	end printboard
	
	func cls():bool
		begin cls
			write("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			return true;
		end cls

	func printheader(player:int;):bool
		func gamename():bool
			begin gamename
				write "             _______  _______                ___   \n";
				write "            (  ____ \(  ____ \              /   )  \n";
				write "            | (    \/| (    \/             / /) |  \n";
				write "            | (__    | (__       _____    / (_) (_ \n";
				write "            |  __)   |  __)     (_____)  (____   _)\n";
				write "            | (      | (                      ) (  \n";
				write "            | )      | )                      | |  \n";
				write "            |/       |/                       (_)  \n\n\n\n";
				return true;
			end gamename

		begin printheader
			dummy = gamename();
			write "-----------------------------------------------------------\n";
			write "------------------      ";
			if (game.player == 1) then
				write "PLAYER  ONE";
			else
				write "PLAYER  TWO";
			endif;
			write "      ------------------\n";
			return true;
		end printheader
	

	func checkwin():bool
		var res:bool;
			board: vector [7] of vector [6] of int;

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
			board=game.board;
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
			write "|---------------------------------------------------------|\n";
			write "|                                                         |\n";
			write "|                                                         |\n";
			write "|                                                         |\n";
			write "|                                                         |\n";
			write "|---------------------------------------------------------|\n";
			return true;
		end printmenu

	func printmessage(message:string; len:int;):bool
		var i:int;
		begin printmessage
			write "|";
			for i=0 to (55-len) / 2 do
				write " ";
			endfor;
			write message;
			for i=0 to (56-len) / 2 do
				write " ";
			endfor;
			write "|\n";
			return true;
		end printmessage


	func refreshscreen():bool
	begin refreshscreen
		dummy = cls();
		dummy = printheader(game.player);
		dummy = printboard();
		dummy = printmenu();
		return true;
	end refreshscreen

	func saferead():bool
	begin saferead
		dummy = savegame();
		read userchoice;
		return true;
	end saferead

	func savegame():bool
	begin savegame
		write ["game.ff4"] game;
		return true;
	end savegame

	begin FF4
		read ["game.ff4"] game;
		--write game;
		if game.properclose == true then
			write "SIIII";
			game.properclose = false;
			for j=0 to 6 do 
				for i=0 to 5 do 
					--write i;
					game.board[i][j]=0;
				endfor;
			endfor;
			game.player = 1;
		endif;
		--write game;
		dummy = refreshscreen();
		while true do
			write game.properclose;
			dummy = refreshscreen();
			dummy = printmessage("select the column", 17);
			dummy = saferead();
			ok=true;
			okfull=true;
			first = true;
			while(not ok or not okfull or first) do
				first=false;
				if (not ok) then
					dummy = refreshscreen();
					--write "Please, insert a number between 1 and 7\n";
					dummy = printmessage("Please, insert a number between 1 and  7", 40);
					dummy = saferead();
				endif;
				if(userchoice>=1 and userchoice<=7) then
					ok = true;
				else
					ok = false;
				endif;
				if(ok) then
					if(insertoken(userchoice, game.player)) then
						okfull = true;
						if checkwin() then
							dummy = refreshscreen();
							dummy = printwinner(game.player);
							return true;
						endif;
					else
						okfull = false;
						dummy = refreshscreen();
						dummy = printmessage("This column is full, try again",30);
						dummy = saferead();
					endif;
				endif;
			endwhile;

			if game.player == 1 then
				game.player = 2;
			else
				game.player = 1;
			endif;
		endwhile;
		return true;
	end FF4

