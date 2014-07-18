func tester():bool
var i,j: int;
begin tester
read i;
j = if i==0 then
	i + 3
--elsif i>5 then
--  	i - 2
--elsif i>4 then
--   	i - 1
else
  	i * 3
endif;

write j;

return true;
end tester