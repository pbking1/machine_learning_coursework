/*
 * File:   RF_Train.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_TRAIN_H
#define	RF_TRAIN_H

#include <iostream>
#include "RF_DataSampleCont.h"
#include "RF_RandomForest.h"


using namespace std;

class RF_Train {
public:
    RF_Train();
    RF_Train(string dataFile, string modelFile);
    virtual ~RF_Train();
    void setDataFile(string f);
    void setModelFile(string f);

    void prepareTraining(void);
    void trainForest(void);

    void exportModel(void);

    void printResults(void);
    string getResults(void);
private:

    /* Forest configuration */
    int _maxDepth;
    int _maxTrees;
    int _n;

    /* Other attributes */
    string _dataFile;
    string _modelFile;
    bool _trained;

    /* Training data */
    RF_DataSampleCont * _data;

    /* Random Forest */
    RF_RandomForest * _forest;

};

#endif	/* RF_TRAIN_H */

