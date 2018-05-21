#ifndef _TEXT_PARSER_H_
#define _TEXT_PARSER_H_

#include "basetype.h"


namespace booleonindex{

   
   int getDnfExpressFromDoc(const string &doc, vector<string> &dnfs);
   
   int getConjFromDnf(const string dnf, vector<string> &assignlist );
   
   int getTermsFormAssign(const string assign, vector<Term> &termlist ,bool  &belong);

}



#endif
