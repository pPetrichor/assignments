#include<iostream>
using namespace std;
struct person
{
	int weight;
	int height;
};
void QuickSort(person *arr, int low, int high)
{
	if (low >= high) return;
	int left = low, right = high;
	int key = arr[low].height; int key_ = arr[low].weight;
	while (low < high)
	{
		while (low < high && arr[high].height <= key) --high;
		arr[low].height = arr[high].height; arr[low].weight = arr[high].weight;
		while (low < high && arr[low].height >= key) ++low;
		arr[high].height = arr[low].height; arr[high].weight = arr[low].weight;
	}
	arr[low].height = key; arr[low].weight = key_;
	//每次排序后都分成两部分[left, low) (low, right]
	//arr[low]的位置是一定是有序的
	QuickSort(arr, left, low - 1);
	QuickSort(arr, low + 1, right);
	return;
}
int LIS(person A[], int n)
 {
	int* d = new int[n];
	int len = 1;
	int i, j;
	for (i = 0; i < n; i++)
	{
		d[i] = 1;
		for (j = 0; j < i; j++)
		{
			if (A[j].weight > A[i].weight && (d[j] + 1) >= d[i] && A[j].height > A[i].height)
			 d[i] = d[j] + 1;
		}
		if (d[i] > len) len = d[i];
	}
	delete[]d;
	return len;
}
int main()
{
	int n; cin >> n;
	person *people = new person[n];
	for(int i=0;i<n;++i)
	{
		cin >> people[i].height;
		cin >> people[i].weight;
	}
	QuickSort(people, 0, n - 1);
	cout << LIS(people, n);
	system("pause");
	return 0;
}