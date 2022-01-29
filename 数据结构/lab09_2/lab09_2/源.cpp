#include<iostream>
using namespace std;
struct mylink
{
	int x;
	mylink *next;
	mylink()
	{
		x = 0;
		next = NULL;
	}
	mylink(int a, mylink* b)
	{
		x = a;
		next = b;
	}
};
struct mynode
{
	int val;//¼ÇÂ¼¶È
	mylink *next;
};
int main()
{
	int n,m;
	cin >> n >> m;
	if (n != 1)
	{
		mynode *mylist = new mynode[n];
		for (int i = 0; i < n; ++i)
		{
			mylist[i].val = 0;
			mylist[i].next = NULL;
		}
		for (int i = 0; i < m; ++i)
		{
			int first, second;
			cin >> first >> second;
			mylist[first].val++; mylist[second].val++;
			mylink* pa = new mylink(second, mylist[first].next);
			mylink* pb = new mylink(first, mylist[second].next);
			mylist[first].next = pa;
			mylist[second].next = pb;
		}
		int remain = n;
		while (remain > 2)
		{
			int *to_be_minus = new int[remain];
			int pos = 0;
			for(int i=0;i<n;++i)
			{
				if (mylist[i].val == 1) { to_be_minus[pos] = i; ++pos; }
			}
			for(int i=0;i<pos;++i)
			{
				mylist[to_be_minus[i]].val--; 
				remain--;
				if (remain == 1) break;
				mylink *current = mylist[to_be_minus[i]].next;
				while(current!=NULL)
				{
					mylist[current->x].val--;
					current = current->next;
				}
			}
			delete[] to_be_minus;
		}
		
		for(int i=0;i<n;++i)
		{
			if (mylist[i].val > 0) cout << i << " ";
		}
	}
	system("pause");
	return 0;
}