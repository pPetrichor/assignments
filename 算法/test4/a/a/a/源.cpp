#include<iostream>
using namespace std;
int myfind(int a[],int b[],int a_start,int a_end,int b_start,int b_end)
{
	if(a_start == a_end)
	{
		if (a[a_start] < b[b_start]) return a[a_start];
		else return b[b_start];
	}
	int a_mid = a[(a_start+a_end)/ 2];
	int b_mid = b[(b_start+b_end) / 2];
	if (a_mid == b_mid) return a_mid;
	if(a_mid < b_mid)
	{
		int a_s = (a_start + a_end) / 2;
		int b_e = (b_start + b_end) / 2;
		if ((a_end - a_start) % 2 != 0)
			a_s++;
		return myfind(a, b, a_s, a_end, b_start, b_e);
	}
	else
	{
		int a_e = (a_start + a_end) / 2;
		int b_s = (b_start + b_end) / 2;
		if ((a_end - a_start) % 2 != 0)
			b_s++;
		return myfind(a, b, a_start, a_e, b_s, b_end);
	}
}
int main()
{
	int n;
	scanf("%d", &n);
	int *a = new int[n];
	int *b = new int[n];
	for (int i = 0; i < n; ++i)
		scanf("%d", &a[i]);
	for (int i = 0; i < n; ++i)
		scanf("%d", &b[i]);
	int ret = myfind(a, b, 0, n - 1, 0, n - 1);
	printf("%d", ret);
	system("pause");
	return 0;
}