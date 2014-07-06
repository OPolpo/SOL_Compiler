func sqrt(val:real;):real
var low, high, mid, oldmid, midsqr: real;
    step:int;

    func fabs(a:real;):real
    begin fabs
        return if a>0.0 then a else -a endif;
    end fabs

begin sqrt
    step = 0;
    low = 0.0;
    high = val;
    mid = val;
    oldmid = -1.0;
    --write vector("Step", "Number", "Low", "High", "Mid", "Square", "Result");
    while (fabs(oldmid - mid) >= 0.000001) do
        oldmid = mid;
        mid = (high + low) / 2.0;
        midsqr = mid * mid;
        step = step+1;
        --write vector(toreal(step),val,low,high,mid,midsqr);
        if (mid * mid > val) then
            high = mid;
            --write ("- too high\n");
        else
            low = mid;
            --write ("- too low\n");
        endif;
    endwhile;

    return mid;
end sqrt
