#include<iostream>
#include<stack>
#include<vector>
#include<sstream>
#include<string>
#define white 1
#define gray 2
#define black 3
using namespace std;

struct node
{
	vector<int> neighbors;
	int scc_number;
	int color;
};
struct scc_node
{
	vector<int> neighbors;
	int n_nodes;
	int vals;
	int color;
};
stack<int> nodestack;
int number = 0;
vector<node*> G;
vector<node*> GT;
vector<scc_node*> scc_G;
void DFS1(int v)
{
	(*G[v]).color = gray;
	for (int i = 0; i < (*G[v]).neighbors.size(); ++i)
	{
		if ((*G[(*G[v]).neighbors[i]]).color == white)
		{
			DFS1((*G[v]).neighbors[i]);
		}
	}
	nodestack.push(v);
	(*G[v]).color = black;
}
void DFS2(int v)
{
	(*GT[v]).color = gray;
	for (int i = 0; i < (*GT[v]).neighbors.size(); ++i)
	{
		if ((*GT[(*GT[v]).neighbors[i]]).color == white)
		{
			DFS2((*GT[v]).neighbors[i]);
		}
	}
	if ((*GT[v]).scc_number == -1)
	{
		(*GT[v]).scc_number = number;
		
	}
	(*GT[v]).color = black;
}
void DFS_WRAPPER()
{
	for (int i = 0; i < G.size(); ++i)
	{
		if ((*G[i]).color == white)
		{
			DFS1(i);
		}
	}
}
void scc()
{
	DFS_WRAPPER();	
	for(int i=0;i<G.size();++i)
	{
		for (int j = 0; j < (*G[i]).neighbors.size(); ++j)
		{
			(*GT[(*G[i]).neighbors[j]]).neighbors.push_back(i);
		}
	}
	while(!nodestack.empty())
	{
		int current = nodestack.top();
		nodestack.pop();
		while ((*GT[current]).scc_number != -1 && !nodestack.empty())
		{
			current = nodestack.top();
			nodestack.pop();
		}
		if (nodestack.empty() && (*GT[current]).scc_number != -1) break;
		DFS2(current);
		number++;
	}
}
void DFS_scc(int v, int&sum)
{
	sum += (*scc_G[v]).n_nodes;
	(*scc_G[v]).color = gray;
	for (int i = 0; i < (*scc_G[v]).neighbors.size(); ++i)
	{
		if ((*scc_G[(*scc_G[v]).neighbors[i]]).color == white)
		{
			DFS_scc((*scc_G[v]).neighbors[i], sum);
		}
	}
	
	(*scc_G[v]).color = black;
}
int main()
{
	cout << "I have read the rules about plagiarism punishment" << endl;
	string line;
	while(getline(cin,line))
	{
		node* temp = new node;
		(*temp).color = white;
		if (line == "")
		{
			G.push_back(temp);
			continue;
		}
		stringstream ss(line);
		int x;
		ss >> x;
		if (x == -1) break;
		else (*temp).neighbors.push_back(x);
		while (ss >> x) (*temp).neighbors.push_back(x);
		G.push_back(temp);
	}
	
	for (int k = 0; k < G.size(); ++k)
	{
		node *temp = new node;
		(*temp).color = white;
		(*temp).scc_number = -1;
		GT.push_back(temp);
	}
	scc();
	for (int i = 0; i < G.size(); ++i) delete G[i];
	vector<node*>().swap(G);
	for(int i=0;i<number;++i)
	{
		scc_node *temp = new scc_node;
		(*temp).n_nodes = 0;
		(*temp).vals = 0;
		(*temp).color = white;
		scc_G.push_back(temp);
	}
	for(int i=0;i<GT.size();++i)
	{
		int x = (*GT[i]).scc_number;
		(*scc_G[x]).n_nodes++;
		for(int j=0;j< (*GT[i]).neighbors.size();++j)
		{
			int y = (*GT[(*GT[i]).neighbors[j]]).scc_number;
			bool flag = false;
			if (x != y)
			{
				for (int k = 0; k < (*scc_G[y]).neighbors.size(); ++k)
				{
					if ((*scc_G[y]).neighbors[k] == x)
					{
						flag = true; break;
					}
				}
				if (!flag)
				{
					(*scc_G[y]).neighbors.push_back(x);
					(*scc_G[x]).vals++;
				}
			}
		}
	}
	int max = 0;
	vector<int> max_scc;
	for (int i = 0; i < scc_G.size(); ++i)
	{
		if((*scc_G[i]).vals==0)
		{
			int current = 0;
			for (int k = 0; k < scc_G.size(); ++k) (*scc_G[k]).color = white;
			DFS_scc(i,current);
			if (current > max)
			{
				max = current;
				max_scc.clear();
				max_scc.push_back(i);
			}
			else if(current == max)
			{
				max_scc.push_back(i);
			}
		}
	}
	cout << max-1 << endl;
	for (int i = 0; i < GT.size(); ++i)
	{
		bool flag = false;
		for (int j = 0; j < max_scc.size(); ++j)
		{
			if ((*GT[i]).scc_number == max_scc[j])
			{
				flag = true;
				break;
			}
		}
		if (flag) cout << i << " ";
	}
	for (int i = 0; i < GT.size(); ++i) delete GT[i];
	vector<node*>().swap(GT);
	for (int i = 0; i < scc_G.size(); ++i) delete scc_G[i];
	vector<scc_node*>().swap(scc_G);
	system("pause");
	return 0;
}