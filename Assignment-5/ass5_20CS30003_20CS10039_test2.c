int main()
{
    // examples of identifiers and pointers
    int a, b, c, d;
    int *p, *q, *r, *s;
    p = &a;
    q = &b;
    r = &c;
    float **x, *y, z;
    x = &p;
    y = &z;
    z = 1.0;
    **x = *p + *q;
    a++;
    *q = *r + 1;
    b--;
    ++*r;
    --c;
    a = ~a;
    b = !b;
    c = -c;
    d = +d;
    return 0;
}
