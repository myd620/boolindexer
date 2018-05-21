#include "booleanindexer.h"

using namespace std;

namespace booleonindex{

    BooleanIndexer::BooleanIndexer()
    {

    }


    BooleanIndexer::~BooleanIndexer()
    {

    }

    int BooleanIndexer::Init()
    {
        Term term("", "");
        term.id_ = 0;
        terms_.push_back(term);

        assigns_.clear();
        conjs_.clear();
        docs_.clear();
        conjRevsIndex_.clear();
        docRevsIndex_.clear();

        MapTermtoConj tmp;
        conjRevsIndex_[0] = tmp;
        return 0;
    }



    int BooleanIndexer::AddDoc(int docid, const string &des, const string &content)
    {
        int ret = 0;
        if (content.length() == 0)
            return -1;

        auto itor = find_if(docs_.begin(), docs_.end(), [docid](Doc &doc) {return  (doc.id_ == docid) ? true : false; });
        if (itor != docs_.end())
            return -1;

        Doc doc(docid,des, content);
        vector<string> dnfs;

        ret = getDnfExpressFromDoc(content, dnfs);
        if (ret < 0)
            return ret;

        for (vector<string>::iterator itor = dnfs.begin(); itor != dnfs.end(); itor++)
        {
            //todo £¬ first to find this dnf is in the conjs_. if ,then add <conjid, docid >  to docRevsIndex_

            //add a new conj
            Conj conj;
            conj.conjid_ = conjs_.size();

            vector<string> assignlist;
            vector<Assign> assigns;
            ret = getConjFromDnf(*itor, assignlist);
            if (ret < 0)
                continue;

            for (auto itor2 = assignlist.begin(); itor2 != assignlist.end(); itor2++)
            {
                bool belong = false ;
                vector<Term> termlist;
                getTermsFormAssign(*itor2, termlist, belong);

                Assign assign;
                assign.belong_ = belong;

                for (auto itor3 = termlist.begin(); itor3 != termlist.end(); itor3++)
                {
                    auto itor4 = find_if(terms_.begin(), terms_.end(), TermCmp(*itor3));
                    if (itor4 == terms_.end())
                    {
                        itor3->id_ = terms_.size();
                        terms_.push_back(*itor3);
                    }
                    else
                    {
                        itor3->id_ = itor4->id_;
                    }
                    assign.termlist_.push_back(itor3->id_);
                }

                if (belong == true)
                {
                    conj.assignnum_++;
                }

                //first find assign in  assigns_
                assign.id_ = assigns_.size();
                assigns.push_back(assign);
                assigns_.push_back(assign);
                conj.assignlist_.insert(assign.id_);
            }

            //add conj to conjRevsIndex_
            AddConjToRevsIndex(assigns, conj);

            //add conj to conjs_
            conjs_.push_back(conj);

            //add <conjid -- docid> to  docRevsIndex_
            docRevsIndex_[conj.conjid_].push_back(doc.id_);
        }


        docs_.push_back(doc);
        return 0;
    }




    int BooleanIndexer::AddConjToRevsIndex(vector<Assign> &assigns, Conj &conj)
    {
        for (auto itor = assigns.begin(); itor != assigns.end(); itor++)
        {
            for (int iloop = 0; iloop < itor->termlist_.size(); iloop++)
            {
                int termid = itor->termlist_[iloop];
                if (conjRevsIndex_.find(conj.assignnum_) != conjRevsIndex_.end())
                {
                    MapTermtoConj &tmp = conjRevsIndex_[conj.assignnum_];
                    tmp[termid].push_back(make_pair(conj.conjid_, itor->belong_));
                }
                else
                {
                    MapTermtoConj tmp;
                    tmp[termid].push_back(make_pair(conj.conjid_, itor->belong_));
                    conjRevsIndex_[conj.assignnum_] = tmp;
                }
            }
        }

        // if a conj has 0 assignment, then put it into Revs[0] with <conjid, true>
        if (conj.assignnum_ == 0)
        {
            MapTermtoConj &tmp = conjRevsIndex_[conj.assignnum_];
            tmp[0].push_back(make_pair(conj.conjid_, true));
        }
        return 0;
    }


    int BooleanIndexer::BeforeSearch()
    {
        sort(terms_.begin(), terms_.end(), TermCmpByKV());
        counter_.setMaxCounterSize(conjs_.size());
        return 0;
    }

