func tester():int
var a, b, ris: int;
begin tester
	a=3;
	b=1;
	ris = 0;
	while a>=b do
		ris = ris+1;
		a=a-b;
	endwhile;
return ris;
end tester