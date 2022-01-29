#include<iostream>
#include<algorithm>
using namespace std;
struct node 
{
	int x, y, p;
};

bool cmp(node a, node b)
{
	return a.p > b.p;
}
class solve
{
private:
	int *father;
	int *bad;
	node *num;
	int n;
	int m;
public:
	solve()
	{
		cin >> n >> m;
		father = new int[n+1];
		bad = new int[n+1];
		num = new node[m];
		for (int i = 1; i <= n; i++)
		{
			father[i] = i;
			bad[i] = 0;
		}
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
	void run()
	{
		for (int i = 0; i < m; i++)
		{
			cin >> num[i].x >> num[i].y >> num[i].p;
		}
		sort(num, num + m, cmp);
		for (int i = 0; i < m; i++)
		{
			int tmp1 = find(num[i].x);
			int tmp2 = find(num[i].y);
			if (tmp1 == tmp2) { cout << num[i].p; return; }
			if (bad[num[i].x] == 0)
				bad[num[i].x] = num[i].y;
			if (bad[num[i].y] == 0)
				bad[num[i].y] = num[i].x;
			int tx = find(num[i].x);
			int ty = find(bad[num[i].y]);
			father[ty] = tx;
			tx = find(bad[num[i].x]);
			ty = find(num[i].y);
			father[tx] = ty;
		}
		cout << 0;
	}
};

int main()
{
	solve s;
	s.run();
	system("pause");
	return 0;
}
