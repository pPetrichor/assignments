#include<iostream>
using namespace std;
int main()
{
	const int MAX_SIZE = 1000001;
	int i = 0;
	int left = 0, need = 0;
	char str[MAX_SIZE];
	cin >> str;
	while(str[i]!='\0')
	{
		if (str[i] == '(') left++;
		else if (left > 0)
			left--;
		else need++;
		i++;
	}
	cout << need + left;
	return 0;
}