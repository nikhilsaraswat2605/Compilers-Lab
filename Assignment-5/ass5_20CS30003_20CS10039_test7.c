int mod;

int primes[1000000];
void sieve(int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		primes[i] = 1;
	}
	primes[0] = 0;
	primes[1] = 0;
	for (i = 2; i * i < n; i++)
	{
		if (primes[i])
		{
			for (j = i * i; j < n; j += i)
			{
				primes[j] = 0;
			}
		}
	}
}

int fib(int n)
{
	if (n == 0)
	{
		return 0;
	}
	if (n == 1)
	{
		return 1;
	}
	return fib(n - 1) + fib(n - 2);
}

int main()
{
	int n = 1000000;
	sieve(n);
	int i;
	for (i = 0; i < n; i++)
	{
		if (primes[i])
		{
			printf("%d ", i);
		}
	}
	int sumfib = 0;
	for (i = 0; i < 10; i++)
	{
		sumfib += fib(i);
	}
	return 0;
}
