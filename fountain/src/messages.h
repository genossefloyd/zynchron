#ifndef MESSAGES_H
#define MESSAGES_H

namespace msg
{

//enum with data type identifier
//it is used as data tag for output classes to identify the type/origin of send data
enum DataType
{
    DEVICENAME = 0x01,
    ACCELEROMETER = 0x02,
    GYRO = 0x03,
    TEMPERATURE = 0x04
};

}

#endif // MESSAGES_H
