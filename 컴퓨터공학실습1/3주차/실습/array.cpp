#include "array.h"

Array::Array(int size) {
    if (size < 0) {
        cout<<"Error Message"<<endl;
    }
    else {
        len = size;
        data = new int[len];
    }
}

Array::~Array() {
    delete [] data;
}

int Array::length() const{
    return len;
}

int& Array::operator [](int i) {
    static int tmp;
    if (i >= 0 && i < len)
        return data[i];
    else {
        cout<<"Array bound error!"<<endl;
        return tmp;
    }
}

int Array::operator [](int i) const{
    if (i >= 0 && i < len)
        return data[i];
    else {
        cout<<"Array bound error!"<<endl;
        return 0;
    }
}

void Array::print() {
    cout<<"[";
    for(int i=0; i<len; i++)
        cout<<' '<<data[i];
    cout<<"]"<<endl;
}

RangeArray::RangeArray(int b,int e):Array(e-b+1) {
    base = b;
    end = e;
}

int RangeArray::baseValue() {
    return base;
}

int RangeArray::endValue() {
    return end;
}

int& RangeArray::operator [](int i) {
    static int tmp;
    if (i >= base && i <= end)
        return data[i-base];
    else {
        cout<<"Array bound error!"<<endl;
        return tmp;
    }
}

int RangeArray::operator [](int i) const{
    if (i >= base && i <= end)
        return data[i-base];
    else {
        cout<<"Array bound error!"<<endl;
        return 0;
    }
}
