/*
Jordan Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
MIT licence via mit-license.org held by author
*/

#ifndef BITTER_HPP
#define BITTER_HPP

#include <iostream>
#include <cassert>
using namespace std;

// A simple bit-reader that latches onto a "host" variable. The variable may be
// changed from the outside, or have its memory changed bitwise by the bitter.
template <class T>
class Bitter
{
public:
    // Construct from a NON-ARRAY host
    Bitter(T *Host) : host(Host), sizeOfHost(sizeof(*host) * 8) {}

    // Construct from an ARRAY host
    Bitter(T *Host, const unsigned int &ArrSize) : host(Host), sizeOfHost(sizeof(*host) * ArrSize * 8) {}

    // Get a bit from the host's memory
    bool operator[](const unsigned int &Bit) const;

    // Get a modifiable reference to the host's data starting at a point
    char *getFrom(const unsigned int &Start);

    // Flip a bit of the host's memory
    void flip(const unsigned int &Bit);

    // Set a bit of the host's memory
    void set(const unsigned int &Bit, const bool &To);
    void set(const unsigned int &Byte, const char &To);

    // Get the size (in bytes) of the host (still works with arrays)
    int size() const;

protected:
    T *host;
    const unsigned int sizeOfHost;
};

// Output the bits of the host's memory to a stream
template <class T>
ostream &operator<<(ostream &strm, const Bitter<T> &b)
{
    cout << "0b";
    for (int i = 0; i < b.size(); i++)
    {
        strm << b[i];
    }
    return strm;
}

//////////////////////////////////

template <class T>
bool Bitter<T>::operator[](const unsigned int &Bit) const
{
    if (Bit >= sizeOfHost * 8)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    // narrow to char (convenience)
    unsigned char data = ((unsigned char *)(host))[Bit / 8];

    // bitshift and apply mask
    data = data << (Bit % 8);
    data &= 0b1000'0000;

    // create bool
    return (data == 128);
}

template <class T>
char *Bitter<T>::getFrom(const unsigned int &Start)
{
    if (Start >= sizeOfHost)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    return (char *)(&host) + Start;
}

template <class T>
void Bitter<T>::flip(const unsigned int &Bit)
{
    if (Bit >= sizeOfHost * 8)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    char *data = (char *)(host) + (Bit / 8);
    char mask = 0b0000'0001 << Bit % 8;

    *data ^= mask;

    return;
}

template <class T>
void Bitter<T>::set(const unsigned int &Bit, const bool &To)
{
    if (Bit >= sizeOfHost * 8)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    char *data = (char *)(host) + (Bit / 8);
    char mask = 0b0000'0001 << Bit % 8;

    if (To)
    {
        // The thing at data has its Bit % 8'th bit turned positive
        *data |= mask;
    }
    else
    {
        // The thing at data has its Bit % 8'th bit turned negative
        // (and equals with an inverted mask)
        *data &= ~mask;
    }

    return;
}

template <class T>
void Bitter<T>::set(const unsigned int &Byte, const char &To)
{
    if (Byte >= sizeOfHost)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    char *data = (char *)(host) + (Byte / 8);
    *data = To;

    return;
}

template <class T>
int Bitter<T>::size() const
{
    return sizeOfHost;
}

#endif
