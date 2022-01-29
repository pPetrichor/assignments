#include<iostream>
using namespace std;
struct node
{
	node *left;
	node *right;
	int weight;
};
int main()
{
	int a[50][50];
	int num;
	cin >> num;
	for(int i =0;i<num;++i)
		for(int j = 0;j<num;++j)
		{
			cin >> a[i][j];
		}
	node *p = new node[num*num];
	for(int i=0;i<num;++i)
		for(int j=0;j<num;++j)
		{
			p[i*num + j].weight = a[i][j];
			if(i<num-1&&j<num-1)
			{
				p[i*num + j].left = &p[(i+1)*num+j];
				p[(i + 1)*num + j].weight = a[i+1][j];
				p[i*num + j].right = &p[i*num + j + 1];
				p[i*num + j + 1].weight = a[i][j + 1];
			}
			
		}
	system("pause");
	return 0;
}
/*int num;
	cin >> num;
	int *times = new int [num];
	int *p = new int[num];
	for(int i=0;i<num;++i)
	{
		times[i] = 0; p[i] = 0;
	}
	int ele;
	for(int i=0;i<num;++i)
	{
		cin >> ele;
		int order = ele % num;
		while (p[order] != 0&&p[order]!=ele) order++;
		p[order] = ele;
		times[order]++;
	}
	int k; cin >> k;
	for(int i=0;i<k;++i)
	{
		int max = 0; int j = 0; int x = 0;
		while(j<num)
		{
			if (max < times[j]) { max = times[j]; x = j; }
			j++;
		}
		cout << p[x] << " ";
		times[x] = 0;
	}*/
/*struct node
{
	int val;
	node* leftchild;
	node* rightchild;
};
class Queue
{
	int rear, front;
	node ele[200];
public:
	Queue() { front = rear = 0; }
	void enqueue(node *&p)
	{
		if (p == NULL) return;
		ele[rear] = *p;
		rear++;
	}
	void dequeue(node *&p)
	{
		*p = ele[front];
		front++;
	}
	bool ifempty()
	{
		return front == rear;
	}
};
void creat_tree(stringstream &in,node *&root)
{
	root = new node; int v;
	if (in >> v)
	{
		if(v != -1)
		{
			root->val = v;
			creat_tree(in, root->leftchild);
			creat_tree(in, root->rightchild);
		}
		else root = NULL;
	}
}string s;
	getline(cin, s);
	stringstream ss(s);
	node *root1;node *root2;
	creat_tree(ss, root1);
	getline(cin, s);
	stringstream ss_(s);
	creat_tree(ss_, root2);
	bool flag = true;
	Queue q1; Queue q2;
	q1.enqueue(root1); q2.enqueue(root2);
	node *temp1 = new node;
	node *temp2 = new node;
	while(!q1.ifempty()&&!q2.ifempty())
	{
		q1.dequeue(temp1); q2.dequeue(temp2);
		if (temp1->val != temp2->val) { flag = false; break; }
		if (temp1->leftchild != NULL) q1.enqueue(temp1->leftchild);
		if (temp1->rightchild != NULL) q1.enqueue(temp1->rightchild);
		if (temp2->rightchild != NULL) q2.enqueue(temp2->rightchild);
		if (temp2->leftchild != NULL) q2.enqueue(temp2->leftchild);
	}
	if (q1.ifempty() != q2.ifempty()) flag = false;
	if (flag) cout << "YES";
	else cout << "NO";*/