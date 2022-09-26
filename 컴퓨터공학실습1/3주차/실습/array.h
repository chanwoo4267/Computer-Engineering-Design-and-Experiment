#ifndef ARRAY_H_
#define ARRAY_H_

#include <iostream>
using namespace std;

class Array {
protected:
    int *data;
    int len;
public:
    Array(int size);
    ~Array();
    int length() const;
    int & operator [](int i);
    int operator [](int i) const;
    void print();
};

class RangeArray : public Array {
private:
    int base;
    int end;
    
public:
    RangeArray(int b, int e);
    int baseValue();
    int endValue();
    int & operator [](int i);
    int operator [](int i) const;
};

#endif /* ARRAY_H */
