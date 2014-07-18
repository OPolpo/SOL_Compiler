func tester():bool
var i,j: int;
begin tester
i = rd ["b.txt"] int;
j = if i==rd ["a.txt"] int then
	i + 3
--elsif i>5 then
--  	i - 2
--elsif i>4 then
--   	i - 1
else
  	i * 3
endif;

write wr ["c.txt"] j;

return true;
end tester