//前提假设：系统中有很多资源，先将资源分类几个类型，每一类型下又有几个小分类
//具体的小分类下保存最终的文件名；

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;

vector<string> vecResTypes;//资源类型
map< string,vector<string> > mapCategory; //资源类型---资源分类
typedef map< string,vector<string> >::iterator  MapCateIter;
typedef map< string,vector<string> >::const_iterator  CMapCateIter;
//-----------------second method-------------------
map< string, map< string,vector<string> >  > mapTotal;
typedef map< string, map< string,vector<string> >  > ::iterator  MapResIter;
typedef map< string, map< string,vector<string> >  > ::const_iterator  CMapResIter;
void init()
{
	vecResTypes.push_back("Model");
	vecResTypes.push_back("Image");
	vecResTypes.push_back("Video");
	vecResTypes.push_back("Audio");
	vecResTypes.push_back("Other");

	vector<string> category;
	category.push_back("车辆");
	category.push_back("动物");
	category.push_back("植物");
	category.push_back("武器");
	mapCategory["Model"] = category;

	category.clear();
	category.push_back("动物");
	category.push_back("花木");
	mapCategory["Image"] = category;

	category.clear();
	category.push_back("科技");
	category.push_back("人文");
	mapCategory["Video"] = category;

}

void secondinit()
{
	vector<string> vecRess;  //具体的资源名
	map< string,vector<string> > mapCate; //某一分类下的所有资源名
	vector< map< string,vector<string> > > vecType; //某一类型下的所有资源

//---------------------------------------------------------------------
	vecRess.push_back("1.xml");  
	vecRess.push_back("2.xml");
	vecRess.push_back("132.xml");
	mapCate["动物"] = vecRess; //设置一个分类下的所有文件名
	

	vecRess.clear();	
	vecRess.push_back("z1.xml");
	vecRess.push_back("z342.xml");
	vecRess.push_back("zw.xml");
	mapCate["植物"] = vecRess;
	 
	vecRess.clear();	
	vecRess.push_back("35654.xml");
	vecRess.push_back("3dff.xml");
	vecRess.push_back("3hfgh.xml");
	mapCate["武器"] = vecRess;

	mapTotal["Model"] = mapCate;  //设置一个类型下的所有分类
//---------------------------------------------------------------------
	vecRess.clear();
	mapCate.clear();
	vecRess.push_back("555555.xml");  
	vecRess.push_back("666666.xml");
	vecRess.push_back("777777.xml");
	mapCate["动物"] = vecRess; //设置一个分类下的所有文件名


	vecRess.clear();	
	vecRess.push_back("1111.xml");
	vecRess.push_back("2222.xml");
	vecRess.push_back("3333.xml");
	mapCate["花木"] = vecRess;	

	mapTotal["Image"] = mapCate;  //设置一个类型下的所有分类
//---------------------------------------------------------------------
	vecRess.clear();
	mapCate.clear();
	vecRess.push_back("zzzz.xml");  
	vecRess.push_back("aaaa.xml");
	vecRess.push_back("eeee.xml");
	mapCate["科技"] = vecRess; //设置一个分类下的所有文件名


	vecRess.clear();	
	vecRess.push_back("ggggg.xml");
	vecRess.push_back("hhhhh.xml");
	vecRess.push_back("bbbbb.xml");
	mapCate["人文"] = vecRess;	

	mapTotal["Video"] = mapCate;  //设置一个类型下的所有分类
//---------------------------------------------------------------------
	vecRess.clear();
	mapCate.clear();
	vecRess.push_back("678678.xml");  
	vecRess.push_back("7.xml");
	 
	mapCate["流行"] = vecRess; //设置一个分类下的所有文件名	

	mapTotal["Audio"] = mapCate;  //设置一个类型下的所有分类
//---------------------------------------------------------------------
	vecRess.clear();
	mapCate.clear();
	 

	mapTotal["Other"] = mapCate;  //设置一个类型下的所有分类
//---------------------------------------------------------------------
}

bool addResource(const std::string& type,const std::string& category,const std::string& name)
{
	bool bflag = true;
	//std::map< std::string, std::map< std::string,std::vector<std::string> >  > mapTotal; //用于表示本对象包含的资源
	MapResIter iterType = mapTotal.find(type);
	if (iterType!=mapTotal.end())
	{
		MapCateIter iterCate = iterType->second.find(category);
		if(iterCate!=iterType->second.end())
		{
			iterCate->second.push_back(name);
		}
		else
		{
			std::vector<std::string> vecNames;
			vecNames.push_back(name);
			(iterType->second)[category] = vecNames;
		}
	}
	else
	{
		//std::map< std::string,std::vector<std::string> >::iterator  MapCateIter;
		std::map< std::string,std::vector<std::string> > mapCates;
		std::vector<std::string> vecNames;
		vecNames.push_back(name);
		mapCates[category] = vecNames;
		mapTotal[type]  = mapCates; 
	}
	return bflag;
}

string itostr(int i)
{
	std::ostringstream oss;
	std::string str1;

	oss.str( "" );
	oss << i;
	str1 = oss.str();
	return str1;
}
int main()
{
	init();

	vector<string> vecTmp;
	for(int i =0;i<vecResTypes.size();i++)
	{
		cout<<vecResTypes[i]<<":\n";
		MapCateIter iter = mapCategory.find(vecResTypes[i]);
		if(iter!=mapCategory.end() )//有对应的资源分类
		{
			vecTmp = iter->second;
			for(int j=0;j<vecTmp.size();j++)
			{
				cout<<"\t"<<vecTmp[j]<<endl;
			}
		}		
	}

	cout<<"------------------------second method------------------------"<<endl;
	
	//secondinit();
	DWORD starttime;
	//starttime=GetTickCount();

	srand(1);
	int times = 1000;
	for (int i = 1; i< times ; i++)
	{
		addResource(itostr(i),"cate"+itostr(rand()%times),"name"+itostr(i*10)+"");
		addResource(itostr(i),"cate"+itostr(rand()%times),"name"+itostr(i*10)+"");
		addResource(itostr(i),"cate"+itostr(rand()%times),"name"+itostr(i*10)+"");
	}
	int count = mapTotal.size();
//	starttime=GetTickCount();

	_LARGE_INTEGER time_start;    /*开始时间*/ 
	_LARGE_INTEGER time_over;        /*结束时间*/ 
	double dqFreq;                /*计时器频率*/ 
	LARGE_INTEGER f;            /*计时器频率*/ 
	QueryPerformanceFrequency(&f);  
	dqFreq=(double)f.QuadPart;  
	QueryPerformanceCounter(&time_start);  
	Sleep(1000);/*循环耗时*/ 
	 

	map< string,vector<string> > maptmp;
	for (MapResIter iter = mapTotal.begin();iter!=mapTotal.end();iter++)
	{
		cout<<iter->first<<":\n";
		maptmp = iter->second;
		for(MapCateIter cateiter = maptmp.begin();cateiter!=maptmp.end();cateiter++)
		{
			cout<<"\t"<<cateiter->first<<":\n";
			vecTmp = cateiter->second;

			for(vector<string>::iterator veciter = vecTmp.begin();veciter!=vecTmp.end();veciter++)
			{
				cout<<"\t\t"<<*veciter<<endl;
			}
		}
	}
	QueryPerformanceCounter(&time_over);  
	cout<<((time_over.QuadPart-time_start.QuadPart)/dqFreq)<<endl;//单位为秒，精度为1000 000/（cpu主频）微秒 
//	DWORD endtime = GetTickCount();
//	cout<<endtime - starttime<<"ms"<<endl;
	system("pause");
	return 0;
}