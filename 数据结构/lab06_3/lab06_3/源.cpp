#include<iostream>
using namespace std;
int myhash(int x)
{
	while(x>=10)
	{
		x = (x - (x % 10)) / 10;
	}
	return x;
}
int main()
{
	int num,t;
	while(cin>>num)
	{
	int ht[21];
	for (int i = 0; i < 21; ++i) ht[i] = -1;
	for(int i=0;i<num;++i)
	{
		cin >> t;
		int n = myhash(t);
		while(ht[n]!=-1)
		{
			n = (n + 1) % 21;
		}
		ht[n] = t;
	}
	int count = 0;
	for(int i=0;i<21;++i)
	{
		if(ht[i]!=-1)
		{
			if (count == num - 1) cout << ht[i];
			else { cout << ht[i] << ","; count++; }
		}
	}
	cout << endl;
	}
	
	system("pause");
	return 0;
}