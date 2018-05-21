#include <iostream>
#include <assert.h>
#include "booleanindexer.h"

using namespace std;
using namespace booleonindex;


int main()
{
    BooleanIndexer boolindexer;
    boolindexer.Init();

    boolindexer.AddDoc(0, "ad0",  "(region in{ ShangHai, Beijing } and age not in{ 3, 4 })");
    boolindexer.AddDoc(1, "ad1", "(region in {ShenZhen, ShangHai}) or (age not in {4, 6})");
    boolindexer.AddDoc(2, "ad2", "(region in {ShangHai, NanJing} and age not in {3, 5, 6})");
    boolindexer.AddDoc(3, "ad3",  "(region in {ChengDu, Beijing, WuHan}) or (age not in {4, 3})");
    boolindexer.AddDoc(4, "ad4", "(age not in {3, 4})");
    boolindexer.AddDoc(6, "ad6", "(region in {ShangHai} and age not in {3, 5, 6} and gender in {men} )");

    //boolindexer.Dump();
    boolindexer.BeforeSearch();

    {
        vector<Term>   vec;
        Term term1( "region", "WuHan");
        vec.push_back(term1);

        Term term2("age", "6");
        vec.push_back(term2);

        set<int> docs;
        boolindexer.Search(vec, docs);

        assert(docs.size() == 1);
        assert(*(docs.begin()) == 3);
        cout << "test case 1 ok"<< endl;
    }


    {
        vector<Term>   vec;
        Term term1("region", "ShangHai");
        vec.push_back(term1);
        Term term2("age", "6");
        vec.push_back(term2);
        set<int> docs;
        boolindexer.Search(vec, docs);
        assert(docs.size() == 2);
       
        cout << "test case 2 ok" << endl;
    }



    {
        vector<Term>   vec;
        Term term1("region", "ShangHai");
        vec.push_back(term1);
        Term term2("age", "4");
        vec.push_back(term2);
        Term term3("gender", "men");
        vec.push_back(term3);
        set<int> docs;
        boolindexer.Search(vec, docs);
        assert(docs.size() == 3);

        cout << "test case 3 ok" << endl;
    }



    return 0;
}
