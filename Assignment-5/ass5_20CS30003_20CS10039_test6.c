int main()
{
    // example of while loop, do while loop and for loop
    int testcases = 10;
    while (testcases--)
    {
        int a = 10;
        do
        {
            a++;
        } while (a < 10);
    }
    // sum of all possible subarrays
    int n = 10;
    int a[n];
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            int sum = 0;
            for (int k = i; k <= j; k++)
            {
                sum += a[k];
            }
        }
    }
}
