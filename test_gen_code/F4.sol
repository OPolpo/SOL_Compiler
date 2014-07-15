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
				if board[cols-1][i] ==0 then
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
			var i,j, count:int;
			begin checkrows
				count = 0;
				write "cicle\n";
				for i=0 to 5 do
					for j=1 to 6 do
						--if( board[j][i] == board[j-1][i]) then
					--	write i;
					--	write "---";
					--	write j;
					--	write "\n";
						dummy = j==1 ;
						if dummy then
							write board;
							
						--else
						--	write "false";
						--	write board;
						endif;
					----	--if( board[j][i] == 1) then
--
					--	--dummy = board[j][i] == board[j-1][i];
--
					--	--	--count = count + 1;
					--	--	--if count == 4 then
					--	--	--	return true;
					--	--	--endif;
					--	--endif;
					--	write i;
					--	write "---";
					--	write j;
					--	write "\n";
						dummy=false;
					endfor;
				endfor;
				return false;
			end checkrows
		func checkcols():bool
			begin checkcols
				return false;
			end checkcols
		func checkdiag():bool
			begin checkdiag
				return false;
			end checkdiag

		begin checkwin
			res = checkrows() and checkcols() and checkdiag();
			return false;
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
		while true do
			write "select the column\n";
			read userchoice;
			if (userchoice>0 and userchoice<8) then
				if(insertoken(userchoice, player)) then
					if checkwin() then
						dummy = printwinner(player);
						return true;
					endif;
					dummy = cls();
					dummy = printheader(player);
					dummy = printboard();
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
		dummy = printboard();
		return true;
	end F4

