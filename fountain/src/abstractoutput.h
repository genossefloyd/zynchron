#ifndef ABSTRACTOUTPUT_H
#define ABSTRACTOUTPUT_H

#include <vector>

typedef std::vector<unsigned char> ByteArray;

//interface class for data output
class AbstractOutput
{
public:
    virtual ~AbstractOutput() {};

    //abstract interface for sending data identified by two tags:
    // deviceID: byte tag to identify the source device
    // tag: byte tag to identify the type of data send
    // payload: byte array with generic payload
    virtual void sendData(char deviceID, char tag, const ByteArray& payload) = 0;
};

#endif // ABSTRACTOUTPUT_H
