#include <iostream>
#include <string>
using namespace std;
class Stack
{
	string *stack;
	int top;
	int length;//栈长度
public:
	Stack()
	{
		stack = new string[201];
		length = 201;
		top = -1;//空栈
	}
	void enlarge();//扩容
	void push(string x);
	string pop();
	void kill() { delete[] stack; }
	string get(int x);
	int gettop() { return top; }
};
void Stack::enlarge()
{
	length += 1000;
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
	int i = 1;//已知长度不为0
	while (s[i] != '\0')
	{
		string ele;
		while(s[i]!='/'&&s[i] != '\0')
		{
			ele += s[i];
			++i;
		}
		if (ele.length() != 0&&ele!=".")//不是连续的'/'也不是'/./'
		{
			if (ele == "..")
			{
				if (sta.gettop() != -1)
				sta.pop();
			}
			else sta.push(ele);
		}
		++i;
	}
	if (sta.gettop() == -1) cout << '/';
	else
	{
		for(int i=0;i<=sta.gettop();++i)
		{
			cout << '/' << sta.get(i);
		}
	}
	sta.kill();
	system("pause");
	return 0;
}