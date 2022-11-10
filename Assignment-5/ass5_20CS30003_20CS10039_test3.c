int main()
{
    // examples of if else conditional statements
    int a, b, c, d;
    if (a > b)
    {
        c = a + b;
        if (c > d)
        {
            d = c + a + b * c;
        }
        else
        {
            d = c + b * (a + b);
        }
    }
    else
    {
        d = a - b;
        if (d > c)
        {
            c = d + a + b * c;
        }
        else if (d > a)
        {
            c = d + b * (a + b);
        }
        else
        {
            c = d + a * (b + c);
        }
    }
    return 0;
}
