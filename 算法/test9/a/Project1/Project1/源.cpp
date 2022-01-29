#include<iostream>
using namespace std;
int maxnum = 0;
struct Treenode
{
	int val;
	Treenode *leftchild;
	Treenode *rightchild;
};
int maxdepth(Treenode *root)
{
	if (root == NULL) return 0;
	int l = maxdepth(root->leftchild);
	int r = maxdepth(root->rightchild); int depth;
	if (l > r) depth = l + 1;
	else depth = r + 1;
	if (maxnum < (l + r + 1)) maxnum = l + r + 1;
	return depth;
}
Treenode * buildTree(int* pre, int* in, int pre_start, int pre_end, int in_start, int in_end)
{
	if (pre_start > pre_end)
		return NULL;
	Treenode *node = new Treenode;
	node->val = pre[pre_start];
	int t = 0;
	for (int i = in_start; i < in_end + 1; i++)
	{
		if (pre[pre_start] == in[i])
		{
			t = i;
			break;
		}
	}
	node->leftchild = buildTree(pre, in, pre_start + 1, pre_start + t - in_start, in_start, t - 1);
	node->rightchild = buildTree(pre, in, pre_start + t - in_start + 1, pre_end, t + 1, in_end);
	return node;
}
int main()
{
	cout << "I have read the rules about plagiarism punishment" << endl;
	int num;
	cin >> num;
	int *pre = new int[num];
	int *in = new int[num];
	for(int i=0;i<num;++i)
	{
		cin >> pre[i];
	}
	for (int i = 0; i < num; ++i)
	{
		cin >> in[i];
	}
	
	Treenode* tree = buildTree(pre, in, 0, num - 1, 0, num - 1);
	int dep = maxdepth(&tree[0]);
	cout << dep - 1 << endl << maxnum - 1;
	system("pause");
	return 0;
}