    int BooleanIndexer::Search(vector<Term> &cond, set<int> &docs)
    {
        int assignsize = cond.size();

        counter_.Reset();
        for (int iloop = assignsize; iloop >= 0; iloop--)
        {
            if (conjRevsIndex_.rbegin()->first < iloop || conjRevsIndex_.find(iloop) == conjRevsIndex_.end())
            {
                continue;
            }

            MapTermtoConj &tmp = conjRevsIndex_[iloop];
            for (int j = 0; j < assignsize; j++)
            {
                int termid = GetTermIdbyKV(cond[j]);
                if (termid != -1 && tmp.find(termid) != tmp.end())
                {
                    auto &vec = tmp[termid];
                    for (int k = 0; k < vec.size(); k++)
                    {
                        if (vec[k].second == true)
                        {
                            counter_.addConjCountById(vec[k].first);
                        }
                        else
                        {
                            counter_.addBlackConjById(vec[k].first);
                        }
                    }
                }
            }

            if (iloop == 0)
            {
                auto &vec = tmp[0];
                for (int k = 0; k < vec.size(); k++)
                {
                    counter_.addConjCountById(vec[k].first);
                }
            }
        }


        return GetDocsFromCounter(docs);

    }


    int BooleanIndexer::GetTermIdbyKV(Term &term)
    {
        auto itor = lower_bound(terms_.begin(), terms_.end(), term, TermCmpByKV());
        if (itor == terms_.end() || itor->equal(term) == false)
        {
            return -1;
        }
        return itor->id_;
    }

    int BooleanIndexer::GetDocsFromCounter(set<int> &docs)
    {

        for (int conjid = 0; conjid < conjs_.size(); conjid++)
        {
            int num = 0;
            bool black = false;
            counter_.getConjidNumById(conjid, num, black);

            if ((black == false) && (num == conjs_[conjid].assignnum_))
            {
                docs.insert(docRevsIndex_[conjid].begin(), docRevsIndex_[conjid].end());
            }
        }
        return 0;
    }

    int BooleanIndexer::Dump()
    {
        cout << "term info :" << endl;
        for (int i = 0; i < terms_.size(); i++)
        {
            cout << "id :" << terms_[i].id_ << " key :" <<  terms_[i].key_ << " value :" << terms_[i].value_ << endl;
        }

        cout << "assign info :" << endl;
        for (int i = 0; i < assigns_.size(); i++)
        {
            cout << "id :" << assigns_[i].id_ << " belong : " << assigns_[i].belong_ << " termlist :";
            for (int j = 0; j < assigns_[i].termlist_.size(); j++)
            {
                cout << assigns_[i].termlist_[j] << "  ";
            }
            cout << endl;
        }


        cout << "conj info :" << endl;
        for (int i = 0; i < conjs_.size(); i++)
        {
            cout << "id :" << conjs_[i].conjid_ << " assign num:" << conjs_[i].assignnum_ ;
            //for (int j = 0; j < conjs_[i].assignlist.size(); j++)
            //{
            //    cout << conjs_[i].assignlist[j] << "  " << endl;
            //}
            cout << endl;
        }


        cout << "doc info :" << endl;
        for (int i = 0; i < docs_.size(); i++)
        {
            cout << "id : " << docs_[i].id_ << " conjlist  ";
            for (auto itor = docs_[i].conjlist.begin(); itor != docs_[i].conjlist.end(); itor++)
            {
                cout << *itor << " ";
            }
            cout << endl;
        }

        cout << "conj map" << endl;
        for (auto itor = conjRevsIndex_.begin(); itor != conjRevsIndex_.end(); itor++)
        {
            cout << "term size : " << itor->first << endl;

            for (auto itor2 = itor->second.begin(); itor2 != itor->second.end(); itor2++)
            {
                cout << "term id : " << itor2->first << " ";
                for (auto itor3 = itor2->second.begin(); itor3 != itor2->second.end(); itor3++)
                {
                    cout << itor3->first << ":" << itor3->second << ",";
                }
                cout << endl;
            }
           
        }

        return 0;
    
    }

    int BooleanIndexer::LoadIndex(string filename)
    {
        return 0;
    }

    int BooleanIndexer::DumpTofile(string filename)
    {
        return 0;
    }

}


