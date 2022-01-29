#include<iostream>
using namespace std;
int need = 0;
struct node
{
	int val;
	node* leftchild;
	node* rightchild;
};
void creat_tree(node *&root)
{
	root = new node; int v;
	if (cin >> v)
	{
		if (v != -1)
		{
			root->val = -2;
			creat_tree(root->leftchild);
			creat_tree(root->rightchild);
			if (root->leftchild == NULL && root->rightchild == NULL) root->val = 1;//叶节点
		}
		else root = NULL;
	}
}
void postorder(node *&root)
{
	if(root!=NULL)
	{
		postorder(root->leftchild);
		postorder(root->rightchild);
		if (root->leftchild)
		{
			if (root->leftchild->val == 1) root->val = 2;//守卫
			else if (root->leftchild->val == 2) root->val = 3;
			
		}
		if (root->rightchild) 
		{
			if (root->rightchild->val == 1) root->val = 2;
			else if (root->rightchild->val == 2 && root->val != 2) root->val = 3;
		}
		if (root->val == -2) root->val = 1;//去支后的叶节点
		if (root->val == 2) need++;
	}
}
int main()
{
	node *root;
	creat_tree(root);
	postorder(root);
	if (root->val ==  1) need++;
	cout << need;
	system("pause");
	return 0;
}