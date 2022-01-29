#define _CRT_SECURE_NO_WARNINGS//Ñ¹ÖÆºê
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
struct mynode
{
	int val;
	vector<int> neighbors;
	vector<int> weights;
};

int main()
{
	printf("I have read the rules about plagiarism punishment\n");
	int n;
	scanf("%d", &n);
	int* longest_path = new int[n];
	longest_path[0] = 0;
	for (int i = 1; i < n; ++i)
		longest_path[i] = -1000;
	vector<mynode> mylist;
	queue<int> todos;
	for (int i = 0; i < n; ++i)
	{	
		mynode temp;
		temp.val = 0;
		mylist.push_back(temp);
	}
	int first = 0;
	while (~scanf("%d",&first))
	{
		if (first == -1) break;
		int second, weight;
		scanf("%d", &second);
		scanf("%d", &weight);
		mylist[second].val++;
		mylist[first].neighbors.push_back(second);
		mylist[first].weights.push_back(weight);
	}
	todos.push(0);
	while (!todos.empty())
	{
		int current = todos.front();
		todos.pop();
		for(int i=0;i<mylist[current].neighbors.size();++i)
		{
			int c = mylist[current].neighbors[i];
			mylist[c].val--;
			if (mylist[c].val == 0) todos.push(c);
			int new_path = longest_path[current] + mylist[current].weights[i];
			if (new_path > longest_path[c]) longest_path[c] = new_path;
		}
	}
	for (int i = 1; i < n; ++i)
		printf("%d ", longest_path[i]);
	delete[] longest_path;
	system("pause");
	return 0;
}