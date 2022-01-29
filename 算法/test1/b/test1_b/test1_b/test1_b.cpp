#include<iostream>
using namespace std;
class minheap//最小堆实现
{
	int size;//容量
	int *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	minheap()
	{

	}
	minheap(int n)//构造容量为n的最小堆
	{
		size = n;
		heap = new int[size];
		currentsize = 0;
	}
	void siftup(int start)
	//最小堆的上滑调整算法
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];//i指示j的父结点
		while (j > 0)//判断是否调整到根
		{
			if (heap[i] <= temp) break;//父结点小，无需调整
			else//子女结点小，需调整
			{
				heap[j] = heap[i];//交换
				j = i;
				i = (i - 1) / 2;//继续执行
			}
			heap[j] = temp;//回填
		}
	}
	void insert(int x)//向最小堆中插入元素
	{
		heap[currentsize] = x;//在堆末赋值x
		siftup(currentsize);//向上调整，将x放到正确位置
		currentsize++;//存储数据个数加1
	}
	void siftdown(int start, int end)
	//最小堆的下滑调整算法
	{
		int i = start;
		int j = 2 * i + 1;//j为i的左子女
		int temp = heap[i];//记录当前父节点位置关键码
		while (j <= end)//判断是否到达结束位置
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;//j指示左右子女较小的一个
			if (temp <= heap[j]) break;//父节点小，无需调整，直接结束
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }//关键码小的上浮，i，j下降，继续执行
		}
		heap[i] = temp;//找到最初start处关键码的正确位置i，赋值
	}
	void remove(int &x)//删除最小堆的堆顶元素
	{
		x = heap[0];//记录堆顶元素到x中
		heap[0] = heap[currentsize - 1];//将堆的最后一个元素填补到堆顶
		currentsize--;//数据个数减1，堆顶元素被删除，此时根的左右分支仍为最小堆
		siftdown(0, currentsize - 1);//执行一次下滑调整，重新得到最小堆
	}
};
int main()
{
	int k,n; cin >> k >> n;
	minheap mh(n);
	int temp;
	for(int i=0;i<n;++i)
	{
		cin >> temp;
		mh.insert(temp);
	}
	int out;
	if (n % 2 == 0)
	{
		for (int i = 0; i < n / 2 + k - 1; ++i)
		{
			mh.remove(out);
			if (i >= n / 2 - 1 - k)
				cout << out << " ";
		}
		mh.remove(out);
		cout << out;
	}
	else
	{
		for (int i = 0; i < n / 2 + k; ++i)
		{
			mh.remove(out);
			if (i >= n / 2 - k)
				cout << out << " ";
		}
		mh.remove(out);
		cout << out;
	}
	return 0;
}