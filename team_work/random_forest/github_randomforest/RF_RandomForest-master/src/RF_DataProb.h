/*
 * File:   RF_DataProb.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_DATAPROB_H
#define	RF_DATAPROB_H

#include <map>

using namespace std;
typedef unsigned int uint;


class RF_DataProb {
public:
    RF_DataProb();
    virtual ~RF_DataProb();

    float getPosteriori(uint i);
    uint getMaximal();
    void increasePosteriori(uint i);
    void normalize();

    string dumpProbabilities();

    void addValue(uint key, float val);
    void clear();
    void sum(RF_DataProb * p);
private:

    map<uint, float> probabilities;
};

#endif	/* RF_DATAPROB_H */

