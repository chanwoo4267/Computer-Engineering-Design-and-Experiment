#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;

template <typename T>
class Array {
protected:
    T *data;
    int len;
public:
    Array(int size)
    {
        if (size < 0) {
            cout<<"Error Message"<<endl;
        }
        else {
            len = size;
            data = new T[len];
        }
    }
    ~Array()
    {
        delete [] data;
    }
    int length() const
    {
        return len;
    }
    T & operator [](int i)
    {
        static T tmp;
        if (i >= 0 && i < len)
            return data[i];
        else {
            cout<<"Array bound error!"<<endl;
            return tmp;
        }
    }
    T operator [](int i) const
    {
        if (i >= 0 && i < len)
            return data[i];
        else {
            cout<<"Array bound error!"<<endl;
            return 0;
        }
    }
            
    void print()
    {
        cout<<"[";
        for(int i=0; i<len; i++)
            cout<<' '<<data[i];
        cout<<"]"<<endl;
    }
};

#endif
