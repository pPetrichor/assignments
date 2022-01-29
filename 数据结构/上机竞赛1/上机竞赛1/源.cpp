
/*int row, col;
	cin >> row >> col;
	int *head = new int[row*col];
	for(int i=0;i<row;++i)
		for(int j=0;j<col;++j)
		{
			cin >> head[i*col+j];
		}
	int up_ = 0, down_ = row - 1, left_ = 0, right_ = col - 1;
	int count = 0; int sum = row * col;
	while(count<sum)
	{
		int up = up_, down = down_, left = left_, right = right_;
		while (left <= right_) { cout << head[up_*col + left] << " "; left++; count++; }  up++;
		if (count == sum) break;
		while (up <= down_) { cout << head[up*col + right_] << " "; up++; count++;
		} right--; if (count == sum) break;
		while (right >= left_) { cout << head[down_*col + right] << " "; right--; count++;
		} down--; if (count == sum) break;
		while (down > up_) { cout << head[down*col + left_] << " "; down--; count++;
		}if (count == sum) break;
		up_++; down_--; left_++; right_--;
		up = up_, down = down_, left = left_, right = right_;
	}
	*/
/*#include<iostream>
#include<vector>
using namespace std;
int main()
{
	
	int all, depth;
	cin >> all >> depth;
	vector<int> p;
	vector<int> want;
	vector<int> q;
	int t; bool flag = 1;
	for(int i=0;i<all;++i)
	{
		p.push_back(i);
		cin >> t;
		want.push_back(t);
	}
	while(want.size()>0)
	{
		if (p[0] == want[0]) { p.erase(p.begin()); want.erase(want.begin()); }
		else if (q.size() == 0)
		{ 
			while(p[0]!=want[0]&&p.size()>0)
			{
				int temp = p[0]; p.erase(p.begin()); q.push_back(temp);
			}
			if (p.size() == 0) flag = 0;
			if (q.size() > depth) flag = 0;
		}
		else if (q[0] == want[0]) { q.erase(q.begin()); want.erase(want.begin());
		}
		else
		{
			flag = 0; break;
		}
	}
	if (flag) cout << "YES";
	else cout << "NO";
	//system("pause");
	return 0;
}*/
/*string workout_1(string a,char b)
{

	int x[2];x[1] = 0;int sb_i = b - '0';
	for(int i=0;i<a.length();++i)
	{
		char sa = a[a.length() - 1 - i];
		int sa_i = sa - '0';
		x[0] = (sa_i*sb_i) % 10; x[0] += x[1];
		x[1] = (sa_i*sb_i) / 10;
		if (x[0] >= 10) { x[1]++; x[0] -= 10; }
		string result;
		result += x[0];
	}//resultµ¹Ðò
}
string add(string a,string b)
{
	
}
string workout_2(string a,string b)
{
	string *p = new string[b.length()];
	for(int i=0;i<b.length();++i)
	{
		p[i] = workout_1(a, b[b.length() - 1 - i]);
	}

}*/
/*string BigNumMultiply(string str1, string str2)
{
	int size1 = str1.size(), size2 = str2.size();
	string str(size1 + size2, '0');
	for (int i = size2 - 1; i >= 0; --i)
	{
		int mulflag = 0, addflag = 0;
		for (int j = size1 - 1; j >= 0; --j)
		{
			int temp1 = (str2[i] - '0')*(str1[j] - '0') + mulflag;
			mulflag = temp1 / 10;
			temp1 = temp1 % 10;
			int temp2 = str[i + j + 1] - '0' + temp1 + addflag;
			str[i + j + 1] = temp2 % 10 + 48;
			addflag = temp2 / 10;
		}
		str[i] += mulflag + addflag;
	}
	if (str[0] == '0')
		str = str.substr(1, str.size());
	return str;
}
int main()
{
	string x1, x2;
	int num = 0;
	cin >> num;
	if (num == 1) { cin >> x1; cout << x1; }
	else
	{
		cin >> x1; cin >> x2;
		int t = 2;
		while(t<=num)
		{
			x1 = BigNumMultiply(x1, x2);
			if(t<num) cin >> x2;
			t++;
		}
		cout << x1;
	}
	system("pause");
	return 0;
}*/
#include<iostream>
using namespace std;
int main()
{
	long long n, a[1000000] = {}, s[1000000] = { 0,1 }, top = 1, sum = 0;
	cin >> n;
	for (int i = 1; i <= n; ++i)
		cin >> a[i];
	for (int i = 2; i <= n; ++i)
		if (a[i] < a[s[top]]) s[++top] = i;
		else
		{
			while (a[s[top]] <= a[i] && top > 0)
				sum += i - s[top--] - 1;
			s[++top] = i;
		}
	while (top > 0)
		sum += n - s[top--];
	cout << sum ;
	return 0;
}