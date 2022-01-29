#include<iostream>
#include<string>
using namespace std;
class Stack
{
	string *stack; 
	int top;
	int length;//’ª≥§∂»
public:
	Stack()
	{
		stack = new string[201];
		length = 201;
		top = -1;//ø’’ª
	}
	void enlarge();//¿©»›
	void push(string x);
	string pop();
	void kill() { delete[] stack; }
	string get(int x);
	int gettop() { return top; }
};
void Stack::enlarge()
{	length += 1000;
	string *new_stack = new string[length];
	for (int i = 0; i < length - 1000; ++i) new_stack[i] = stack[i];
	string *p = stack;
	stack = new_stack;
	delete[]p;
}
void Stack::push(string x)
{
	if (top + 1 == length) enlarge();
	top++;
	stack[top] = x;
}
string Stack::pop()
{
	string temp = stack[top];
	--top;
	return temp;
}
string Stack::get(int x)
{
	return stack[x];
}
int main()
{
	Stack sta;
	string s;
	getline(cin, s);
	s += '\0';
	int i = 0;
	while(s[i]!='\0')
	{
		string ele;
		if(s[i] >= 'a'&&s[i] <= 'z')
		{
			while (s[i] >= 'a'&&s[i] <= 'z')
			{
				ele += s[i];
				++i;
			}
			sta.push(ele);
		}
		else if(s[i] >= '0'&&s[i] <= '9')
		{
			while (s[i] >= '0'&&s[i] <= '9')
			{
				ele += s[i];
				++i;
			}
			sta.push(ele);
		}
		else if(s[i]=='[')
		{
			ele += s[i];
			++i;
			sta.push(ele);
		}
		else
		{
			++i;
			int t = sta.gettop();
			while (sta.get(t) != "[") --t;
			++t;
			string current;
			while (t <= sta.gettop()) { current += sta.get(t); ++t; }
			while (sta.get(sta.gettop()) != "[") sta.pop();
			sta.pop(); string times = sta.pop(); times += '\0';
			string new_top;
			long long times_ = 0; int x = 0;
			while (times[x] != '\0') { times_ *= 10; times_ += (times[x] - '0'); x++; }
			for(int i=0;i<times_;++i)
			{
				new_top += current;
			}
			sta.push(new_top);
		}
	}
	int out = 0;
	while (out <= sta.gettop()) { cout << sta.get(out); ++out; }
	sta.kill();
	//getchar();
	return 0;
}