#include  "counter.h"

namespace booleonindex{

    Counter::Counter()
    {

    }


    Counter::~Counter()
    {

    }

    int Counter::Reset()
    {
        for (int i = 0; i < conjnum.size(); i++)
        {
            conjnum[i] = 0;
        }

        for (int i = 0; i < blackset.size(); i++)
        {
            blackset[i] = 0;
        }

        return 0;
    }


    int Counter::setMaxCounterSize(int maxSize)
    {
        if (conjnum.size() < maxSize)
        {
            conjnum.resize(maxSize);
        }

        int bitnum = (maxSize % 8) ? (maxSize / 8 + 1) : maxSize / 8;

        if (blackset.size() < bitnum)
        {
            blackset.resize(bitnum);
        }

        return 0;
    }


    int Counter::addConjCountById(int conjid)
    {
        if (conjid >= conjnum.size())
        {
            return -1;
        }

        conjnum[conjid] += 1;

        return 0;
    }

    int Counter::addBlackConjById(int conjid)
    {
        int index = conjid / 8;
        int bit = conjid % 8;
        if (index >= blackset.size())
        {
            return -1;
        }
        blackset[index] |= (0x01 << bit);
        return 0;
    }

    int Counter::getConjidNumById(int conjid, int &num, bool &black)
    {
        if (conjid >= conjnum.size() || (conjid / 8) > blackset.size())
        {
            return -1;
        }

        num = conjnum[conjid];

        black = (blackset[conjid / 8] & (0x01 << (conjid % 8))) ? true : false;

        return 1;
    }

}

