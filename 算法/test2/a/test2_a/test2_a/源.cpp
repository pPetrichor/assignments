#include<iostream>
#include<vector>
#include<string>
using namespace std;
struct myword
{
	string val;
	string change;
};
class minheap_char//最小堆实现
{
	int size;//容量
	char *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	minheap_char()
	{

	}
	minheap_char(int n)//构造容量为n的最小堆
	{
		size = n;
		heap = new char[size];
		currentsize = 0;
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2;
		char temp = heap[j];
		while (j > 0)
		{
			if (heap[i] <= temp) break;
			else
			{
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			heap[j] = temp;
		}
	}
	void insert(char x)//向最小堆中插入元素
	{
		heap[currentsize] = x;
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
		//最小堆的下滑调整算法
	{
		int i = start;
		int j = 2 * i + 1;
		char temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;
			if (temp <= heap[j]) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	char remove()//删除最小堆的堆顶元素
	{
		char x = heap[0];
		heap[0] = heap[currentsize - 1];
		currentsize--;
		siftdown(0, currentsize - 1);
		return x;
	}
	void clear() { delete[]heap; }
};
class minheap_word//最小堆实现
{
	int size;//容量
	myword *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	minheap_word()
	{

	}
	minheap_word(int n)//构造容量为n的最小堆
	{
		size = n;
		heap = new myword[size];
		currentsize = 0;
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2;
		myword temp = heap[j];
		while (j > 0)
		{
			if (heap[i].change <= temp.change) break;
			else
			{
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			heap[j] = temp;
		}
	}
	void insert(myword x)//向最小堆中插入元素
	{
		heap[currentsize] = x;
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
		//最小堆的下滑调整算法
	{
		int i = start;
		int j = 2 * i + 1;
		myword temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j].change>heap[j + 1].change) j++;
			if (temp.change <= heap[j].change) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	void remove(myword &x)//删除最小堆的堆顶元素
	{
		x = heap[0];
		heap[0] = heap[currentsize - 1];
		currentsize--;
		siftdown(0, currentsize - 1);
	}
	void clear() { delete[]heap; }
};
class minheap_str//最小堆实现
{
	int size;//容量
	string *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	minheap_str()
	{

	}
	minheap_str(int n)//构造容量为n的最小堆
	{
		size = n;
		heap = new string[size];
		currentsize = 0;
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2;
		string temp = heap[j];
		while (j > 0)
		{
			if (heap[i] <= temp) break;
			else
			{
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			heap[j] = temp;
		}
	}
	void insert(string x)//向最小堆中插入元素
	{
		heap[currentsize] = x;
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
		//最小堆的下滑调整算法
	{
		int i = start;
		int j = 2 * i + 1;
		string temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;
			if (temp <= heap[j]) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	string remove()//删除最小堆的堆顶元素
	{
		string x = heap[0];
		heap[0] = heap[currentsize - 1];
		currentsize--;
		siftdown(0, currentsize - 1);
		return x;
	}
	void clear() { delete[]heap; }
};
int main()
{
	int n;
	cin >> n;
	vector<string> out;
	myword *sort_dict = new myword[n];
	for(int i=0;i<n;++i)
	{
		myword temp;
		cin >> temp.val;
		temp.change = temp.val;
		int j = 0;
		int l = temp.val.size();
		minheap_char heap(l);
		while (j < l)
		{
			heap.insert(temp.val[j]);
			j++;
		}
		j = 0;
		while (j < l)
		{
			 temp.change[j] = heap.remove();
			 j++;
		}
		heap.clear();
		sort_dict[i] = temp;
	}
	minheap_word dict(n);
	
	for(int i=0;i<n;++i)
	{
		dict.insert(sort_dict[i]);
	}
	for (int i = 0; i < n; ++i)
	{
		dict.remove(sort_dict[i]);
	}
	dict.clear();
	int i = 0;
	while (i < n)
	{
		if(i < (n-1) &&sort_dict[i].change == sort_dict[i+1].change)
		{
			myword current = sort_dict[i]; 
			++i;
			while (i < n && sort_dict[i].change == current.change)
			{
				if (sort_dict[i].val < current.val) current = sort_dict[i];
				++i;
			}
			out.push_back(current.val);
		}
		else ++i;
	}
	delete[]sort_dict;
	minheap_str out_sort(out.size());
	for(int i=0;i<out.size();++i)
	{
		out_sort.insert(out[i]);
	}
	for (int i = 0; i < out.size(); ++i)
	{
		out[i] = out_sort.remove();
	}
	out_sort.clear();
	cout << "wo yi yue du guan yu chao xi de shuo ming" << endl;
	int s = out.size();
	cout << s << endl;
	int t = 0;
	while (t < s)
	{
		cout << out[t] << endl;
		++t;
	}
	return 0;
}
