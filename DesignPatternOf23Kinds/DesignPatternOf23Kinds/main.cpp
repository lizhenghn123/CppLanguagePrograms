#include <iostream>
#include <list>
using namespace std;

int main()
{
	list<int> cmds;
	cmds.push_back(34);
	cmds.push_back(45);
	cmds.push_front(400);

	cout<<"需要运行哪个项目？该项目本身并没有程序代码.haha\n";
	system("pause");
	return 0;
}