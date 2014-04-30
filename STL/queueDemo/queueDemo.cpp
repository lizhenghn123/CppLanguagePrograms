#include <iostream>
#include <queue>
#include <string>
using namespace std;

int main ()
{
	string s1("C++");
	string s2("is");
	string s3("powerfull");
	string s4("language");

	queue<string> que;
	que.push(s1);
	que.push(s2);
	que.push(s3);
	que.push(s4);

	cout << "size of queue 'que' = " << que.size() << endl;

	string temp = que.back();
	cout << temp << endl;
	while ( !que.empty() )
	{
		temp = que.front();
		cout << temp << " ";
		que.pop();
	}
	cout << endl;

	system("pause");
	return 0;
} 