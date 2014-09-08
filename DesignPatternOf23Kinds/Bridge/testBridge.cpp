#include "Abstraction.h" 
#include "AbstractionImp.h" 
#include <iostream> 
using namespace std; 
int main(int argc,char* argv[]) 
{ 
	AbstractionImp* impA = new ConcreteAbstractionImpA(); 
	Abstraction* absa = new RefinedAbstraction(impA); 
	absa->Operation(); 

	AbstractionImp* impB = new ConcreteAbstractionImpB(); 
	Abstraction* absb = new RefinedAbstraction(impB); 
	absb->Operation(); 

	system("pause");
	return 0; 
} 