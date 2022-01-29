#include<iostream>
#include<queue>
using namespace std;//地图模式？
int main()
{
	int m, n;
	cin >> m >> n;
	int all = m * n;
	int *array = new int[all];
	int *length = new int[all];
	int start = 0;
	for (int i = 0; i < all; ++i)
	{
		cin >> array[i];
		if (array[i] == 1) { length[i] == 0; start = i; }
		else length[i] = -1;
	}
	queue<int> campus;
	campus.push(start);
	while(!campus.empty())
	{
		int current = campus.front();
		campus.pop();
		if(start >= m)//上
		{
			if (array[start - m] == -1) { array[start - m] = 1; campus.push(start - m); }
			
		}
		if (start + m < all)//下
		{
			if (array[start + m] == -1) { array[start + m] = 1; campus.push(start + m); }
		}

	}
}