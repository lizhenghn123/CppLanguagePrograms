#include "Memento.h" 
#include <iostream> 
using namespace std; 
int main(int argc,char* argv[]) 
{ 
	Originator* o = new Originator(); 
	o->SetState("old"); //±¸ÍüÇ°×´Ì¬ 
	o->PrintState(); 
	Memento* m = o->CreateMemento(); //½«×´Ì¬±¸Íü 
	o->SetState("new"); //ÐÞ¸Ä×´Ì¬ 
	o->PrintState(); 
	o->RestoreToMemento(m);           //»Ö¸´ÐÞ¸ÄÇ°×´Ì¬ 
	o->PrintState(); 

	system("pause");
	return 0; 
}