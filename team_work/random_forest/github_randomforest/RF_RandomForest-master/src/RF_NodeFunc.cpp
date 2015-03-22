/*
 * File:   RF_NodeFunc.cpp
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#include "RF_NodeFunc.h"

/**
 * Class constructor.
 */
RF_NodeFunc::RF_NodeFunc()
{
}

/**
 * Class destructor.
 */
RF_NodeFunc::~RF_NodeFunc()
{
}

int RF_NodeFunc::getType()
{
    return this->type;
}

void RF_NodeFunc::setType(int t)
{
    this->type = t;
}

int RF_NodeFunc::getChannel()
{
    return this->channel;
}

void RF_NodeFunc::setChannel(int ch)
{
    this->channel = ch;
}

int RF_NodeFunc::getThreshold()
{
    return this->threshold;
}

void RF_NodeFunc::setThreshold(int t)
{
    this->threshold = t;
}