#ifndef ABSTRACTOUTPUT_H
#define ABSTRACTOUTPUT_H

#include <vector>

typedef std::vector<unsigned char> ByteArray;

class AbstractOutput
{
public:
    virtual ~AbstractOutput() {};
    virtual void sendData(char deviceID, char tag, const ByteArray& payload) = 0;
};

#endif // ABSTRACTOUTPUT_H
