#include<iostream>
using namespace std;
class solve
{
private:
	int n, m;
	int *father;
	int *r;//记录该点与其父亲的关系,0表示同类，1表示父亲克，2表示父亲克的克，3表示父亲克的克的克，4表示父亲克的克的克的克，即克父节点
public:
	solve() 
	{
		cin >> n >> m;
		father = new int[n];
		r = new int[n];
		for(int i=0;i<n;++i)
		{
			father[i] = i;
			r[i] = 0;
		}
	}
	int find(int x)
	{
		if (x == father[x])  return x;  
		else {
			int mid = father[x];   
			father[x] = find(mid);
			r[x]=(r[x]+r[mid])%5;//得出x与当前father[x]的关系  
			return father[x];   
		}
	}
	void run()
	{
		int all_false = 0;
		int k, x, y;
		while(m--)
		{
			cin >> k >> x >> y;
			if (k == 1 && x == y) { all_false++; continue; }
			int x_r = find(x);
			int y_r = find(y);
			if(x_r!=y_r)//有一个未加入并查集中
			{
				father[y_r] = x_r;
				r[y_r] = (k + 5 - r[y] + r[x]) % 5;//加5保证为正数
			}
			else//在一起
			{
				if (k == 0 && r[x] != r[y]) all_false++;
				if (k == 1 && ((5 + r[y] - r[x]) % 5) != 1) all_false++;
			}
		}
		cout << all_false;
	}
};
int main()
{
	solve s;
	s.run();
	system("pause");
	return 0;
}