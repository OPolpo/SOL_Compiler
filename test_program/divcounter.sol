func divino():int
	var count:int;
		word, searched:string;
		divina: vector[4999] of
		 string;

begin divino
	read ["div.txt"] divina;
	read searched;
	foreach word in divina do
		if searched==word then
			count = count +1;
		endif;
	endforeach;
	return count;
end divino
