#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int *father = NULL;
struct edge
{
	int start;
	int end;
	int weight;
};
bool cmp(edge a,edge b)
{
	if (a.weight < b.weight) return true;
	else return false;
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
int main()
{
	int n;
	cin >> n;
	father = new int[n];
	for (int i = 0; i < n; ++i)
	{
		father[i] = i;
	}
	vector<edge> edges;
	int s, e, w;
	while (cin >> s)
	{
		if (s == -1) break;
		cin >> e >> w;
		edge temp;
		temp.start = s; temp.end = e; temp.weight = w;
		edges.push_back(temp);
	}
	sort(edges.begin(), edges.end(), cmp);
	int nums = 0, cost = 0;
	int i = 0;
	while (nums < n - 1)
	{
		edge current_edge = edges[i];
		if(find(current_edge.start)!=find(current_edge.end))
		{
			nums++;
			cost += current_edge.weight;
			father[find(current_edge.end)] = find(current_edge.start);
		}
		i++;
	}
	cout << cost;
	system("pause");
	return 0;
}