/*
 * File:   RF_DataProb.cpp
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#include <stdexcept>
#include <iostream>
#include <string>
#include "RF_DataProb.h"
#include "RF_Utils.h"

using namespace std;

/**
 * Class constructor.
 */
RF_DataProb::RF_DataProb()
{
}

/**
 * Class destructor.
 */
RF_DataProb::~RF_DataProb()
{
}

/**
 * Increase posterior probability for key given in argument
 *
 * @param i Key which should have posterior probability incresed
 */
void RF_DataProb::increasePosteriori(uint i)
{
    try
    {
        this->probabilities.at(i) = this->probabilities.at(i) + 1;
    }
    catch (out_of_range)
    {
        this->probabilities.insert(pair<uint, float>(i, 1.0f));
    }
}

/**
 * Normalize all the stored probabilities to sum to 1 (like 100 %).
 */
void RF_DataProb::normalize()
{
    float sum = 0.0f;

    /* Delete black - unlabeled */
    this->probabilities.erase(0);

    /* Count sum */
    for (map<uint, float>::iterator it = this->probabilities.begin(); it != this->probabilities.end(); it++)
    {
        sum += it->second;
    }

    /* Divide every element by sum */
    for (map<uint, float>::iterator it = this->probabilities.begin(); it != this->probabilities.end(); it++)
    {
        this->probabilities.at(it->first) /= sum;
    }
}

/**
 * Dump probability values into one string to export purposes. Every pair (key, value) is stored in format 'key=value' (without apostrophs).
 *
 * @return String of all probabilities stored in object
 */
string RF_DataProb::dumpProbabilities()
{
    string result = "";

    /* Append every pair key-value */
    for (map<uint, float>::iterator it = this->probabilities.begin(); it != this->probabilities.end();)
    {
        result += Number2String(it->first);
        result += "=";
        result += Number2String(it->second);

        if (++it != this->probabilities.end())
        {
            result += ",";
        }
    }

    return result;
}

/**
 * Import into probability map new pair.
 *
 * @param key Key of inserted pair
 * @param value Value of insterted pair
 */
void RF_DataProb::addValue(uint key, float value)
{
    this->probabilities.insert(pair<uint, float>(key, value));
}

/**
 * Clear the map of probabilities.
 */
void RF_DataProb::clear()
{
    this->probabilities.clear();
}

/**
 * Add given probabilities to this one (per element).
 *
 * @param p Probabilities to add to this object (per element)
 */
void RF_DataProb::sum(RF_DataProb* p)
{
    /* Iterate over all items */
    for (map<uint, float>::iterator it = p->probabilities.begin(); it != p->probabilities.end(); it++)
    {
        try
        {
            /* Add value if key exists */
            this->probabilities.at(it->first) += it->second;
        }
        catch (out_of_range)
        {
            /* Insert new element if key doesn't exist */
            this->probabilities.insert(pair<uint, float>(it->first, it->second));
        }
    }
}

/**
 * Get key of maximal probability (key with maximal value). It actually returns the first maximal value if there are more of them with the very same value.
 *
 * @return Key of element with maximal probability
 */
uint RF_DataProb::getMaximal()
{
    uint max = 0;
    float maxVal = 0.0f;
    for (map<uint, float>::iterator it = this->probabilities.begin(); it != this->probabilities.end(); it++)
    {
        if (it->second > maxVal)
        {
            max = it->first;
            maxVal = it->second;
        }
    }
    return max;
}