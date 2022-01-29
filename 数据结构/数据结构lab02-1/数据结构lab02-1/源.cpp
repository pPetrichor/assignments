#include<iostream>
using namespace std;
struct node
{
	int val;
	node *next;
	bool flag;
};
class List
{
	node* head;
	int key;
public:
	List() { head = new node; head->next = NULL; }
	void creatlist();
	void print();
};
void List::creatlist()
{
	cin >> head->val;
	node* current = head;
	cin >> key;
	for (int i = 0; i < head->val; ++i)
	{
		node *t = new node;
		if (t == NULL) { cerr << "wrong!"; exit(-1); }
		cin >> t->val;
		if (t->val < key) t->flag = 1;
		else t->flag = 0;
		t->next = NULL;
		current->next = t;
		current = t;
	}

}
void List::print()
{
	if (head->val == 0) {
		cout << " "; return;
	}
	node*current = head->next;
	while (current != NULL)
	{
		if(current->flag==1)
		cout << current->val << " ";
		current = current->next;
	}
	current = head->next;
	while (current != NULL)
	{
		if (current->flag == 0)
			cout << current->val << " ";
		current = current->next;
	}
}
int main()
{
	List listnode;
	listnode.creatlist();
	listnode.print();
	return 0;
}