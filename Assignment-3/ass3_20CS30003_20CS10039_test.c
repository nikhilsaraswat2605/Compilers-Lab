/*
 *   // Amit Kumar  - 20CS30003
 *   // Nikhil Saraswat - 20CS10039
 */

#define MOD 1000000007

inline int power(double *restrict p, volatile vol, ...)
{
    auto a = 1;
    const int ef = 5220;
    return a;
}

struct wow
{
    _Bool b;
    int n;
    volatile int d;
};

struct Complex
{
    _Complex c;
    double real;
    _Imaginary i;
};

float calculate(float a, float b)
{
    register int b;
    extern int c;
    return a + b;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

union union_test
{
    int a;
    float b;
};

enum enum_test
{
    a,
    b,
    c
};

typedef struct wow complex;
static test1 test2;

signed main(int argc, char const *argv[])
{
    int n1 = 4 * sizeof(int);
    int arr[n1];
    int n2 = sizeof(arr) / sizeof(int);
    for (int i = 0; i < n2; i++)
    {
        arr[i] = i;
    }
    for (int i = 0; i < n2; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    switch (argc)
    {
    case 1:
        printf("No arguments\n");
        break;
    case 2:
        printf("One argument\n");
        break;
    case 3:
        printf("Two arguments\n");
        break;
    default:
        printf("More than two arguments\n");
        break;
    }
    goto label;
    while (n1 > 0)
    {
        printf("%d ", n1);
        n1--;
    }
    short n3 = sizeof(short);
    do
    {
        printf("%d ", n3);
        n3++;
    } while (n3 < 0);
label:
    printf("\n");
    volatile int n4 = sizeof(volatile int);
    auto n5 = sizeof(auto int);
    register int n6 = sizeof(register int);
    for (int i = n1; i <= n1 && i >= 0; i++)
    {
        n1 /= 2;
        n1 *= 2;
        if (n1 % 2 == 0)
            printf("n = %d\n", n1);
        else
        {
            n1 = n1 + n1 + (-n1) / 1;
            n1 <<= 2;
            n1 >>= 2;
        }
        n1 += 1, n1 -= 1;
        int n7 = 100;
        n7 += n7;
        n7 -= n7;
        n7 *= n7;
        n7 /= n7;
        n1 = n1 ^ n1 ^ n1;
        n1 |= n1;
        n1 &= n1;
        n7 %= n7;
    }

    float f3 = 0.234e+2;
    float f4 = 12.234e01;
    double d2 = 234.e2;
    double d3 = .00;
    const float f1 = .234;
    float f2 = .234E-2;
    char s1[] = "A string // comments dont work here :P /* not even multiline ones */";
    do
    {
        printf("%f ", f1);
        f1 += f1;
    } while (f1 < f2);
    printf("%f %f %f %f\n", f3, f4, d2, d3);
    printf("%s\n", s1);
    char *s2 = "A string";
    char s2[] = "" /* Empty string check */;
    int n2 = n1 & s ? 1 : 2;
    n2--;
    unsigned long l = 12902311123;
    signed short s = -12;
    char c = '\?';
    double d1 = 234.;

    mystruct *ms1; // ok I understand this will give seg fault without allocating memory but I am not running it so :)
    switch (ms1->b)
    {
    case 1:
        ms2.n %= n2;
        ms2.n ^= ms2.n;
        break;
    default:
        break;
    }

    do
    {
        s >> 2;
        s = s | s;
        s << 2;
        if (s < 10 && s > 0)
            goto some_label;
        s = ~s;
    some_label:
        continue;
    } while (n2 > 0 && n2 < 10);

    return;
}