--test :P
func tester():int
var a:struct(nome,cognome:string; giorno:struct(g:int;m:int;a:int;); eta:int;);
const dd:struct(g:int;m:int;a:int;)=struct(1,2,3);
begin tester
	--a = struct("io","pippo", dd,10);
	write dd;
	a.giorno = dd;
	return 1000000;
end tester