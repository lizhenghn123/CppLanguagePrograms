#include "AdapterOfClass.h" 
#include "AdapterOfObject.h"
#include <iostream> 
using namespace std; 


int main(int argc,char* argv[]) 
{ 
	//类模式的Adapter实现
	//Adapter* adt = new Adapter(); 
	TargetOfClass* adt = new AdapterOfClass(); 
	adt->Request(); 

	//对象模式的Adapter实现
	AdapteeOfObject* ade2 = new AdapteeOfObject; 
	TargetOfObject* adt2 = new AdapterOfObject(ade2); 
	adt2->Request(); 

	system("pause");
	return 0; 
} 
