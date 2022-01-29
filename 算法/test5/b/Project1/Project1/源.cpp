#include<iostream>
#include<vector>
#include<algorithm>
#define white 1
#define gray 2
#define black 3
using namespace std;
struct node
{
	int parent;
	int number;
	int color;
	vector<int> neighbors;
	int discover;
	int back;
};
struct edge
{
	int s;
	int e;
};
bool compare(edge a,edge b)
{
	return a.s < b.s;
}
int mytime = 0;
vector<node> nodes;
vector<int> articulation_point;
vector<edge> bridge;
void DFS(int v)
{
	nodes[v].color = gray;
	mytime++;
	nodes[v].discover = mytime;
	nodes[v].back = mytime;
	for(int i=0;i< nodes[v].neighbors.size();++i)
	{
		int w = nodes[v].neighbors[i];
		if (nodes[w].color == white)
		{
			nodes[w].parent = v;
			DFS(w);
			if (nodes[w].back >= nodes[v].discover && nodes[v].parent!=-1)
			{
				articulation_point.push_back(nodes[v].number);
			}
			if (nodes[w].back > nodes[v].discover)
			{
				edge x;
				if(v > w)
				{
					x.s = w; x.e = v;
					
				}
				else
				{
					x.e = w; x.s = v;
				}
				bridge.push_back(x);
			}
			nodes[v].back = (nodes[v].back < nodes[w].back ? nodes[v].back: nodes[w].back);
		}
		else if(nodes[w].color == gray && nodes[w].number!= nodes[v].parent)
			nodes[v].back = (nodes[v].back < nodes[w].discover ? nodes[v].back : nodes[w].discover);
	}
}

int main()
{
	int n;
	cin >> n;
	cout << "wo yi yue du guan yu chao xi de shuo ming" << endl;
	for (int i = 0; i < n; ++i)
	{
		node temp;
		temp.color = white;
		temp.number = i;
		nodes.push_back(temp);
	}
	int a, b;
	while(cin>>a)
	{
		if (a == -1) break;
		cin >> b;
		nodes[a].neighbors.push_back(b);
		nodes[b].neighbors.push_back(a);
	}
	nodes[0].parent = -1;
	DFS(0);
	int root = 0;
	for(int i=0;i<n;++i)
	{
		if (nodes[i].parent == 0) root++;
	}
	if (root > 1) articulation_point.push_back(0);
	sort(articulation_point.begin(),articulation_point.end());
	sort(bridge.begin(), bridge.end(), compare);
	for (int i = 0; i < articulation_point.size(); ++i) cout << articulation_point[i] << endl;
	for (int i = 0; i < bridge.size(); ++i) cout << bridge[i].s << " " << bridge[i].e << endl;
	system("pause");
	return 0;
}
