--test :P
func tester2():int
var a, b, c: int;
	res: int;
	t, r: struct(x: int; y: string;);
	v: vector [100] of int; 
	i: int;
	m, n: int;

const MM: int=20;

func sum(v: vector [10] of int;):int 
var n, res: int;
begin sum
	res = 0;	
	foreach n in v do
		res = res + n; 
	endforeach; 
	return res;
end sum
	
func fact(n: int;): int 
begin fact
	if n <= 0 then 
		return 1;
	else
		return n * fact(n-1);
	endif; 
end fact
func exread(): int
type Vect: vector [100] of int;
var name: string; v1, v2: Vect;
begin exread
	v1 = rd Vect;
	v2 = reverse(rd [name] Vect);
end exread

func exwrite():int
var i, j: int;
	name: string;
	v: vector [100] of int;
begin exwrite
	write f(v);
	read name;
	write [name] reverse(v);
end exwrite

func exwrite2():int
var i: int;
	name: string;
	v: vector [100] of int;
	v1, v2: vector [20] of int;
begin exwrite2
	v1 = wr f(v);
	v2 = f(wr [name] v);
end exwrite2

begin tester2

	m = sum(v);
	a = if b>c then b+c elsif b==c+1 then b-c else a+1 endif;
	a = if b>c then b+c else a+1 endif;
	a = if b>c then b+c elsif b==c+1 then b-c elsif b==d+1 then b-d elsif b==e+1 then b-e else a+1 endif;
	if a==b then
		t = r;
	elsif a>b then
		t = struct(2, "alpha");
		a = b + c;
	else
		a = b-c;
	endif;

	res = 0;
	while a >= b do
		res = res + 1;
		a = a - b; 
	endwhile;

	for i=1 to 100 do 
		k = f(v[i]);
		a = p(k, i); 
	endfor;

	foreach n in v do 
		m = compute(n);
	endforeach;

	read v;
	read ["v.dat"] v;


end tester2