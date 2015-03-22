/*
 * File:   RF_Tree.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_TREE_H
#define	RF_TREE_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <sstream>
#include "RF_DataSampleCont.h"
#include "RF_NodeFunc.h"
#include "RF_DataProb.h"
#include "RF_Utils.h"

using namespace std;
using namespace cv;

class RF_Tree {
public:
    RF_Tree();
    virtual ~RF_Tree();

    void train();

    void setDataset(RF_DataSampleCont* d);
    void setChannels(vector<int> c);

    string dumpTree();
    void setId(int id);
    int getId();
    void setMaximalDepth(int i);
    int getMaximalDepth();

    bool isLeaf();

    void generatePosteriori();

    RF_DataProb* solveTree(RF_DataSample *ds);

    RF_DataProb* getProbabilities();

    void clearDataset();

    void validate();

    RF_Tree* getLeft();
    RF_Tree* getRight();
    void normalizeProbs();

    RF_NodeFunc* getFunc();


    void addSubtree(bool leaf, int id, int left, int right, RF_NodeFunc *f, RF_DataProb* p);
    void addNode(int left, int right, RF_NodeFunc* f);
    void addLeaf(RF_DataProb* p);
private:
    int _treeId;

    bool leaf;
    RF_NodeFunc * func;
    RF_Tree* tLeft;
    RF_Tree* tRight;

    RF_DataProb* probabilities;

    int maximalDepth;
    RF_DataSampleCont * _dataset;
    vector<int> _channels;
};

#endif	/* RF_TREE_H */

