#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
using namespace std;
int main()
{
	vector<char> s;
	char temp;
	short initial = 1;
	while (scanf("%c", &temp) != EOF)
	{
		if (temp == 'e') break;
		if(temp != ' ')
			s.push_back(temp);
	}
	int l = s.size();
	vector<short*> dp;
	for (int i = 0; i < l; ++i)
	{
		short * current = new short[i + 1];
		current[i] = 1;
		dp.push_back(current);
	}
		
	for (int i = 1; i < l; ++i)
	{
		if (s[i] == s[i - 1]) dp[i][i - 1] = 2;
		else dp[i][i - 1] = 1;
	}
	for(int j=2;j<l;++j)
		for (int i = 0; i < l - j; ++i)
		{
			int c = 0;
			if (s[i] == s[i + j])
			{
				c = 2 + dp[i + j - 1][i + 1];
			}
			else
			{
				c = dp[i + j - 1][i];
				if (c < dp[i + j][i + 1]) c = dp[i + j][i + 1];
			}
			dp[i + j][i] = c;
		}
	printf("I have read the rules about plagiarism punishment\n");
	printf("%d",dp[l - 1][0]);
	system("pause");
	return 0;
}