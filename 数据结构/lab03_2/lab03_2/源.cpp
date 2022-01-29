#include<iostream>
using namespace std;
struct node {
	char ch;
	node *next;
};
int main()
{
	char str[100001] = {'\0'};
	cin.getline(str, 100001);
	int i = 0;
	node *all_head = NULL;
	while(str[i]!='\0')
	{
		if(str[i]!=' ')
		{
			node *head = new node;
			head->ch = str[i]; head->next = NULL;
			++i;
			node *last = head;
			while(str[i]!=' '&&str[i] != '\0')
			{
				node *p = new node;
				p->ch = str[i]; p->next = NULL;
				last->next = p; last = p;
				++i;
			}
			if(str[i] != '\0')
			{
				last->next = all_head;
				node *p = new node;
				p->ch = ' '; p->next = head;
				all_head = p;
			}
			else 
			{
				last->next = all_head;
				all_head = head;
			}
		}
		else
		{
			node *p = new node;
			p->ch = ' ';
			p->next = all_head;
			all_head = p;
		}
		++i;
	}
	while (all_head != NULL)
	{
		cout << all_head->ch;
		all_head = all_head->next;
	}
	//system("pause");
	return 0;

}