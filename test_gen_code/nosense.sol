-- no sense
func f2(): int 
var a,b,c:int;
	d,e,f:real;
	ff:bool;
	i:int;
	stru:vector [2] of struct(a:struct(a:int;b:string;);b:vector[2] of real;);
	begin f2
		stru = vector(struct(struct(1,"ciao"),vector(3.0,4.0)),struct(struct(9,"cieo"),vector(6.0,5.0)));
		write stru[0].b[0];
--		ff = false or rd bool;
--		for i=0 to 5 do
--			for i=0 to 2 do
--				write "ciao";
--			endfor;
--		endfor; 
--		if(rd bool) then
--			write toreal(toint(rd real));
--		else
--			if(rd bool) then
--				if(rd bool) then
--					if rd bool then
--						write if(rd bool) then "vero" else "falso" endif;
--					endif;
--				endif;
--			endif;	
--		endif;
		write "\n";
		return 0;
	end f2
