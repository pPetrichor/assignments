#include<iostream>
using namespace std;
struct node
{
	int val;
	node* leftchild;
	node* rightchild;
};
void creat_tree(node *&root)
{
	root = new node; int v;
	if (cin>>v)
	{
		if (v != -1)
		{
			root->val = v;
			creat_tree(root->leftchild);
			creat_tree(root->rightchild);
		}
		else root = NULL;
	}
}
int x = 0;
void inorder(node *&root,int t[])
{
	if (root != NULL)
	{
		inorder(root->leftchild, t);
		t[x] = root->val; x++;
		inorder(root->rightchild, t);
	}
}
int main()
{
	int n;
	cin >> n;
	int *t = new int[n];
	node *root;
	creat_tree(root);
	inorder(root, t);
	int first = -1, second = -1;
	for(int i =0;i<n-1;++i)
	{
		if (t[i] > t[i + 1]&&first==-1)  first = t[i];
		if (t[i] > t[i + 1])  second = t[i+1];
	}
	cout << second << " " << first;
	system("pause");
	return 0;
}