#include <iostream>
#include "baz.hpp"

Foo::Baz::Baz(int n) : 
    sz2{n},
    arr2{new double[n]}
{
    std::cout << this << "\tBaz(int)\n";
}

Foo::Baz::Baz(Bar x) :
    b(std::move(x))
{
    std::cout << this << " " << &x << "\tBaz(Bar)\n";
}

Foo::Baz::Baz(std::initializer_list<double> list) :
    Bar(list),
    sz2{static_cast<int>(list.size())},
    arr2{new double[sz2]}
{
    std::cout << this << "\tBaz(std::initializer_list<double>)\n";

    std::copy(list.begin(), list.end(), arr2.get());
}

Foo::Baz::Baz(const Baz& other) :
    Bar(other),
    sz2{other.sz2},
    arr2{new double[sz2]}
{
    std::cout << this << " " << &other << "\tBaz(const Baz&)\n";

    std::copy(other.arr2.get(), other.arr2.get() + other.sz2, arr2.get());
}

Foo::Baz::Baz(Baz&& other) noexcept :
Foo::Bar::Bar(std::move(other)),    /* the type of 'other' is
                                       Baz&& yet its value 
                                       category is lvalue
                                       */
    sz2{other.sz2},
    arr2{std::move(other.arr2)}
{
    std::cout << this << " " << &other << "\tBaz(Baz&&)\n";
}

Foo::Baz::~Baz() 
{
    std::cout << this << "\t~Baz()\n";
}

Foo::Baz& Foo::Baz::operator=(Baz other)
{
    std::cout << this << " " << &other << "\toperator=(Baz)\n";

    swap(*this, other);

    return *this; 
}

double Foo::Baz::operator[](int i) const
{
    return arr2.get()[i]; 
}

double& Foo::Baz::operator[](int i)
{
    return arr2.get()[i];
}

Foo::Baz Foo::Baz::operator+(const Baz& other) const
{
    std::cout << this << " " << &other << "\toperator+(const Baz&)\n";

    if (this->sz2 != other.sz2)
    {
        std::cout << this << " " << &other << "\toperator+(const Baz&) : MSG1\n";
        return Baz{-1};
    }

    Baz ret(sz2);

    for (int i = 0; i < sz2; ++i)
    {
        ret.arr2.get()[i] = this->arr2.get()[i] + other.arr2.get()[i];
    }

    return ret;
}

void Foo::swap(Foo::Baz& a, Foo::Baz& b) noexcept
{
    using std::swap; 

    swap(static_cast<Bar&>(a), static_cast<Bar&>(b));
    // this still works because the compiler won't
    // implicitly upcast b to type Baz&
    // swap(a, static_cast<Bar&>(b));
    swap(a.sz2, b.sz2);
    swap(a.arr2, b.arr2);
}

std::ostream& Foo::operator<<(std::ostream& out, const Foo::Baz& obj) {
    return out << "obj[0] = " << obj[0];
}
