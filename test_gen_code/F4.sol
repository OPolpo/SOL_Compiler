--test FOX FORCE 4 XD
func FF4():bool
	type gametype: struct(board: vector [7] of vector [6] of int; gameend: bool; player: int;);
	var i,j:int;
		userchoice:int;
		dummy:bool;
		ok, colsselection, okfull, first:bool;
		game:gametype;

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

		for j=0 to 5 do 
			write "   ||-------------------------------------------------------||\n";
			write "   ||";
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
			write "   ||";

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
			write "   ||";

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
	
		endfor;
		write "   -----------------------------------------------------------\n";
		write "   |    1   |   2   |   3   |   4   |   5   |   6   |   7    |\n";
	
		return true;
	end printboard
	
	func cls():bool
		begin cls
			write("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			return true;
		end cls

	func printheader(player:int;):bool
		func gamename():bool
			begin gamename

				write "     ______                ______                          __ __ \n";
				write "    / ____/___  _  __     / ____/___  _____________       / // / \n";
				write "   / /_  / __ \| |/_/    / /_  / __ \/ ___/ ___/ _ \     / // /_ \n";
				write "  / __/ / /_/ />  <     / __/ / /_/ / /  / /__/  __/    /__  __/ \n";
				write " /_/    \____/_/|_|    /_/    \____/_/   \___/\___/       /_/    \n\n";
				return true;
			end gamename

		begin printheader
			dummy = gamename();
			write "   -----------------------------------------------------------\n";
			write "   ------------------      ";
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

	func checkfull():bool
		var i:int;
		begin checkfull
			for i=0 to 6 do
				if(game.board[i][5] == 0) then
					return false;
				endif;
			endfor;
			return true;
		end checkfull

	func printwinner(player:int;):bool
		begin printwinner
			if (game.player == 1) then
				--write "bu1";
				dummy = printmessage("PLAYER ONE WINS !!!!", 20);
			else
				--write"bu2";
				dummy = printmessage("PLAYER TWO WINS !!!!", 20);
			endif;
			return true;
		end printwinner

	func printmenu():bool
		begin printmenu
			write "   |---------------------------------------------------------|\n";
			write "   |        0      *  Exit                                   |\n";
			write "   |        1/7    *  Insert Token                           |\n";
			write "   |        9      *  Reset Game                             |\n";
			write "   |---------------------------------------------------------|\n";
			return true;
		end printmenu

	func printmessage(message:string; len:int;):bool
		var i:int;
		begin printmessage
			write "   |";
			for i=0 to (55-len) / 2 do
				write " ";
			endfor;
			write message;
			for i=0 to (56-len) / 2 do
				write " ";
			endfor;
			write "|\n";
			write "   -----------------------------------------------------------\n";
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

	func resetgame():bool
	begin resetgame
		game.gameend = false;
		for j=0 to 6 do 
			for i=0 to 5 do 
				game.board[j][i]=0;
			endfor;
		endfor;
		game.player = 1;
		first = true;
		return true;
	end resetgame

	begin FF4
		read ["game.ff4"] game;

		if game.gameend == true then
			game.gameend = false;
			dummy = resetgame();
		endif;

		dummy = refreshscreen();
		while true do
			dummy = refreshscreen();
			ok=true;
			okfull=true;
			colsselection=false;
			first = true;
			if checkwin() then
				dummy = refreshscreen();
				game.gameend = true;
				dummy = printwinner(game.player);
			elsif checkfull() then
				dummy = refreshscreen();
				game.gameend = true;
				dummy = printmessage("No Winner...",12);
			else
				dummy = printmessage("Select the column", 17);
			endif;
			while(not ok or not okfull or first) do
				dummy = saferead();
				write userchoice;
				dummy = refreshscreen();
				dummy = printmessage("Select the column", 17);
				first=false;

				if(userchoice>=1 and userchoice<=7 and not game.gameend) then
					colsselection=true;
					ok = true;
				elsif (userchoice==9) then
					dummy = resetgame();
					dummy = refreshscreen();
					dummy = printmessage("Select the column", 17);
					ok=true;
					colsselection=false;
				elsif (userchoice==0) then
					return true;
					colsselection=false;
				else
					ok = false;
					colsselection=false;
				endif;

				if (not colsselection and not game.gameend) then
					dummy = refreshscreen();
					dummy = printmessage("Please, insert a number between 1 and  7", 40);
				endif;

				if(not game.gameend and colsselection) then
					if(insertoken(userchoice, game.player)) then
						okfull = true;
					else
						okfull = false;
						dummy = refreshscreen();
						dummy = printmessage("This column is full, try again",30);
					endif;
				endif;
				if checkwin() then
					dummy = refreshscreen();
					game.gameend = true;
					dummy = printwinner(game.player);
				endif;
				if checkfull() then
					dummy = refreshscreen();
					dummy = printmessage("No Winner...",12);
					game.gameend = true;
				endif;
			endwhile;
			if (not game.gameend ) then
				if game.player == 1 then
					game.player = 2;
				else
					game.player = 1;
				endif;
			endif;
		endwhile;
		return true;
	end FF4

