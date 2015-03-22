/*
 * File:   RF_DataSample.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_DATASAMPLE_H
#define	RF_DATASAMPLE_H

#include <opencv2/core/core.hpp>
#include <map>

using namespace std;
using namespace cv;

class RF_DataSample {
public:
    RF_DataSample();
    virtual ~RF_DataSample();

    void setChannel(Mat m, int i);
    Mat getChannel(int i);
    void setLabel(Mat m);
    Mat getLabel();
    void setName(string s);
    string getName();
    int getWidth();
    int getHeight();

    void generateChannel(int i);
    void createGrayscaleChannel();
    void createRedChannel();
    void createGreenChannel();
    void createBlueChannel();

    float getSimilarityOflabels(RF_DataSample* ds);
    RF_DataSample* getSubsample(int lx, int ly, int hx, int hy);

private:
    string _name;
    map<int, Mat> _channel;
    Mat _label;
};

#endif	/* RF_DATASAMPLE_H */

