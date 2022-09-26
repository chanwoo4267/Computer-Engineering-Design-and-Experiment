#ifndef GARRAY_H_
#define GARRAY_H_

#include "array.h"

template <typename T>
class GrowableArray : public Array<T> {

public:
    GrowableArray(int l) : Array<T>(l)
    {
            
    }
    T & operator [](int i)
    {
        if (i >= 0 && i < this->len)
            return this->data[i];
        else {
            T* tmpdata;
            T* deldata;
            
            tmpdata = new T[i*2];
            for(int k=0;k<this->len;k++)
                tmpdata[k] = this->data[k];
            for(int k=this->len;k<i*2;k++)
                tmpdata[k] = 0;
            
            deldata = this->data;
            this->data = tmpdata;
            delete[] deldata;
            
            this->len = i*2;
            
            return this->data[i];
        }
    }
};

#endif
