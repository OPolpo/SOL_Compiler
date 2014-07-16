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
					write userchoice;
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

