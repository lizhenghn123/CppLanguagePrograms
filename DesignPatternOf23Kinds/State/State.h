/************************************************************************/
/* State 模式主要是要适应对象对于状态改变时的不同处理策略的实现.
   State模式很好地实现了对象的状态逻辑和动作实现的分离，状态逻辑分布在 State 的派生
   类中实现，而动作实现则可以放在 Context 类中实现（这也是为什么 State 派生类需要拥
有一个指向 Context 的指针） 。这使得两者的变化相互独立，改变 State 的状态逻辑可以很
容易复用 Context 的动作， 也可以在不影响 State 派生类的前提下创建 Context的子类来更
改或替换动作实现。 
   State模式问题主要是逻辑分散化，状态逻辑分布到了很多的 State 的子类中，很难看到整
个的状态逻辑图，这也带来了代码的维护问题。 
   State 模式在实现中，有两个关键点： 
   1）将 State声明为 Context 的友元类（friend class） ，其作用是让 State 模式访问
Context的 protected接口 ChangeSate（） 。 
   2）State 及其子类中的操作都将 Context*传入作为参数，其主要目的是 State 类可以通
过这个指针调用 Context中的方法 （在本示例代码中没有体现） 。 这也是 State 模式和 
Strategy模式的最大区别所在。                */
/************************************************************************/
//state.h 
#ifndef _STATE_H_ 
#define _STATE_H_ 
class Context; //前置声明 


class State 
{ 
public: 
	State(); 
	virtual ~State(); 
	virtual void OperationInterface(Context* ) = 0; 
	virtual void OperationChangeState(Context*) = 0; 
protected: 
	bool ChangeState(Context* con,State* st); 
private: 
	//bool ChangeState(Context* con,State* st); 
}; 


class ConcreteStateA:public State 
{ 
public: 
	ConcreteStateA(); 
	virtual ~ConcreteStateA(); 
	virtual void OperationInterface(Context* ); 
	virtual void OperationChangeState(Context*); 
protected: 
private: 
}; 


class ConcreteStateB:public State 
{ 
public: 
	ConcreteStateB(); 
	virtual ~ConcreteStateB(); 
	virtual void OperationInterface(Context* ); 
	virtual void OperationChangeState(Context*); 
protected: 
private: 
}; 


#endif //~_STATE_H_ 
