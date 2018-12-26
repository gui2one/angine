#ifndef UI_EVENT_H
#define UI_EVENT_H

#include <typeinfo>
template<typename T> 
class UIEvent
{
	public:
		
	inline void print(){ std::cout << "hello event ::::::::::::::::::::::::";}
	inline void getType(){
		std::cout << "Type is : " << typeid(T).name() << "\n";
	}
};



class UIEventItem{
	public : 
		std::string name;
};

class MouseEvent : UIEventItem
{
	public:
		double xpos, ypox;
};

#endif 
