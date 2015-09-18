#include <iostream>
#include <string>
#include "xml/pugixml/pugixml.hpp"
using namespace std;
using namespace pugi;

// see http://pugixml.org/docs/quickstart.html

//更新xml，要求xml_file文件必须存在， name键也必须存在，否则更新失败
void updateXmlElement(const char* xml_file, const char* name, const char* value)
{
    cout << "------------------ " << updateXmlElement << "-------------------\n";
    xml_document doc;
    xml_parse_result result = doc.load_file(xml_file);
    if (result.status != xml_parse_status::status_ok)
    {
        printf("loaf file [%s] failure[%d]\n", xml_file, result.status);
        return;
    }

    xml_node node = doc.child("root").child(name);
    node.text().set(value);

    doc.save_file(xml_file);
}

void read(const char* xml_file)
{
    cout << "------------------- read " << xml_file << "-------------------\n";
    xml_document doc;
    xml_parse_result result = doc.load_file(xml_file);
    if (!doc.load_file(xml_file))
    {
        return;
    }
    {
        xml_node node = doc.child("root").child("ip");
        string ip = node.first_child().value();
        cout << ip << "\n";
        node.first_child().text().set("10.10.10.10");
    }
    {
        xml_node node = doc.child("root").child("list");
        cout << node.first_child().value() << "\n";
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
        {
            std::cout << attr.name() << "=" << attr.value() << "\n";
        }

        xml_node node2 = node.child("desc");
        cout << node2.first_child().value() << "\n";
    }
}

// see http://blog.csdn.net/clever101/article/details/7521603
void read1()
{
    cout << "------------------ " << "-------------------\n";
    pugi::xml_document doc;
    xml_parse_result result = doc.load_file("1.xml");
    if (result.status != xml_parse_status::status_ok)
    {
        printf("loaf file [%s] failure[%d]\n", "1.xml", result.status);
        return ;
    }

    pugi::xml_node tools = doc.child("Profile").child("Tools");

    // 遍历属性
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
    {
        std::cout << "Tool:";

        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
        {
            std::cout << " " << attr.name() << "=" << attr.value();
        }

        std::cout << std::endl;
    }
}

// xpath 路径解析 http://blog.csdn.net/yukin_xue/article/details/7540011
void read2()
{
    cout << "------------------ " << "-------------------\n";
    pugi::xml_document doc;
    xml_parse_result result = doc.load_file("2.xml");
    if (result.status != xml_parse_status::status_ok)
    {
        printf("loaf file [%s] failure[%d]\n", "2.xml", result.status);
        return;
    }

    std::string strPOIRoot = "/root/list/poi";
    std::string strPOIID = "pguid";
    std::string strPOINam = "name";

    pugi::xpath_node_set tools = doc.select_nodes(strPOIRoot.c_str());
    for (pugi::xpath_node_set::const_iterator it = tools.begin(); it != tools.end(); ++it)
    {
        pugi::xpath_node node = *it;
        string strPOI = node.node().child_value(strPOIID.c_str());
        string strName = node.node().child_value(strPOINam.c_str());
        cout << strPOI << "\t" << strName << "\n";
    }
}

void read_grammar()
{
    cout << "-----------------------------------\n";
    pugi::xml_document doc;
    if (!doc.load_file("grammar.xml"))
    {
        cout << "cannot load file grammar.xml\n";
        return ;
    }

    pugi::xml_node rules = doc.child("grammar");
    std::cout << "grammar: \n";

    for (pugi::xml_node rule = rules.first_child(); rule; rule = rule.next_sibling())
    {
        std::cout << "  rule: " << rule.attribute("id").value();

        // 输出所有的属性,如果有的话
        //for (pugi::xml_attribute attr = rule.first_attribute(); attr; attr = attr.next_attribute())
        //{
        //    std::cout << "  " << attr.name() << "=" << attr.value();
        //}
        pugi::xpath_node_set items = rule.select_nodes("one-of/item");
        cout << "  item_size : " << items.size() << "\n";
        for (pugi::xpath_node_set::const_iterator it = items.begin(); it != items.end(); ++it)
        {
            pugi::xpath_node node = *it;
            if (rule.attribute("id").value() == std::string("service") || rule.attribute("id").value() == std::string("keyword"))
            {
                cout << "      " << node.node().first_child().attribute("uri").value() << "\n";
            }
            else
            {
                cout << "      " << node.node().first_child().value() << "\n";

            }
        }
    }
}

int main()
{
    //updateXmlElement("server.xml", "ip", "127.70.55.7");
    //updateXmlElement("server.xml", "name", "中国");

    //read("server.xml");

    //read2();

    read_grammar();

    system("pause");
}