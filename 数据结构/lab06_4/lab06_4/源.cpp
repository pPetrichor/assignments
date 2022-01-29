#include<iostream>
using namespace std;
class solve
{
private:
	int n, m;
	int *father;
	int *r;//��¼�õ����丸�׵Ĺ�ϵ,0��ʾͬ�࣬1��ʾ���׿ˣ�2��ʾ���׿˵Ŀˣ�3��ʾ���׿˵Ŀ˵Ŀˣ�4��ʾ���׿˵Ŀ˵Ŀ˵Ŀˣ����˸��ڵ�
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
			r[x]=(r[x]+r[mid])%5;//�ó�x�뵱ǰfather[x]�Ĺ�ϵ  
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
			if(x_r!=y_r)//��һ��δ���벢�鼯��
			{
				father[y_r] = x_r;
				r[y_r] = (k + 5 - r[y] + r[x]) % 5;//��5��֤Ϊ����
			}
			else//��һ��
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