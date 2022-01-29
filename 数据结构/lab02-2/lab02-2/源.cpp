#include<iostream>
using namespace std;
struct node
{
	int val;
	node *next;
	node *front;
};
class Dblist
{
	node *head;
	node *last;
	int length;
public:
	Dblist() { head = new node; head->front = head; head->next = head; last = head; }
	void creatdblist();
	void remove(node *a);
	void choose();
	void deletelist();
};
void Dblist::creatdblist()
{
	cin >> length;
	node*f = head;
	for(int i=0;i<length;++i)
	{
		node*p = new node;
		if (p == NULL) { cerr << "wrong!"; exit(-1); }
		p->val = i + 1;
		p->front = f;
		p->next = f->next;
		f->next = p;
		f = p;
		head->front = p;
	}
	last = f;
}
void Dblist::remove(node*a)
{
	a->next->front = a->front;
	a->front->next = a->next;
	delete a;
}
void Dblist::choose()
{
	int k, m;
	cin >> k >> m;
	node *p1 = head->next, *p2 = last;
	while(length>1)
	{
		for (int i = 1; i < k; ++i) { if (p1 == head) p1 = p1->next;  p1 = p1->next; if (p1 == head) p1 = p1->next; }
		for (int i = 1; i < m; ++i) { if (p2 == head) p2 = p2->front; p2 = p2->front;if (p2 == head) p2 = p2->front; }
		if (p1 == p2) { cout << p1->val; break; }
		else 
		{
			length -= 2;
			node *temp = p1; p1 = p1->next; cout << temp->val << " "; remove(temp);
			temp = p2; p2 = p2->front; cout << temp->val;
			if (length != 0) cout << " ";
			if (temp == p1)//p1指向下一个时可能会指到p2要删的，所以判断一下，如果删除，则p1取下一个
				p1 = p1->next;
			remove(temp);
		}
	}
	if (length == 1) cout << head->next->val;
}
void Dblist::deletelist()
{
	if (length == 0) delete head;
	else 
	{
		node *t = head->next;
		delete t;
		delete head;
	}
}
int main()
{
	Dblist dblist;
	dblist.creatdblist();
	dblist.choose();
	dblist.deletelist();
	system("pause");
	return 0;
}