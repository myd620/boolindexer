#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <stdint.h>
#include <vector>

using namespace std;


namespace booleonindex{

    class Counter
    {

    public:
        Counter();

        ~Counter();

        int Reset();

        int setMaxCounterSize(int maxSize);

        int addConjCountById(int conjid);

        int addBlackConjById(int conjid);

        int getConjidNumById(int conjid, int &num, bool &black);

    private:
        vector<uint8_t> conjnum;

        vector<uint8_t> blackset;
    };

}

#endif
