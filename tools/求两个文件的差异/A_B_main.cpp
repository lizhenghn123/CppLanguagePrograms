#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <Windows.h>
#pragma warning(disable : 4996)
using namespace std;

//两个文本文件file1、file2，求file1出有但file2中没有的文本行，以行为单位

struct icompare
{
    bool operator ()(const std::string& left, const std::string& right) const
    {
        return strcmp(left.c_str(), right.c_str()) < 0;
    }
};

struct icompare_nocase
{
    bool operator ()(const std::string& left, const std::string& right) const
    {
        return stricmp(left.c_str(), right.c_str()) < 0;
    }
};

//--------------------------------Method 1--------------------------------//
int Cpp_Impl_1(const char *file1, const char *file2, bool is_case = false)
{
    int ignore_case = is_case;
    std::fstream fl, fp;
    try
    {
        fp.open(file1, std::ios::in | std::ios::binary);
    }
    catch(std::exception&)
    {
        std::cerr << "Can not open" << file1 << std::endl;
        return 1;
    }
    try
    {
        fl.open(file2, std::ios::in | std::ios::binary);
    }
    catch(std::exception&)
    {
        std::cerr << "Can not open" << file2 << std::endl;
        return 1;
    }
    std::vector<std::string> buf;
    while(!fl.eof())
    {
        std::string ln;
        if(std::getline(fl, ln).rdstate())
            break;
        std::replace_if(ln.begin(), ln.end(), [ = ](int c)
        {
            return c == '\n' || c == ' ';
        }, 0);
        buf.push_back(std::forward<std::string>(ln));
    }
    fl.close();
    struct icompare_nocase icom;
    if (ignore_case)
        std::sort(buf.begin(), buf.end(), icom);
    else
        std::sort(buf.begin(), buf.end(), icom);
    while(!fp.eof())
    {
        std::string ln;
        if(std::getline(fp, ln).rdstate())
            break;
        std::replace_if(ln.begin(), ln.end(), [ = ](int c)
        {
            return c == '\n' || c == ' ';
        }, 0);
        if (ignore_case)
        {
            if(!std::binary_search(buf.begin(), buf.end(), ln, struct icompare_nocase()))
                std::cout << ln << std::endl;
        }
        else
        {
            if(!std::binary_search(buf.begin(), buf.end(), ln))
                std::cout << ln << std::endl;
        }
    }
    fp.close();
    return 0;
}

//--------------------------------Method 2--------------------------------//
// 加载文件内容到vector容器
bool LoadStringFile_Vector(const std::string& filename, std::vector<std::string>& vector_load)
{
    try
    {
        std::fstream fs(filename, std::ios::in | std::ios::binary);
        std::string line;
        while(std::getline(fs, line))
        {
            vector_load.push_back(std::move(line));
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return false ;
    }
    return true ;
}// End LoadStringFile_Vector()

// 加载文件内容到set容器
bool LoadStringFile_Set(const std::string& filename, std::set<std::string>& set_load)
{
    try
    {
        std::fstream fs(filename, std::ios::in | std::ios::binary);
        std::string line;
        while(std::getline(fs, line))
        {
            set_load.insert(line) ;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return false ;
    }
    return true ;
}// End LoadStringFile_Set()

int Cpp_Impl_2(const char *file1, const char *file2, bool is_case = false)
{
    {
        std::vector<std::string> Vec_File_A ;
        std::vector<std::string> Vec_File_B ;
        std::vector<std::string> Vec_File_Diff   ;
        LoadStringFile_Vector(file1, Vec_File_A) ;
        LoadStringFile_Vector(file2, Vec_File_B) ;
        //vector 差集
        std::sort(Vec_File_A.begin(), Vec_File_A.end());
        std::sort(Vec_File_B.begin(), Vec_File_B.end());
        std::set_difference(Vec_File_A.begin(), Vec_File_A.end(),
                            Vec_File_B.begin(), Vec_File_B.end(),
                            std::back_inserter(Vec_File_Diff));
    }
    {
        std::set<std::string> Set_File_A ;
        std::set<std::string> Set_File_B ;
        std::set<std::string> Set_File_Diff;
        LoadStringFile_Set(file1, Set_File_A) ;
        LoadStringFile_Set(file2, Set_File_B) ;
        /** Set 差集**/
        std::set_difference(Set_File_A.begin(), Set_File_A.end(),
                            Set_File_B.begin(), Set_File_B.end(),
                            std::insert_iterator< std::set<std::string> >(Set_File_Diff, Set_File_Diff.begin()));
        for (std::set<std::string>::iterator itr = Set_File_Diff.begin(); itr != Set_File_Diff.end(); ++itr)
        {
            std::cout << *itr << "\n";
        }
        return Set_File_Diff.size();
    }
    return 0;
}

//--------------------------------Method 3--------------------------------//
int Cpp_Impl_3(const char *file1, const char *file2, bool is_case = false)
{
    bool ignore_case = is_case;
    std::ifstream fprog(file1);
    std::ifstream flist(file2);
    std::vector<std::string> listLines;
    std::string line;
    while (std::getline(flist, line))
        listLines.push_back(line);
    if (ignore_case)
        std::sort(listLines.begin(), listLines.end(), struct icompare());
    else
        std::sort(listLines.begin(), listLines.end());
    while (std::getline(fprog, line))
    {
        if (ignore_case)
        {
            if (!std::binary_search(listLines.begin(), listLines.end(), line, struct icompare()))
                std::cout << line << std::endl;
        }
        else
        {
            if (!std::binary_search(listLines.begin(), listLines.end(), line))
                std::cout << line << std::endl;
        }
    }
    return 1;
}

int main()
{
    Cpp_Impl_1("more.txt", "less.txt", false);
    std::cout << "----------------------------------\n";
    Cpp_Impl_2("more.txt", "less.txt", false);
    std::cout << "----------------------------------\n";
    Cpp_Impl_3("more.txt", "less.txt", false);
    std::cout << "----------------------------------\n";
    system("pause");
    return 0;
}