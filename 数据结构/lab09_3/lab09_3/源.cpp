#include<iostream>
#include<algorithm>
using namespace std;
int *father;
int *if_father;
struct edge
{
	int start;
	int end;
	int cost;
};
bool cmp(edge a,edge b)
{
	return a.cost < b.cost;
}
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
int if_find(int x)
{
	if (x == if_father[x])
		return if_father[x];
	else
	{
		if_father[x] = if_find(if_father[x]);
	}
	return if_father[x];
}
int main()
{
	int n, m;
	cin >> n >> m;
	int all = m + n;//可能的路条数
	int if_n = n + 1;//港口设置为虚点
	father = new int[n+1];//并查集
	if_father = new int[if_n + 1];
	for(int i=1;i<=n;++i)
	{
		father[i] = i;
	}
	for (int i = 1; i <= if_n; ++i) if_father[i] = i;
	edge *if_all_edge = new edge[all];
	edge *all_edge = new edge[m];
	for(int i=0;i<m;++i)
	{
		cin >> all_edge[i].start >> all_edge[i].end >> all_edge[i].cost;
		if_all_edge[i].start = all_edge[i].start;
		if_all_edge[i].end = all_edge[i].end;
		if_all_edge[i].cost = all_edge[i].cost;
	}
	int temp = 0;
	for(int i=0;i<n;++i)
	{
		cin >> temp;
		if(temp==-1)
		{
			if_all_edge[i + m].start = -1;
			if_all_edge[i + m].end = -1;
			if_all_edge[i + m].cost = 100000;
		}
		else
		{
			if_all_edge[i + m].start = i+1;
			if_all_edge[i + m].end = if_n;//港口
			if_all_edge[i + m].cost = temp;
		}
	}
	int count = 1; int pos = 0;
	sort(all_edge, all_edge + m, cmp);
	int sum1 = 0;
	while (count < n && pos < m)
	{
		int u = find(all_edge[pos].start);
		int v = find(all_edge[pos].end);
		if (u != v||all_edge[pos].cost<0)
		{
			sum1 += all_edge[pos].cost;
			if (u != v)
			{
				count++;
				father[v] = u;
			}
		}
		pos++;
		if(count == n)
		{
			for(int i = pos;pos<m;++pos)
			{
				if(all_edge[pos].cost < 0) sum1 += all_edge[pos].cost;
			}
		}
	}
	if (count < n) sum1 = 1000000;
	count = 1; pos = 0;
	sort(if_all_edge, if_all_edge + all, cmp);
	int sum2 = 0;
	while (count < if_n && pos < all)
	{
		int u = if_find(if_all_edge[pos].start);
		int v = if_find(if_all_edge[pos].end);
		if (u != v||if_all_edge[pos].cost<0)
		{
			sum2 += if_all_edge[pos].cost;
			if(u!=v)
			{
				count++;
				if_father[v] = u;
			}
			
		}
		pos++;
		if (count == if_n)
		{
			for (int i = pos; pos < all; ++pos)
			{
				if (if_all_edge[pos].cost < 0) sum2 += if_all_edge[pos].cost;
			}
		}
	}
	int sum = sum1;
	if (sum > sum2) sum = sum2;
	cout << sum;
	system("pause");
	return 0;
}