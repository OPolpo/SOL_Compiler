func alpha(i,j,k: int;):int
	var
		n, m: int;
	const
		msg: string = "Hello!";
begin alpha
	write msg;
	n = i + k;
	m = n - k;
	return 1;
	j = n * m;
	k = k / j;
	return 2;
	return n +m -k;
end alpha