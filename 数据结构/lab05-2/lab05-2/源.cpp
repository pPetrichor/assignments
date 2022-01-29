#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
using namespace std;
struct node
{
	int val;
	node* leftchild;
	node* rightchild;
};

int caculate_d(node *t,int val,int d)
{
	int i = -1;
	if(t)
	{
		if (t->val == val) return d;
		int a = caculate_d(t->leftchild, val, d + 1), b = caculate_d(t->rightchild, val, d + 1);
		if (a != -1) i = a;
		else i = b;
	}
	return i;
}
int main()
{
	string s, temp;
	getline(cin, s);
	stringstream ss(s);
	node *tree = new node[100];
	int i = 0;
	while(ss>>temp)
	{
		if (temp == "#") {
			tree[i].val = -1; tree[i].leftchild = NULL; tree[i].rightchild = NULL;
		}
		else
		{
			tree[i].val = 0;
			for(int j=0;j<temp.length();++j)
			{
				tree[i].val *= 10;
				tree[i].val += temp[j] - '0';
			}
		}
		++i;
	}
	for(int k=0;k<i;++k)
	{
		if (2 * (k + 1) <= i) tree[k].leftchild = &tree[2 * (k + 1) - 1];
		else tree[k].leftchild = NULL;
		if (2 * (k + 1) + 1<= i) tree[k].rightchild = &tree[2 * (k + 1)];
		else tree[k].rightchild = NULL;
		if (tree[k].leftchild&&tree[k].leftchild->val == -1) tree[k].leftchild = NULL;
		if (tree[k].rightchild&&tree[k].rightchild->val == -1) tree[k].rightchild = NULL;
	}
	int nju = -1, find = -1, nju_f = -1, find_f = -1;
	cin >> nju >> find;
	for(int t=0;t<100;++t)
	{
		if ((tree[t].leftchild&&tree[t].leftchild->val == nju) || (tree[t].rightchild&&tree[t].rightchild->val == nju)) nju_f = tree[t].val;
		if ((tree[t].leftchild&&tree[t].leftchild->val == find) || (tree[t].rightchild&&tree[t].rightchild->val == find)) find_f = tree[t].val;
		if (nju_f != -1 && find_f != -1) break;
	}
	int nju_d = caculate_d(&tree[0], nju, 0), find_d = caculate_d(&tree[0], find, 0);
	if (nju_d == find_d && nju_f != find_f) cout << "true";
	else cout << "false";
	system("pause");
	return 0;
}