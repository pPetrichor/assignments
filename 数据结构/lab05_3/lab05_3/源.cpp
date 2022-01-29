#include<iostream>
using namespace std;
struct node
{
	int val;
	node* leftchild;
	node* rightchild;
};
class Queue
{
	int rear, front;
	node ele[30];
public:
	Queue() { front = rear = 0; }
	void enqueue(node &p)
	{
		ele[rear] = p;
		rear++;
	}
	void dequeue(node &p)
	{
		p = ele[front];
		front++;
	}
	bool ifempty()
	{
		return front == rear;
	}
};
int main()
{
	int num;
	cin >> num;
	node *tree = new node[num];
	int l, r;
	for(int i=0;i<num;++i)
	{
		cin >> l >> r;
		if (l != -1) { tree[i].leftchild = &tree[l - 1]; tree[i].leftchild->val = l; }
		else tree[i].leftchild = NULL;
		if (r != -1) { tree[i].rightchild = &tree[r - 1]; tree[i].rightchild->val = r; }
		else tree[i].rightchild = NULL;
	}
	tree[0].val = 1;
	Queue q;
	q.enqueue(tree[0]);
	node temp;
	node current_last, next_last;
	current_last = tree[0];
	while(!q.ifempty())
	{
		q.dequeue(temp);
		cout << temp.val << " ";//输出第一个
		node t = temp;
		while (temp.val != current_last.val)
		{
			if (temp.leftchild) { next_last.val = temp.leftchild->val; q.enqueue(*temp.leftchild); }
			if (temp.rightchild) { next_last.val = temp.rightchild->val; q.enqueue(*temp.rightchild); }
			q.dequeue(temp);
		}
		if (t.val != current_last.val) cout << temp.val << " ";//此层处理完毕
		if (temp.leftchild) { next_last.val = temp.leftchild->val; q.enqueue(*temp.leftchild); }
		if (temp.rightchild) { next_last.val = temp.rightchild->val; q.enqueue(*temp.rightchild); }
		current_last.val = next_last.val;
	}
	system("pause");
	return 0;
}