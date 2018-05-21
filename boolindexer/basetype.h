#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include <iostream>
#include <vector>
#include <set>


using namespace std;

namespace booleonindex{

#define MAX_ASSIGN_NUM 0xFF

    struct Term
    {
        int  id_;
        string key_;
        string value_;

        Term(string key, string value) :
            key_(key), value_(value)
        {
        }

        bool equal(Term &term)
        {
            return this->key_ == term.key_ && this->value_ == term.value_;
        }
    };



    class TermCmp {
    private:
        Term &term_;
    public:
        TermCmp(Term &term) :
            term_(term)
        {
        }

        bool operator () (Term& rh)
        {
            return term_.key_ == rh.key_ && term_.value_ == rh.value_;
        }
    };


    class TermCmpByKV
    {
    public:
        bool operator () (const Term &lh,const Term &rh)
        {
            if (lh.key_ == rh.key_)
            {
                return lh.value_ < rh.value_;
            }
            else
            {
                return lh.key_ < rh.key_;
            }
        }
    };




    struct Assign
    {
        int  id_;
        bool belong_;
        vector<int> termlist_;

        Assign() :
            id_(0), belong_(false)
        {
        }

        bool equal(const Assign &Assign)const
        {
            if (this->belong_ != Assign.belong_ || this->termlist_.size() != Assign.termlist_.size())
            {
                return false;
            }

            auto itor1 = termlist_.begin();
            auto itor2 = Assign.termlist_.begin();
            for (; itor1 != termlist_.end() && itor2 != Assign.termlist_.end(); itor1++, itor2++)
            {
                if (*itor1 != *itor2)
                {
                    return false;
                }
            }
            return true;
        }


    };




    struct Conj
    {
        int conjid_;
        int assignnum_;   // 正向条件数目
        //string context;  //不同通过字符描述来判断是否相等
        set<int> assignlist_;

        Conj():
            conjid_(0), assignnum_(0)
        {
        }

        bool equal(const Conj &conj)
        {
            if (this->assignnum_ != conj.assignnum_)
            {
                return false;
            }
            auto itor1 = assignlist_.begin();
            auto itor2 = conj.assignlist_.begin();
            for (; itor1 != assignlist_.end() && itor2 != conj.assignlist_.end(); itor1++, itor2++)
            {
                if (*itor1 != *itor2)
                {
                    return false;
                }
            }
            return true;
        }

        bool operator < (const Conj &conj)const
        {
            if (assignnum_ < conj.assignnum_)
            {
                return true;
            }
            auto itor1 = assignlist_.begin();
            auto itor2 = conj.assignlist_.begin();
            for (; itor1 != assignlist_.end() && itor2 != conj.assignlist_.end(); itor1++, itor2++)
            {
                if (*itor1 < *itor2)
                {
                    return true;
                }
                else if (*itor1 == *itor2)
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }

    };



    struct Doc
    {
        int id_;
        string des_;
        string content_;
        set<int> conjlist;

        Doc() :
            id_(0)
        {
        }


        Doc(int id,  const string &des  ,const string &content) :
            id_(id),
            des_(des),
            content_(content)
        {

        }
    };

}

#endif
