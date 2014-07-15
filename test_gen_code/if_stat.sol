func tester(i:int;):int
var j: int;
begin tester
if i==0 then
	j=3;
elsif i>5 then
	j=i-2;
elsif i>4 then
	j=i-1;
else
	j=i*3;
endif;
return j;
end tester