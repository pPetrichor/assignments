/*#include<iostream>
using namespace std;
struct myroad
{
	int start;
	int end;
	int val;
	int driver;
	int dist;
};
class minheap
{
	int size;
	myroad *heap;
	int currentsize;
public:
	minheap()
	{

	}
	minheap(int n)
	{
		size = n;
		heap = new myroad[size]; currentsize = 0;
		for (int i = 0; i < size; ++i) { heap[i].dist = -1; }
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2; myroad temp = heap[j];
		while (j > 0)
		{
			if (heap[i].dist <= temp.dist) break;
			else
			{
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			heap[j] = temp;
		}
	}
	void insert(myroad x)
	{
		heap[currentsize] = x;
		currentsize++;
	}
	void siftdown(int start, int end)
	{
		int i = start;
		int j = 2 * i + 1;
		myroad temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j].dist>heap[j + 1].dist) j++;
			if (temp.dist <= heap[j].dist) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	void remove(myroad &x)
	{
		x.dist = 1000000000; int t = 0;
		for(int i=0;i<currentsize;++i)
		{
			if (x.dist > heap[i].dist) { x = heap[i]; t = i; }
		}
		heap[t] = heap[currentsize - 1];
		currentsize--;
	}
	bool if_empty() { return currentsize == 0; }

};

int main()
{
	int n, m;
	scanf("%d", &n);
	scanf("%d", &m);
	minheap m_heap(m);
	int *dist1 = new int[n];
	int *dist2 = new int[n];
	
	for(int i=0;i<n;++i)
	{
		dist1[i] = 1000000000;
		dist2[i] = 1000000000;
	}
	myroad *road = new myroad[m];
	for (int i = 0; i < m; ++i)
	{
		scanf("%d", &road[i].start);
		scanf("%d", &road[i].end);
		scanf("%d", &road[i].val);
		scanf("%d", &road[i].driver);
		road[i].dist = road[i].val;
		if (road[i].start == 0)
			m_heap.insert(road[i]);
	}
	myroad x;
	while(!m_heap.if_empty())
	{
		m_heap.remove(x);
		if(x.driver==1)
		{
			if(dist1[x.end] > x.dist) dist1[x.end] = x.dist;
			for (int i = 0; i < m; ++i)
			{
				if (road[i].driver == 2)
				{
					if (road[i].start == x.end&&dist2[road[i].end] == 1000000000)
					{
						road[i].dist = dist1[x.end] + road[i].val;
						m_heap.insert(road[i]);
					}
							
				}
			}
		}
		else
		{
			if(dist2[x.end] > x.dist) dist2[x.end] = x.dist;
			for (int i = 0; i < m; ++i)
			{
				if (road[i].driver == 1)
				{
					if (road[i].start == x.end&&dist1[road[i].end] == 1000000000)
					{
						road[i].dist = dist2[x.end] + road[i].val;
						m_heap.insert(road[i]);
					}
						
				}
			}
		}

	}
	printf("0 ");
	for(int i=1;i<n;++i)
	{
		if (dist1[i] == 1000000000 && dist2[i] == 1000000000) printf("-1 ");
		else if (dist1[i] < dist2[i]) printf("%d ", dist1[i]);
		else printf("%d ", dist2[i]);
	}
	system("pause");
	return 0;
}*/
#include<iostream>
using namespace std;
struct road
{
	int val;
	int driver;
};
road map_[1000][1000];

int main()
{
	for (int i = 0; i < 1000; ++i)
		for (int j = 0; j < 1000; ++j)
			map_[i][j].val = 1000000000;
	int n, m;
	cin >> n >> m;
	int *dist1 = new int[n];
	int *dist2 = new int[n];
	bool *flag1 = new bool[n];
	bool *flag2 = new bool[n];
	for (int i = 0; i < n; ++i)
	{
		dist1[i] = 1000000000;
		dist2[i] = 1000000000;
		flag1[i] = 0; flag2[i] = 0;
	}
	for (int i = 0; i < m; ++i)
	{
		int s, e, l, d;
		cin >> s >> e >> l >> d;
		map_[s][e].val = l; map_[s][e].driver = d;
		if (s == 0)
		{
			if (d == 1) dist1[e] = l;
			else dist2[e] = l;
		}
	}
	dist1[0] = 0; flag1[0] = 1;
	dist2[0] = 0; flag2[0] = 1;
	for (int i = 0; i < n - 1; i++)
	{
		int min = 1000000000, w = 0;
		int temp = 0;
		for (int j = 0; j < n; j++)
			if (dist2[j] < min && !flag2[j])
			{
				temp = j;
				min = dist2[j];
			}
		flag2[temp] = 1;
		for (int k = 0; k < n; k++) {
			w = map_[temp][k].val;
			if (map_[temp][k].driver == 1 && !flag1[k] && w < 1000000000 && dist2[temp] + w < dist1[k])
			{
				dist1[k] = dist2[temp] + w;
			}
		}
		min = 1000000000;
		temp = 0;
		for (int j = 0; j < n; j++)
			if (!flag1[j] && dist1[j] < min) 
			{
				temp = j;
				min = dist1[j];
			}
		flag1[temp] = 1;

		for (int k = 0; k < n; k++)
		{
			w = map_[temp][k].val;
			if (map_[temp][k].driver == 2 && !flag2[k] && w < 1000000000 && dist1[temp] + w < dist2[k])
			{
				dist2[k] = dist1[temp] + w;
			}
		}
	}
	
	for (int i = 0; i < n; ++i)
	{
		if (dist1[i] == 1000000000 && dist2[i] == 1000000000) printf("-1 ");
		else if (dist1[i] < dist2[i]) printf("%d ", dist1[i]);
		else printf("%d ", dist2[i]);
	}
	system("pause");
	return 0;
}
