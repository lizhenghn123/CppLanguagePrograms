#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

namespace DynamicPoly
{
	class Geometry
	{
	public:
		virtual void Draw()const = 0;
	};

	class Line : public Geometry
	{
	public:
		virtual void Draw()const
		{
			std::cout << "Line Draw()\n";
		}
	};

	class Circle : public Geometry
	{
	public:
		virtual void Draw()const
		{
			std::cout << "Circle Draw()\n";
		}
	};

	class Rectangle : public Geometry
	{
	public:
		virtual void Draw()const
		{
			std::cout << "Rectangle Draw()\n";
		}
	};

	void DrawGeometry(const Geometry *geo)
	{
		geo->Draw();
	}

	//动态多态最吸引人之处在于处理异质对象集合的能力
	void DrawGeometry(std::vector<DynamicPoly::Geometry*> vecGeo)
	{
		const size_t size = vecGeo.size();
		for(size_t i = 0; i < size; ++i)
			vecGeo[i]->Draw();
	}

}

namespace StaticPoly
{
	class Line
	{
	public:
		void Draw()const
		{
			std::cout << "Line Draw()\n";
		}
	};

	class Circle
	{
	public:
		void Draw(const char* name=NULL)const
		{
			std::cout << "Circle Draw()\n";
		}
	};

	class Rectangle
	{
	public:
		void Draw(int i = 0)const
		{
			std::cout << "Rectangle Draw()\n";
		}
	};

	template<typename Geometry>
	void DrawGeometry(const Geometry& geo)
	{
		geo.Draw();
	}

	template<typename Geometry>
	void DrawGeometry(std::vector<Geometry> vecGeo)
	{
		const size_t size = vecGeo.size();
		for(size_t i = 0; i < size; ++i)
			vecGeo[i].Draw();
	}
}

void test_dynamic_polymorphism()
{
	DynamicPoly::Line line;
	DynamicPoly::Circle circle;
	DynamicPoly::Rectangle rect;
	DrawGeometry(&circle);

	std::vector<DynamicPoly::Geometry*> vec;
	vec.push_back(&line);
	vec.push_back(&circle);
	vec.push_back(&rect);
}

void test_static_polymorphism()
{
	StaticPoly::Line line;
	StaticPoly::Circle circle;
	StaticPoly::Rectangle rect;
	DrawGeometry(circle);

	std::vector<StaticPoly::Line> vecLines;
	StaticPoly::Line line2;
	StaticPoly::Line line3;
	vecLines.push_back(line);
	vecLines.push_back(line2);
	vecLines.push_back(line3);
	//vecLines.push_back(&circle); //编译错误，已不再能够处理异质对象
	//vecLines.push_back(&rect);	//编译错误，已不再能够处理异质对象
	DrawGeometry(vecLines);

	std::vector<StaticPoly::Circle> vecCircles;
	vecCircles.push_back(circle);
	DrawGeometry(circle);
}

/**无法编译通过，因此Widget要求有显式接口，但现在看不到*/
//void DoSomething(Widget& w)
//{
//	if( w.size() > 0 && w != someNastyWidget)
//	{
//		Widget temp(w);
//		temp.normalize();
//		temp.swap(w);
//	}
//}

/**可以编译通过，因此此处只是要求了只有在模板具现时需保证下面可编译（无调用，无具现）*/
template<typename Widget,typename Other>
void DoSomething(Widget& w, const Other& someNasty)
{
	if( w.size() > 0 && w != someNasty) //someNastyT可能是是T类型的某一实例，也可能不是
	{
		Widget temp(w);
		temp.normalize();
		temp.swap(w);
	}
}

void foo(int *){}

int main(void)
{
	//foo(NULL);
	foo(nullptr);

	int *i = nullptr;
	//system("pause");
	return 0;
}