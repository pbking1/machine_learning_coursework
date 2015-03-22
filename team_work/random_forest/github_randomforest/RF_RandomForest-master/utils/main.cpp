/*
 * File:   main.cpp
 * Author: martin
 *
 * Created on 23. říjen 2013, 23:18
 */

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

/* Global variables */
static string POV_help =
    "Usage: ./pov-semantic-segmentation image1 [image2 [image3 [...]]";

int x[] = {0, 500, 0};
int y[] = {0, 0, 500};


/**
 * Do low-level segmentation on given image.
 * Segmentation method: TBD
 * @param input Image to be segmented
 * @return Segmented image
 */
Mat doLowLevelSegmentation(Mat input)
{
    return Mat(input);
}

/**
 * Do high-level segmentation on given image.
 * Segmentation method: TBD
 * @param input Image to be segmented
 * @return Segmented image
 */
Mat doHighLevelSegmentation(Mat input)
{
    return Mat(input);
}


/**
 * Main function of semantic segmentation
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv)
{
    vector<Mat> images;

    /* Read and parse parameters */
    if (argc < 2)
    {
        /* Not enough arguments */
        cerr << "ERR: At least one image needed!" << endl;
        cout << POV_help << endl;
        return 1;
    }
    else if (argc == 2 && string(argv[1]) == "-h" || string(argv[1]) == "--help")
    {
        /* Help required */
        cout << POV_help << endl;
        return 0;
    }
    else
    {
        /* Read input images */
        for (int i = 1; i < argc; i++)
        {
            Mat tmp = imread(string(argv[i]));
            if (tmp.empty())
            {
                cerr << "ERR: Only images could be in parameters" << endl;
                return 1;
            }
            images.push_back(tmp);
        }
    }

    /* Show window for result showing */


    /* Iterate over all images */
    for (int i = 0; i < images.size(); i++)
    {
        /* Prepare data structures */
        Mat original, lowSeg, result;
        original = images.at(i);

        /* Show the original image */
        imshow("Original Image", original);
        moveWindow("Original Image", x[0], y[0]);

        /* Do low-level segmentation, e.g. using watershed or something */
        lowSeg = doLowLevelSegmentation(original);
        //TODO implement

        /* Show semi-result */
        imshow("Low-level segmentation", lowSeg);
        moveWindow("Low-level segmentation", x[1], y[1]);

        /* Do high-level segmentation, e.g. using MRF or CRF */
        result = doHighLevelSegmentation(lowSeg);
        //TODO implement

        /* Show result and wait until user presses the keyboard to continue */
        imshow("Segmentation result", result);
        moveWindow("Segmentation result", x[2], y[2]);

        /* Wait until user presses a key */
        waitKey();

        /* Destroy all windows */
        destroyAllWindows();
    }

    /* Exit */
    return 0;
}

