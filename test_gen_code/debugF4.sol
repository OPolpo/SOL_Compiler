--test FOX FORCE 4 XD
func FF4():int
	type gametype: struct(board: vector [7] of vector [6] of int; gameend: bool; player: int;);
	var i:int;
		dummy:bool;
		game:gametype;

		func printwinner(player:int;):int
		begin printwinner
			if (true) then
				i = printmessage("PLAYER ONE WINS !!!!", 20);
			else
				i = printmessage("PLAYER TWO WINS !!!!", 20);
			endif;
			return 300;
		end printwinner
	
	func printmessage(message:string; len:int;):int
		var i:int;
		begin printmessage
			write "   |";
			--for i=0 to (55-len) / 2 do
			--	write " ";
			--endfor;
			write message;
			for i=0 to (21-len) / 2 do
				write " ";
			endfor;
			write "|\n";
			write "   -----------------------------------------------------------\n";
			return 200;
		end printmessage





	begin FF4
		i=printwinner(1);
		write "www";
		return 100;
	end FF4

