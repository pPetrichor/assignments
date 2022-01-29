#include<iostream>
#include<algorithm>
using namespace std;
int *father;
int find(int x)
{
	if (x == father[x])
		return father[x];
	else
	{
		father[x] = find(father[x]);
	}
	return father[x];
}
bool cmp(int a,int b)
{
	return a > b;
}
int main()
{
	int n, m;
	cin >> n >> m;
	int all = n*m;
	father = new int[all+1];
	for (int i = 1; i <= all; i++)
		father[i] = i;
	int num = 0; cin >> num;
	for(int i=0;i<num;++i)
	{
		int a, b;
		cin >> a >> b;
		int u = find(a);
		int v = find(b);
		if (u != v) father[v] = u;
	}
	int *my = new int[all];
	for (int i = 0; i < all; ++i) my[i] = find(i + 1);
	int sum = 0;
	sort(my, my+all, cmp);
	int current = 0;
	for(int i=0;i<all;++i)
	{
		if (current != my[i]) { sum++; current = my[i]; }
	}
	cout << sum;
	system("pause");
	return 0;
}