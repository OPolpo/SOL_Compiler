func tester():int
var i,j,k: int;
begin tester

if i==j then
	j=j+3;
elsif i>j then
   	i=i-2;
   	j=i+k;
else
  	i=j*k;
endif;

return j;
end tester