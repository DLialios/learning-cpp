#include <iostream>
#include <cstdlib>
#include "foo.hpp"

Foo::Bar::Bar(int n) :
    sz{n},
    arr{new double[n]}
{
    std::cout << this << "\tBar(int)\n";
}

Foo::Bar::Bar(std::initializer_list<double> list) :
    sz{static_cast<int>(list.size())},
    arr{new double[sz]}
{
    std::cout << this << "\tBar(std::initializer_list<double>)\n";

    std::copy(list.begin(),list.end(),this->arr.get());
}

Foo::Bar::Bar(const Bar& other) :
    sz{other.sz},
    arr{new double[other.sz]}
{
    std::cout << this << " " << &other << "\tBar(const Bar&)\n";

    std::copy(other.arr.get(), other.arr.get() + other.sz, this->arr.get());
}

Foo::Bar::Bar(Bar&& other) noexcept : 
sz{other.sz},
    arr{std::move(other.arr)}
{
    std::cout << this << " " << &other << "\tBar(Bar&&)\n";
}

Foo::Bar::~Bar() 
{
    std::cout << this << "\t~Bar()\n";
}

Foo::Bar& Foo::Bar::operator=(Bar other)
{
    std::cout << this << " " << &other << "\toperator=(Bar)\n";

    swap(*this, other);

    return *this; 
}

double Foo::Bar::operator[](int i) const
{
    return arr.get()[i]; 
}

double& Foo::Bar::operator[](int i)
{
    return this->arr.get()[i];
}

Foo::Bar Foo::Bar::operator+(const Bar& other) const
{
    std::cout << this << " " << &other << "\toperator+(const Bar&)\n";

    if (this->sz != other.sz)
    {
        std::cout << this << " " << &other << "\toperator+(const Bar&) : MSG1\n";
        return Bar{-1};
    }

    Bar ret(this->sz);

    for (int i = 0; i < this->sz; ++i)
    {
        ret.arr.get()[i] = this->arr.get()[i] + other.arr.get()[i];
    }

    return ret;
}

void Foo::swap(Bar& a,Bar& b) noexcept {
    using std::swap; 

    swap(a.sz, b.sz);
    swap(a.arr, b.arr);
}

void* Foo::Bar::operator new(size_t size, int x)
{
    void* ret = std::malloc(size);
    std::cout << ret << "\toperator new(size_t,int)\n";
    return ret;
}

void Foo::Bar::operator delete(void* ptr)
{
    std::cout << ptr << "\toperator delete(void*)\n";
    std::free(ptr);
}

Foo::Bar Foo::func(int s) 
{
    return Foo::Bar(s);
}

Foo::Bar Foo::func2(int s) 
{
    return {2,3,4,5};
}
