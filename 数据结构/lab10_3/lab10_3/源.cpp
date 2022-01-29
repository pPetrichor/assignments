#include<iostream>
using namespace std;
struct mylink
{
	int x;
	mylink *next;
	mylink()
	{
		x = 0;
		next = NULL;
	}
	mylink(int a, mylink* b)
	{
		x = a;
		next = b;
	}
};
struct mynode
{
	int val;//¼ÇÂ¼¶È
	mylink *next;
};
int main()
{
	int n, m;
	cin >> n >> m;
	char *str = new char[n];
	for (int i = 0; i < n; ++i) cin >> str[i];
	mynode *mylist = new mynode[n+1];
	for (int i = 1; i <= n; ++i)
	{
		mylist[i].val = 0;
		mylist[i].next = NULL;
	}
	for (int i = 0; i < m; ++i)
	{
		int first, second;
		scanf("%d", &first);
		scanf("%d", &second);
		mylist[second].val++;
		mylink* pa = new mylink(second, mylist[first].next);
		mylist[first].next = pa;
	}
	int *record = new int[n]; int k = 0;
	int res = 0;
	while(1)
	{
		int *to_be_minus = new int[n - res]; int pos = 0;
		for(int i=0;i<n-res;++i)
		{
			to_be_minus[i] = -1;
		}
		for(int i=1;i<=n;++i)
		{
			if(mylist[i].val==0)
			{
				to_be_minus[pos] = i; ++pos; record[k] = i; ++k; res++; mylist[i].val--;
			}
		}
		if (pos == 0) break;
		for(int i=0;i<pos;++i)
		{
			mylink *current = mylist[to_be_minus[i]].next;
			while (current != NULL)
			{
				mylist[current->x].val--;
				current = current->next;
			}
		}
	}
	if (res < n) { cout << -1; system("pause"); return 0; }
	int *dp = new int[n+1];
	for (int i = 1; i <= n; ++i) dp[i] = 0;
	int ret = 0;
	for (char c = 'a'; c <= 'z'; ++c)
	{
		for(int i=k-1;i>=0;--i)
		{
			int t = record[i];
			int current = (str[t-1] == c);
			mylink *now = mylist[t].next;
			dp[t] = current;
			while(now!=NULL)
			{
				if (dp[t] < dp[now->x] + current) dp[t] = dp[now->x] + current;
				now = now->next;
			}
			if (ret < dp[t]) ret = dp[t];
		}

	}
	cout << ret;
	system("pause");
	return 0;
}