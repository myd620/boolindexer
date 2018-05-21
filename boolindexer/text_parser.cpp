#include "text_parser.h"


namespace booleonindex{

    std::string& trim(std::string &s)
    {
        if (s.empty())
        {
            return s;
        }

        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }


    int splitstr(const string &src, const string &sep, vector<string> &out)
    {

        if (src.length() == 0 || sep.length() == 0)
        {
            return -1;
        }

        std::size_t pos = 0;
        std::size_t found = 0;
        for (;;)
        {
            found = src.find(sep, pos);
            if (found == std::string::npos)
            {
                string tmp = src.substr(pos);
                tmp = trim(tmp);
                out.push_back(tmp);
                break;
            }
            else
            {
                string tmp = src.substr(pos, found - pos);
                tmp = trim(tmp);
                out.push_back(tmp);
                pos = found + sep.length();
            }
        }

        return 0;
    }


    int getDnfExpressFromDoc(const string &doc, vector<string> &dnfs)
    {
        splitstr(doc, "or", dnfs);
        return dnfs.size();
    }


    int getConjFromDnf(string dnf, vector<string> &assignlist)
    {
        std::size_t pos = 0;
        pos = dnf.find_first_of("(");
        if (pos != string::npos)
        {
            dnf = dnf.erase(0, pos+1);
        }
       
        pos = dnf.find_last_of(")");
        if (pos != string::npos)
        {
            dnf = dnf.erase( pos );
        }

        splitstr(dnf, "and", assignlist);
        return assignlist.size();
    }


    int getTermsFormAssign(string assign, vector<Term> &termlist, bool  &belong)
    {
        // bool belong ;
        string key;
        string value;
        vector<string> values;
        std::size_t sep = 0;
        sep = assign.find("not in");
        if (sep != std::string::npos)
        {
            belong = false;
            key = assign.substr(0, sep);
            key = trim(key);
            value = assign.substr(sep + 6);
            value = trim(value);
        }
        else
        {
            sep = assign.find("in");
            if (sep == std::string::npos)
            {
                return -1;
            }
            belong = true;
            key = assign.substr(0, sep);
            key = trim(key);
            value = assign.substr(sep + 2);
            value = trim(value);
        }

        //remove '{' '}' in key
        value = value.erase(0, value.find_first_not_of("{"));
        value = value.erase(value.find_last_not_of("}") + 1);
        splitstr(value, ",", values);

        for (int iloop = 0; iloop < values.size(); iloop++)
        {
            Term tmp(key, values[iloop]);
            termlist.push_back(tmp);
        }
        return 0;
    }

}
