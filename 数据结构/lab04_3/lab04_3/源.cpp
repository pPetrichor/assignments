#include<iostream>
using namespace std;
int maxnum = 0;//记录当前最长通路所含节点个数
struct Treenode
{
	Treenode *leftchild;
	Treenode *rightchild;
};
int maxdepth(Treenode *root)//返回树中某个节点的最大深度
{
	if (root == NULL) return 0;
	int l = maxdepth(root->leftchild);
	int r = maxdepth(root->rightchild); int depth;
	if (l > r) depth = l + 1;
	else depth = r + 1;
	if (maxnum < (l + r + 1)) maxnum = l + r + 1;
	return depth;
}
int main()
{
	int num;
	cin >> num;
	Treenode *p = new Treenode[num];
	int left, right;
	for(int i=0;i<num;++i)
	{
		cin >> left >> right;
		if (left != -1) p[i].leftchild = &p[left];
		else p[i].leftchild = NULL;
		if (right != -1) p[i].rightchild = &p[right];
		else p[i].rightchild = NULL;
	}//建立二叉树
	maxdepth(&p[0]);
	cout << maxnum+ 2 * (num - maxnum) - 1;
	system("pause");
	return 0;
}