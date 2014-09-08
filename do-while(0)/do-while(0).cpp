#include <iostream>
//#include <string>

void foo1(){ std::cout << "foo1\n"; }
void foo2(){ std::cout << "foo2\n"; }

#define DOSOMETHING1()  foo1(); foo2();
#define DOSOMETHING2() { foo1(); foo2(); }
#define DOSOMETHING3() \
					do{ \
					foo1();\
					foo2();\
					}while(0)\

#define EMPTYMICRO do{}while(0)

void test_do_while_1()
{
	int a = 1;
	if(a>0)
		DOSOMETHING1();
	//else
	//	std::cout << "a <= 0 \n";

	if(a>0)
		DOSOMETHING2();

	if(a>0)
		DOSOMETHING3();
}

void test_do_while_2()
{
	{	//gotoÓï¾äÁ÷
		int *ptr = (int*)malloc(sizeof(int));
		int error = 0;

		//dosomething...;
		if(error)
			goto END;
		//dosomething...;
		if(error)
			goto END;
		//dosomething...;
END:
		free(ptr);
	}

	{	//do-while(0)Óï¾äÁ÷
		int *ptr = (int*)malloc(sizeof(int));
		int error = 0;
		do
		{
			//dosomething...;
			if(error)
				break;
			//dosomething...;
			if(error)
				break;
			//dosomething...;
		}
		while(0);

		free(ptr);
	}
}

void test_do_while_3()
{
	do
	{
		//int key;
		//std::string value;
		//dosomething for this key,value;
	}while(0);

	do
	{
		//int key;
		//std::string value;
		//dosomething for this key,value;
	}while(0);
}

int main()
{
	test_do_while_1();

	test_do_while_2();

	EMPTYMICRO;

	test_do_while_3();

	system("pause");
	return 0;
}