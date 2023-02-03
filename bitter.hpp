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
#include <sstream>
#include <string>
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
    char *getChars(const unsigned int &Start);
    T *getHost();

    // Flip a bit of the host's memory
    void flip(const unsigned int &Bit);

    // Set a bit of the host's memory
    void set(const unsigned int &Bit, const bool &To);

    // Set a byte of the host's memory starting at a bit
    void set(const unsigned int &Bit, const unsigned char &To);

    // Get the size (in bytes) of the host (still works with arrays)
    int size() const;

    // Get the binary representation
    string to_bin_str(const bool &Fancy = true);

protected:
    T *host;
    const unsigned int sizeOfHost;
};

// Output the bits of the host's memory to a stream
template <class T>
ostream &operator<<(ostream &strm, Bitter<T> &b)
{
    strm << b.to_bin_str(true);
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

    return (((unsigned char *)host)[Bit / 8] & (0b0000'0001 << (Bit % 8))) != 0;
}

template <class T>
char *Bitter<T>::getChars(const unsigned int &Start)
{
    if (Start >= sizeOfHost)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    return (char *)(&host) + Start;
}

template <class T>
T *Bitter<T>::getHost()
{
    return host;
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
        // The thing at data has its (Bit % 8_'th bit turned positive
        *data |= mask;
    }
    else
    {
        // The thing at data has its (Bit % 8)'th bit turned negative
        // (and equals with an inverted mask)
        *data &= ~mask;
    }

    return;
}

template <class T>
void Bitter<T>::set(const unsigned int &Bit, const unsigned char &To)
{
    if (Bit >= sizeOfHost * 8)
    {
        throw runtime_error("Cannot access out-of-range bit");
    }

    for (int i = 0; i < 8; i++)
    {
        bool toSet = ((To << i) & 0b1000'0000) == 0b1000'0000;
        set(Bit + i, toSet);
    }

    return;
}

template <class T>
int Bitter<T>::size() const
{
    return sizeOfHost;
}

template <class T>
string Bitter<T>::to_bin_str(const bool &Fancy)
{
    stringstream strm;

    if (Fancy)
    {
        strm << "0b";
    }

    for (int i = sizeOfHost - 1; i >= 0; i--)
    {
        if (i % 4 == 0 && Fancy && i != 0)
        {
            strm << '\'';
        }

        strm << (*this)[i];
    }

    return strm.str();
}

#endif
