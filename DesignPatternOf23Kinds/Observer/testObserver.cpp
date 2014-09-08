#include "Subject.h" 
#include "Observer.h" 

#include <iostream> 
using namespace std; 
int main(int argc,char* argv[]) 
{ 
	ConcreteSubject* sub = new ConcreteSubject(); 
	Observer* o1 = new ConcreteObserverA(sub); //ConcreteObserverA一对象开始监视ConcreteSubject
	Observer* o2 = new ConcreteObserverB(sub); //ConcreteObserverB一对象开始监视ConcreteSubject
	sub->SetState("old"); //ConcreteSubject(主题，目标)发生变化，改变数据
	sub->Notify(); // 下发通知，让所有的观察者发生相应变化
	sub->SetState("new"); //也可以由 Observer调用 ,//ConcreteSubject(主题，目标)发生变化，改变数据
	sub->Notify();  // 下发通知，让所有的观察者发生相应变化

	system("pause");
	return 0; 
} 