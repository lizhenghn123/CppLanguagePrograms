#include "Factory.h" 
#include "Product.h" 
#include <iostream> 
using namespace std; 


int main(int argc,char* argv[]) 
{ 
	Factory* fac = new ConcreteFactory(); 
	Product* p = fac->CreateProduct(); 

	//在ConcreteProduct类中添加一个非接口中的方法，Product类指针肯定不能直接访问。
	//但是可以使用向下类型转换成具体子类ConcreteProduct的指针，，就可以访问ConcreteProduct的所有成员。
	// 但向下类型转换并不总是能够成功，也并不安全。最好还是将接口提取到抽象类Product中.lizheng 20111110 自我理解
	ConcreteProduct* cp = dynamic_cast<ConcreteProduct*>(p);
	cp->test();
	system("pause");
	return 0; 
} 