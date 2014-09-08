#ifndef _PRODUCT_H_ 
#define _PRODUCT_H_ 

#include <iostream> 
using namespace std;

class Product 
{ 
public: 
	Product()
	{
		ProducePart(); 
		cout<<"return a product"<<endl; 
	}
	~Product(){}
	void ProducePart()
	{
		cout<<"build part of product.."<<endl;
	}
 
}; 


class ProductPart 
{ 
public: 
	ProductPart(){}
	~ProductPart(){} 
	ProductPart* BuildPart()
	{
		return new ProductPart; 
	} 
}; 


#endif //~_PRODUCT_H_ 