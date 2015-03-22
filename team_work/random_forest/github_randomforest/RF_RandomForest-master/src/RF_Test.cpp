/*
 * File:   RF_Test.cpp
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#include "RF_Test.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/**
 * Class constructor.
 */
RF_Test::RF_Test()
{
    this->modelFile = "";
    this->dataFile = "";

    this->forest = NULL;
    this->_data = NULL;

    this->sum = 0.0;
}

RF_Test::RF_Test(string _dataPath, string _modelPath)
{
    this->modelFile = _modelPath;
    this->dataFile = _dataPath;

    this->forest = NULL;
    this->_data = NULL;

    this->sum = 0.0;
}

/**
 * Class destructor.
 */
RF_Test::~RF_Test()
{
    if (this->forest != NULL)
        delete this->forest;
    if (this->_data != NULL)
        delete this->_data;
}

void RF_Test::setDataFile(string f)
{
    this->dataFile = f;
}

void RF_Test::setModelFile(string f)
{
    this->modelFile = f;
}

void RF_Test::loadModel()
{
    cout << "Loading model from file" << endl;
    if (this->modelFile.empty())
        return;
    RF_IO * io = new RF_IO();
    io->setModelFile(this->modelFile);

    this->forest = io->readModel();

    delete io;
}

void RF_Test::solveData()
{
    /* Load data if there are any */
    if (this->dataFile.empty() || this->forest == NULL)
        return;

    RF_IO* io = new RF_IO();
    io->setDataFile(this->dataFile);

    this->_data = io->readData();

    delete io;

    /* Generate all channels */
    this->_data->generateAllChannels();

    RF_DataSample * tmpS;
    float similarity;

    /* Label every sample in dataset and get results */
    for (vector<RF_DataSample*>::iterator it = this->_data->getSamples()->begin(); it != this->_data->getSamples()->end(); it++)
    {
        cout << "Testing " << (*it)->getName() << endl;

        tmpS = this->forest->solveSample((*it));

        similarity = (*it)->getSimilarityOflabels(tmpS);
        this->sum += similarity;

        delete tmpS;
        cout << "\t Success in " << similarity << "%" << endl;
    }
}

void RF_Test::printResults()
{
    cout << "Total success: " << this->sum / this->_data->samplesCount() << "%" << endl;
}