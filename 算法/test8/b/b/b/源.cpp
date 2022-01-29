#include<iostream>
#include<vector>
#include<sstream>
#include<string>
#include<queue>
using namespace std;
struct node
{
	int father;
	int number_of_childs;
};
int main()
{
	cout << "I have read the rules about plagiarism punishment" << endl;
	string line;
	vector<node> nodes;
	int n = 0;
	while (getline(cin, line))
	{
		stringstream ss(line);
		int current, x;
		ss >> current;
		if (current == -1) break;
		if (current >= n)
		{
			node temp;
			temp.father = -1;
			temp.number_of_childs = 0;
			nodes.push_back(temp);
			n++;
		}
		
		while (ss >> x)
		{
			if (x < n) nodes[x].father = current;
			else
			{
				node t;
				t.father = current;
				t.number_of_childs = 0;
				nodes.push_back(t);
				n++;
			}
			nodes[current].number_of_childs++;
		}
		
	}
	int *A = new int[n];
	int *B = new int[n];
	for (int i = 0; i < n; ++i)
	{
		A[i] = 0;
		B[i] = 1;
	}
	queue<int> myqueue;
	for(int i=0;i<n;i++)
	{
		if (nodes[i].number_of_childs == 0)
			myqueue.push(i);
	}
	while (!myqueue.empty())
	{
		int c = myqueue.front();
		myqueue.pop();
		int w = nodes[c].father;
		if (w == -1) break;
		nodes[w].number_of_childs--;
		if (nodes[w].number_of_childs == 0)
		{
			myqueue.push(w);
		}
		A[w] += B[c];
		if (A[c] < B[c]) B[w] += A[c];
		else B[w] += B[c];
	}
	if (A[0] < B[0]) cout << A[0];
	else cout << B[0];
	delete[] A;
	delete[] B;
	system("pause");
	return 0;
}