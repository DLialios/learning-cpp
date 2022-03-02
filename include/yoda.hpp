#ifndef YODA_H
#define YODA_H

#include <iostream>
#include "foo.hpp"

namespace yoda
{
    class Foo
    {
        protected:
            int x = 3;

        public:
            // CTORS
            Foo() {
                std::cout << this << "\tFoo()\n";
            }
            Foo(const Foo&) =delete; 
            Foo(Foo&&) =default;

            // DTOR
            virtual ~Foo() { 
                std::cout << this << "\t~Foo()\n";
            }
            /* this does not need to be pure virtual
               marking it as so is pointless
               It does need an implementation though */

            // operator overloads
            Foo& operator=(const Foo&) =delete; 
            Foo& operator=(Foo&&) =default;

            // member functions
            virtual unsigned roll() const =0;
    };

    class Bar : public Foo
    {
        public:
            // CTORS
            Bar() {
                std::cout << this << "\tBar()\n";
            }
            Bar(int x) {
                std::cout << this << "\tBar(int x)\n";
            }
            Bar(const Bar& other) {
                std::cout << this << " " << &other << "\tBar(const Bar&)\n";
            }

            // DTOR
            virtual ~Bar() override {
                std::cout << this << "\t~Bar()\n";
            }

            // member functions
            unsigned roll() const override { return x; }
    };

    class Baz : public Bar
    {
        private:
            ::Foo::Bar b{1,2,3,4};
            int x = 8;

        public:
            // CTOR
            Baz() {
                std::cout << this << "\tBaz()\n";
            }

            // DTOR
            ~Baz() {
                std::cout << this << "\t~Baz()\n";
            }

            // member functions
            unsigned get() { return ++Bar::x; }
    };
}

#endif
