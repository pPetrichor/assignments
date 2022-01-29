#include<iostream>
#include<map>
using namespace std;
int main()
{
	map<int,int> tree;
	int n; cin >> n;
	for(int i=0;i<n;++i)
	{
		int temp, time;
		time = i;
		cin >> temp;
		tree.insert(pair<int,int>(temp,time));
		if (i > 0)
		{
			map<int, int>::const_iterator it;
			it = tree.find(temp);
			if (it == tree.begin()) { it++; cout << it->first << " "; }
			else if ((++it) == tree.end()) { it--; it--; cout << it->first << " "; }
			else
			{
				it--;
				map<int, int>::const_iterator front;
				map<int, int>::const_iterator behind;
				front = --it; behind = ++it; behind++;
				if (front->second > behind->second) cout << front->first << " ";
				else cout << behind->first << " ";
			}
		}
	}
	system("pause");
	return 0;
}