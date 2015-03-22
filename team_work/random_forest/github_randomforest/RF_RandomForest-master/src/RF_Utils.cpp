/*
 * File:   RF_Utils.cpp
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#include <iostream>

#include "RF_Train.h"
#include "RF_Test.h"


using namespace std;


/* Global variables */
static string RF_help =
    "Usage:\n"
    "  ./RF_Utils train data_file model_file\n"
    "  ./RF_Utils predict data_file model_file\n"
    "  ./RF_Utils test data_file model_file\n"
    "  ./RF_Utils help";

/**
 * 
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv)
{
    if (argc == 2 && string(argv[1]) == "help")
    {
        /* Help requested */
        cout << RF_help << endl;
        return 0;
    }
    else if (argc < 4)
    {
        /* Not enough arguments */
        cerr << "ERR: Not enough arguments!" << endl;
        cout << RF_help << endl;
        return 1;
    }
    else if (argc == 4 && string(argv[1]) == "train")
    {
        /* Train random forest */

        //TODO catch exceptions
        RF_Train *model = new RF_Train(string(argv[2]), string(argv[3]));

        /* Parse inputs, load data and prepare data samples */
        model->prepareTraining();

        /* Train random forest */
        model->trainForest();

        /* Dump trained model into file */
        model->exportModel();

        /* Print parameters of trained model */
        model->printResults();

        delete model;
        /* Exit correctly */
        return 0;
    }
    else if (argc == 4 && string(argv[1]) == "predict")
    {
        /* Predict samples with existing model */
        //TODO call RF_Test with input data - predict mode
        cout << "DBG: predict" << endl;
        return 0;
    }
    else if (argc == 4 && string(argv[1]) == "test")
    {
        //TODO catch exceptions
        /* Evaluate prediction rate */
        RF_Test *model = new RF_Test(string(argv[2]), string(argv[3]));

        /* Load model from file*/
        model->loadModel();

        /* Solve data and store results */
        model->solveData();

        /* Print results of evaluation  */
        model->printResults();


        delete model;
        /* Exit correctly */
        return 0;
    }
    else
    {
        /* Invalid argument combination */
        cerr << "ERR: Invalid argument combination" << endl;
        cout << RF_help << endl;
        return 1;
    }
}

