int LongestCommonSubsequence(char *X, char *Y, int m, int n)
{
	int L[m + 1][n + 1];
	int i, j;
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
				L[i][j] = 0;
			else if (X[i - 1] == Y[j - 1])
				L[i][j] = L[i - 1][j - 1] + 1;
			else
				L[i][j] = max(L[i - 1][j], L[i][j - 1]);
		}
	}
	return L[m][n];
}

int strlen(char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

int kmp(char *str, char *pat)
{
	int m = strlen(pat);
	int n = strlen(str);
	int lps[m];
	int i = 1;
	int len = 0;
	lps[0] = 0;
	while (i < m)
	{
		if (pat[i] == pat[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else
		{
			if (len != 0)
			{
				len = lps[len - 1];
			}
			else
			{
				lps[i] = 0;
				i++;
			}
		}
	}
	int j = 0;
	i = 0;
	while (i < n)
	{
		if (pat[j] == str[i])
		{
			j++;
			i++;
		}
		if (j == m)
		{
			return i - j;
		}
		else if (i < n && pat[j] != str[i])
		{
			if (j != 0)
			{
				j = lps[j - 1];
			}
			else
			{
				i = i + 1;
			}
		}
	}
	return -1;
}

int main()
{
	char *a = "hello";
	char *b = "helloWorld";
	int m = strlen(a);
	int n = strlen(b);
	int lcs = LongestCommonSubsequence(a, b, m, n);
	int kmp1 = kmp(b, a);
	return 0;
}
