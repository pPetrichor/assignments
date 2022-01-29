#include<iostream>
using namespace std;

int main()
{
	int n, time;
	cin >> n >> time;
	int mysum = 0;int mymax = 0;
	int *a = new int[n];
	for(int i=0;i<n;++i)
	{
		cin >> a[i];
		mysum += a[i];
		if (mymax < a[i]) mymax = a[i];
	}
	int slow = (mysum / time) - 1;
	int speed = slow;
	do
	{
		speed = (slow + mymax) / 2;
		int cost = 0;
		for (int i = 0; i < n&&cost <= time; ++i)
		{
			cost += (a[i] / speed + ((a[i] % speed) != 0));
		}
		if (cost <= time) { mymax = speed; }
		else slow = speed;
	} while ((slow + 1) != mymax);
	cout << mymax;
	system("pause");
	return 0;
}