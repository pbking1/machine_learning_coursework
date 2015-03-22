/*
 * File:   RF_NodeFunc.h
 * Author: Martin Simon <martiinsiimon@gmail.com>
 */

#ifndef RF_NODEFUNC_H
#define	RF_NODEFUNC_H

enum RF_FUNC
{
    RF_FUNC_ABSTEST,
    RF_FUNC_DIFFTEST
};

class RF_NodeFunc
{
public:
    RF_NodeFunc();
    virtual ~RF_NodeFunc();

    int getType();
    void setType(int t);

    /* ABSTEST */
    int getThreshold();
    void setThreshold(int t);
    int getChannel();
    void setChannel(int ch);
private:

    int type;

    /* ABSTEST */
    int threshold;
    int channel;
};

#endif	/* RF_NODEFUNC_H */

