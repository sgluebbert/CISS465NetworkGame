#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H



#include "Networking/NetString.h"



class Serializable {
public:
    virtual NetString * Serialize() = 0;
    virtual bool Deserialize(NetString *) = 0;
};



#endif