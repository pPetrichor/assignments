#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<math.h>
using namespace std;
struct node
{
	char* s;
	node* next;
};
class stack
{
	node* top;
	node* head;
public:
	stack() { top = NULL; head = NULL; }
	void push(char *p);
	char* pop();
	void print()
	{
		while(head!=NULL)
		{
			cout << head->s;
			head = head->next;
		}
	}
};
void stack::push(char* p)
{
	node *t = new node;
	t->s = p;
	t->next = NULL;
	if (top != NULL) top->next = t;
	top = t;
	if (head == NULL) head = t;
}
char* stack::pop()
{
	if (head == top && head != NULL) { char* q = top->s; top = NULL; head = NULL; return q; }
	if (head == NULL) return NULL;
	node *current = head;
	char *q = NULL;
	while (current->next != top)
	{
		current = current->next;
	}
	q = top->s;
	
	top = current;
	if (top != NULL) top->next = NULL;
	return q;
}
int main()
{
	stack s;
	char *input = new char[201];
	for (int i = 0; i < 201; ++i) input[i] = '\0';
	cin.getline(input, 200);
	if (input[0] == '\0') return 0;
	int i = 0;
	while(input[i]!='\0')
	{
		char *ele = new char[201];
		for (int i = 0; i < 201; ++i) ele[i] = '\0';
		int j = 0;
		while(input[i]>='a'&&input[i]<='z')
		{
			ele[j] = input[i];
			++i; ++j;
		}
		if (j > 0)
		{
			char*current = NULL;
			current=s.pop();
			if (current == NULL) s.push(ele);
			else if(current[0]>='a'&&current[0]<='z')
			{
				long long l = strlen(current);
				char *new_top_ = new char[l + 200];
				strcpy(new_top_, current);
				strcat(new_top_, ele);
				delete current;
				s.push(new_top_);
			}
			else { s.push(current); s.push(ele); }
		}
		else
		{
			while (input[i] >= '0'&&input[i] <= '9')
			{
				ele[j] = input[i];
				++i; ++j;
			}
			if (j > 0) s.push(ele);
			else if (input[i] == '[') { ele[0] = '['; s.push(ele); ++i; }
			else//']'
			{
				++i;
				char*current = NULL;
				current=s.pop();
				if (current!=NULL&&current[0] == '[')
				{
					current=s.pop();
					if(current!=NULL)
					{
						if (!(current[0] >= '0'&&current[0] <= '9')) s.push(current);
					}
					
				}
				else
				{
					char*times = NULL;
					times=s.pop(); times=s.pop();
					if (times == NULL) s.push(current);
					else
					{
						char*new_top = NULL;
						new_top = s.pop();
						if (new_top == NULL||new_top[0]=='[')
						{

							long long times_ = 0;
							int x = 0;
							while (times[x] != '\0') { times_ *= 10; times_ += (times[x] - '0'); x++; }
							char *new_top_ = new char[times_*(long)strlen(current) + 1];
							strcpy(new_top_, current);
							for (long long i = 1; i < times_; ++i)
							{
								strcat(new_top_, current);
							}
							if (new_top!=NULL&&new_top[0] == '[') s.push(new_top);
							s.push(new_top_);
						}
						else
						{
							if (new_top[0] >= 'a'&&new_top[0] <= 'z')
							{
								long long times_ = 0;
								int x = 0;
								while (times[x] != '\0') { times_ *= 10; times_ += (times[x] - '0'); x++; }
								for (long long i = 0; i < times_; ++i)
								{
									long long l = strlen(new_top);
									//cout << new_top[1];
									char *new_top_ = new char[l + 200];
									strcpy(new_top_, new_top);
									strcat(new_top_, current);
									delete new_top;
									new_top = new_top_;
								}
								s.push(new_top);
							}
							
						}

					}
				}
			}
		}
	}
	s.print();
	getchar();
	return 0;
}