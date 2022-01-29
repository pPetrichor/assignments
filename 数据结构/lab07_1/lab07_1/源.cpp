#include<iostream>
using namespace std;
int main()
{
	int a[10001];//合并1、2食堂
	int b[10001];//合并3、4食堂
	for(int i=0;i<=10000;++i)
	{
		a[i] = 0;
		b[i] = 0;
	}
	int n;
	cin >> n;
	int *first = new int[n];
	int *second = new int[n];
	for (int i = 0; i < n; ++i) cin >> first[i];
	for (int i = 0; i < n; ++i) cin >> second[i];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			int x = first[i] + second[j];
			if (x <= 10000) a[x]++;
		}
	for (int i = 0; i < n; ++i) cin >> first[i];
	for (int i = 0; i < n; ++i) cin >> second[i];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			int x = first[i] + second[j];
			if (x <= 10000) b[x]++;
		}
	int all = 0;
	for(int i=0;i<=10000;++i)
	{
		if (a[i]&&b[10000-i])
		{
			all += a[i] * b[10000 - i];
		}
	}
	cout << all;
	system("pause");
	return 0;
}