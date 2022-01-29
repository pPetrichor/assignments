#include<iostream>
using namespace std;
int main()
{
	int sum[1000];
	for(int i=0;i<1000;++i)
	{
		sum[i] = 0;
	}
	int x;
	while (cin >> x)
	{
		sum[x]++;
	}
	int all = 0;
	for(int i=0;i<1000;++i)
	{
		if(sum[i]!=0)
		{
			if (sum[i] <= i + 1) all += (i + 1);
			else
			{
				int num1 = sum[i] / (i + 1);
				int num2 = sum[i] % (i + 1);
				if (num2 == 0) all += sum[i];
				else all += ((num1+1)*(i+1));
			}
		}
	}
	cout << all;
	system("pause");
	return 0;
}