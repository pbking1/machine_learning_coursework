/*
 * File:   RF_Test.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_TEST_H
#define	RF_TEST_H

#include <string>
#include "RF_RandomForest.h"
#include "RF_IO.h"

using namespace std;

class RF_Test {
public:
    RF_Test();
    RF_Test(string _dataPath, string _modelPath);

    virtual ~RF_Test();

    void setModelFile(string f);
    void setDataFile(string f);

    void loadModel();
    void solveData();
    void printResults();
private:
    string modelFile;
    string dataFile;

    RF_RandomForest* forest;
    RF_DataSampleCont * _data;

    double sum;
};

#endif	/* RF_TEST_H */

