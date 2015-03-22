/*
 * File:   RF_IO.cpp
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#include "RF_IO.h"
#include "RF_Utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

/**
 * Class constructor.
 */
RF_IO::RF_IO()
{
    this->_dataFile = "";
    this->_modelFile = "";
}

/**
 * Class destructor.
 */
RF_IO::~RF_IO()
{
}

void RF_IO::setDataFile(string f)
{
    this->_dataFile = f;
}


void RF_IO::setModelFile(string f)
{
    this->_modelFile = f;
}

RF_DataSampleCont* RF_IO::readData()
{
    if (this->_dataFile.length() < 1)
        throw Exception();

    RF_DataSampleCont * cont = new RF_DataSampleCont();

    /* Read data */
    ifstream inData(this->_dataFile.c_str());
    int i = 0;
    for (string line; getline(inData, line); i++)
    {
        RF_DataSample *sample = new RF_DataSample();

        if (line.find(";") == string::npos)
        {
            delete sample;
            continue;
        }

        string imgPath = line.substr(0, line.find(";"));
        string labelPath = line.substr(line.find(";") + 1, line.length() - line.find(";") - 1);

        sample->setName(imgPath);
        sample->setChannel(imread(imgPath, CV_LOAD_IMAGE_COLOR), T_CHANNEL_RGB);
        sample->setLabel(imread(labelPath, CV_LOAD_IMAGE_COLOR));

        cont->addSample(sample);
    }
    inData.close();

    return cont;
}

RF_RandomForest* RF_IO::readModel()
{
    if (this->_modelFile.empty())
        throw Exception();

    RF_RandomForest * rf = new RF_RandomForest();

    ifstream inModel(this->_modelFile.c_str());

    for (string line; getline(inModel, line);)
    {
        if (line.find("#") == 0)
        {
            continue;
        }

        RF_Tree * tree = new RF_Tree();
        tree->setId(0);

        stringstream lineStream(line);
        for (string node; getline(lineStream, node, ' ');)
        {
            int leftBracket = node.find("(");
            string nodeNr = node.substr(0, leftBracket);
            string symbol = node.substr(leftBracket + 1, 1);
            int rightBracket = node.find(")", leftBracket);

            if (symbol == ":")
            {
                /* This is a leaf */
                string probsString = node.substr(leftBracket + 2, rightBracket - leftBracket - 2);
                RF_DataProb* p = new RF_DataProb();
                stringstream probs(probsString);
                for (string prob; getline(probs, prob, ',');)
                {
                    uint key = String2Number<uint>(prob.substr(0, prob.find("=")));
                    float value = String2Number<float>(prob.substr(prob.find("=") + 1, prob.length() - prob.find("=")));
                    p->addValue(key, value);
                }
                tree->addSubtree(true, String2Number<int>(nodeNr), 0, 0, NULL, p);
            }
            else
            {
                /* This is a sub-tree */
                int comma = node.find(",", leftBracket);
                string leftTree = node.substr(leftBracket + 1, comma - leftBracket - 1);
                string rightTree = node.substr(comma + 1, rightBracket - comma - 1);
                leftBracket = node.find("(", rightBracket);
                rightBracket = node.find(")", leftBracket);
                int comma1 = node.find(",", leftBracket);
                string funcType = node.substr(leftBracket + 1, comma1 - leftBracket - 1);

                RF_NodeFunc *f = new RF_NodeFunc();
                if (String2Number<int>(funcType) == RF_FUNC_ABSTEST)
                {
                    int comma2 = node.find(",", comma1 + 1);
                    string funcChannel = node.substr(comma1 + 1, comma2 - comma1 - 1);
                    string funcThreshold = node.substr(comma2 + 1, rightBracket - comma2 - 1);
                    f->setType(String2Number<int>(funcType));
                    f->setChannel(String2Number<int>(funcChannel));
                    f->setThreshold(String2Number<int>(funcThreshold));
                }

                tree->addSubtree(false, String2Number<int>(nodeNr), String2Number<int>(leftTree), String2Number<int>(rightTree), f, NULL);
            }


        }
        rf->validateTree(tree);
        rf->addTree(tree);
    }

    return rf;
}

/**
 * Write random forest model given in parameter into file also given in parameter.
 *
 * @param rf
 * @param path
 */
void RF_IO::writeModel(RF_RandomForest * rf)
{
    if (this->_modelFile.length() < 1)
        throw Exception();

    ofstream outData(this->_modelFile.c_str());
    outData << rf->dumpForest();
    outData.close();
}