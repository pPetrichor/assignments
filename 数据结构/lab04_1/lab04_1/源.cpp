#include <iostream>
using namespace std;
struct Treenode
{
	int data;
	Treenode *leftchild;
	Treenode *rightchild;
};
class Tree
{
private:
	int size;
	Treenode *p;
public:
	Tree() { cin >> size; p = new Treenode[size]; }
	void creatTree();
	int sum(Treenode *solve);
	void set(Treenode *solve);
	void output(Treenode *solve);
	Treenode* head() { return &p[0]; }
};
void Tree::creatTree()
{
	for(int i=0;i<size;++i)
	{
		cin >> p[i].data;
		if (2 * (i + 1) <= size) p[i].leftchild = &p[2 * (i + 1) - 1];
		else p[i].leftchild = NULL;
		if (2 * (i + 1) + 1 <= size) p[i].rightchild = &p[2 * (i + 1)];
		else p[i].rightchild = NULL;
	}
}
int Tree::sum(Treenode *solve)
{
	if (solve == NULL) return 0;
	else return solve->data + sum(solve->leftchild) + sum(solve->rightchild);
}
void Tree::set(Treenode *solve)
{
	if (solve == NULL) return;
	if(sum(solve->leftchild)>sum(solve->rightchild))
	{
		Treenode *temp = solve->leftchild;
		solve->leftchild = solve->rightchild;
		solve->rightchild = temp;
	}
	set(solve->leftchild);
	set(solve->rightchild);
}
void Tree::output(Treenode *solve)
{
	if (solve == NULL) return;
	output(solve->leftchild);
	cout << solve->data << " ";
	output(solve->rightchild);
}
int main()
{
	Tree BinaryTree;
	BinaryTree.creatTree();
	BinaryTree.set(BinaryTree.head());
	BinaryTree.output(BinaryTree.head());
	system("pause");
	return 0;
}