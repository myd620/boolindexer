#ifndef _BOOLEAN_INDEXER_H_
#define _BOOLEAN_INDEXER_H_
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <algorithm>


#include "basetype.h"
#include "text_parser.h"
#include "counter.h"

using namespace std;

namespace booleonindex{

    typedef  map<int, vector< pair<int, bool> > >  MapTermtoConj;

    class BooleanIndexer
    {
    private:
        vector<Term>  terms_;

        vector<Assign>  assigns_;

        vector<Conj>   conjs_;

        vector<Doc>   docs_;

        //term对应conj
        map<int, MapTermtoConj >   conjRevsIndex_;

        //conj对应的doc
        map<int, vector<int> > docRevsIndex_;


        Counter counter_;

    public:
        BooleanIndexer();

        ~BooleanIndexer();

        int Init();

        int AddConjToRevsIndex(vector<Assign> &assigns, Conj &conj);

        int AddDoc(int docid, const string &des, const string &content);

        int Search(vector<Term> &cond, set<int> &docs);

        int GetTermIdbyKV(Term &term);

        int GetDocsFromCounter(set<int> &docs);

        int BeforeSearch();

        int LoadIndex(string filename);

        int DumpTofile(string filename);

        int Dump();
    };


}
#endif
