#include<iostream>
using namespace std;
/*本模块的任务是给定n个市民的收入后，给定m个查询区间
查询在这些区间之内的数据的平均值；而求平均值就是用此
区间内数据的和除以区间长度，所以问题自然而然就转化为
了求某个区间数据的和；题目要求算法的时间复杂度尽量控
制在O(mlog𝑛)，即m次查找每次的时间复杂度为log𝑛；所以本
模块的任务就是在log𝑛时间找到对应查找区间的和；而log𝑛
复杂度的查找自然而然地就与树联系了起来；
本模块采取的方法是利用线段树结构进行实现；首先将输入
数据从大到小排序，这样就得到了编号依次为1-n的有序数据
排列，于是我们就可以利用二分法切分这个数据排列区间，将
[1,n]依次二分为[1,n/2]、[n/2+1,n]……，到最后无法二分时，
就会得到[1,1]、[2,2]、[3,3]……[n,n]这样的n个区间；这时，
按照排序后的顺序依次给这n个区间赋值，记这n个数据依次为
a1、a2、a3……an，就得到了：当前(-∞,a1]数据和为a1、(a1,a2]
数据和为a2……(an-1,an]数据和为an；这样，查找一个区间[l,r]的
数据和，只需要将其映射到[nl,nr](l>=anl&&r<=anr)，由根向
下取可以组合成为[l,r]的区间，取其数据相加即可
*/
struct node//线段树中结点结构
{
	int left;//区间左值
	int right;//区间右值
	long long val = 0;//该区间中数据和
};
void siftdown(int start,int end,int a[])//利用堆排序以nlog𝑛的复杂度对数据进行排序
//最大堆的下滑调整算法：在start的左右分支均已为最大堆的前提下，从start开始到end，自上向下比较子女与父节点的值，
//将关键码大的上浮，继续向下层比较，从而将一个数组局部调整为最大堆
{
	int i = start;
	int j = 2 * i + 1;//j为i的左子女
	int temp = a[i];//记录当前父节点位置关键码
	while (j <= end)//判断是否到达结束位置
	{
		if (j < end&&a[j] < a[j + 1]) j++;//j指示左右子女较大的一个
		if (temp >= a[j]) break;//父节点大，无需调整，直接结束
		else { a[i] = a[j]; i = j; j = 2 * j + 1; }//关键码大的上浮，i，j下降，继续执行
	}
	a[i] = temp;//找到最初start处关键码的正确位置i，赋值
}
void heapsort(int a[],int n)//n为待调整数组长度
{
	for (int i = (n - 2) / 2; i >= 0; i--) siftdown(i, n - 1, a);//将数组转化为堆，并调整成为最大堆
	for (int i = n - 1; i >= 0; i--)//对数组进行排序，得到从小到大排序数组
	{
		int temp = a[0];
		a[0] = a[i];
		a[i] = temp;//最大值位置i确定，交换元素位置
		siftdown(0, i - 1, a);//在前i个元素中重建最大堆
	}
}
void push_up(int root,node tree[])//根节点状态更新：利用已知左右子女的值求和求父结点的值
{
	tree[root].val = tree[root * 2].val + tree[root * 2 + 1].val;//父结点区间数据和 = 左子女区间数据和+右子女区间数据和
}
void build(int l, int r, int root,node tree[],int a[])//线段树建树
{
	tree[root].left = l; tree[root].right = r;
	if (l == r)//已经离散化，赋上相应区间值
	{
		tree[root].val = a[l];
		return;
	}
	int mid = (l + r) / 2;//否则，继续二分处理
	build(l, mid, root * 2, tree, a);//递归处理左分支，相应root为当前root左子女
	build(mid + 1, r, root * 2 + 1, tree, a);//递归处理左分支，相应root为当前root右子女
	push_up(root,tree);//处理完毕后求父结点对应区间数据和
}
//有了以上内容，我们就可以通过建立线段树来查找了，不过在线段树中的查找区间是[1,n]之间的，所以我们还需要
//将输入的区间转化为[1,n]中对应的一个区间；为实现log𝑛复杂度，这里使用二分查找
int findleft(int x, int l, int r, int a[])
//二分法转化区间:给定l找到其在数组a中的位置i，i满足a[i]<=x<a[i+1]
//考虑到可能有相同的数据，而左边界应该为相同数据的最左，右边界应该为相同数据的最右，故写为两个find
{
	if (x < a[l]) return l;//小于最小值，左边界为l
	if (x > a[r]) return r;//大于最大值，左边界为r
	if (l == r) return l;
	int mid = (l + r) / 2;
	if (x > a[mid]) return findleft(x, mid + 1, r, a);//在右区间
	else return findleft(x, l, mid, a);//在左区间，将等于情况归为左区间以得到相同数据的第一个位置
}
int findright(int x, int l, int r, int a[])
//二分法转化区间:给定r找到其在数组a中的位置i，i满足a[i]<=x<a[i+1]
{
	if (x < a[l]) return l - 1;//小于最小值，右边界为l-1；第一次查找不会出现此情况
	if (x > a[r]) return r;//大于最大值，右边界为r
	if (l == r) return l;
	int mid = (l + r) / 2;
	if (x >= a[mid]) return findright(x, mid + 1, r, a);//在右区间，将等于情况归为右区间以得到相同数据的最后一个位置
	else return findright(x, l, mid, a);//在左区间
}
long long mysum(int i, int l, int r,node tree[])//利用线段树,从结点i开始求区间[l,r]数据和
{
	if (l == tree[i].left&&r == tree[i].right)//查询到对应区间,直接返回数据和
		return tree[i].val;
	int mid = (tree[i].left + tree[i].right) / 2;//否则二分向下查找
	if (r <= mid) return mysum(2 * i, l, r, tree);//查找区间在左分支中
	else if (l > mid) return mysum(2 * i + 1, l, r, tree);//查找区间在右分支中
	else//查找区间分布在左右两个分支中
		return mysum(i * 2, l, mid, tree) + mysum(i * 2 + 1, mid + 1, r, tree);
}
int main()
{
	int n, m;
	scanf("%d", &n); scanf("%d", &m);
	node *tree = new node[4*n+1];//创建线段树，最大空间为4n
	int *a = new int[n+1];//存储数据
	for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
	heapsort(a + 1, n);//排序
	build(1, n, 1, tree, a);
	int l, r;//定义要查询区间
	for(int i=0;i<m;++i)
	{
		scanf("%d", &l);
		scanf("%d", &r);
		if (l > r) { int temp = r; r = l; l = temp; }//取相应区间
		if (r < a[1]) printf("0\n");
		else if (l > a[n]) printf("0\n");//不在查询区间中的两种情况，均返回0
		else
		{
			int fl = findleft(l, 1, n, a);
			int fr = findright(r, 1, n, a);//转化查找区间
			if (fr < fl)  printf("0\n");//该区间内无数据
			else
			{
				long long sum = mysum(1, fl, fr, tree);//得到相应数据和
				printf("%d\n",sum / (fr - fl + 1));//输出平均值
			}
		}
		
		
	}
	system("pause");
	return 0;
}
