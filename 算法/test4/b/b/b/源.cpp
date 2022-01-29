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
	int val;
	mylink *next;
};
int main()
{
	cout << "I have read the rules about plagiarism punishment" << endl;
	int n;
	cin >> n;
	int *cost = new int[n];
	int *real_cost = new int[n];
	int a = 0;
	for (int i = 0; i < n; ++i)
	{
		cin >> a;
		cin >> cost[a - 1];
		real_cost[i] = 0; 
	}
	if (n == 1) cout << cost[0];
	else
	{
		mynode *mylist = new mynode[n];
		for (int i = 0; i < n; ++i)
		{
			mylist[i].val = 0;
			mylist[i].next = NULL;
		}
		int first = 0;
		while (cin>>first)
		{
			int second;
			cin >> second;
			first--; second--;
			mylist[first].val++; 
			mylink* pb = new mylink(first, mylist[second].next);
			mylist[second].next = pb;
		}
		bool flag = true;
		for (int i = 0; i < n; ++i)
		{
			if (mylist[i].val > 0) { flag = false; break; }
		}
		while(!flag)
		{
			for(int i = 0;i<n;++i)
			{
				if(mylist[i].val == 0)
				{
					mylink* current = mylist[i].next;
					if(current != NULL) mylist[i].val--;
					while (current != NULL)
					{
						int c = real_cost[i] + cost[i];
						if (real_cost[current->x] < c)
							real_cost[current->x] = c;
						mylist[current->x].val--;
						current = current->next;
					}
				}
			}
			flag = true;
			for (int i = 0; i < n; ++i)
			{
				if (mylist[i].val > 0) { flag = false; break; }
			}
		}
		for (int i = 0; i < n; ++i)
		{
			if (mylist[i].val == 0) real_cost[i]+=cost[i];
		}
		delete[]mylist;
		delete[]cost;
		int ret = -1;
		for(int i = 0;i<n;++i)
		{
			if (real_cost[i] > ret) ret = real_cost[i];
		}
		cout << ret;
	}
	system("pause");
	return 0;
}