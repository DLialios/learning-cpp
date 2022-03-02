#ifndef BAZ_H
#define BAZ_H

#include <iostream>
#include <memory>
#include "foo.hpp"

namespace Foo 
{
    class Baz : public Bar
    {
        private:
            int sz2;
            std::unique_ptr<double> arr2;
            Bar b{1,3,5};

        public:
            // CTORS
            explicit Baz(Bar x); // testing call order
            explicit Baz(int = 6);
            Baz(std::initializer_list<double>);
            Baz(const Baz&);
            Baz(Baz&&) noexcept;

            // DTOR
            ~Baz() override;

            // conversion functions
            operator double() const { 
                std::cout 
                    << this 
                    << " " 
                    << "\tFoo::Baz::operator double()\n";
                return arr2.get()[0];
            }

            // regular functions
            int size() const { return sz; }
            double* begin() { 
                return sz ? &arr.get()[0] : nullptr;
            }
            double* end() {
                return sz ? &arr.get()[0] + sz : nullptr;
            }

            // operator overloads
            Baz& operator=(Baz);
            double operator[](int) const override;
            double& operator[](int) override;
            Baz operator+(const Baz&) const;

            // friends
            friend void swap(Baz&,Baz&) noexcept;
    };

    extern void swap(Foo::Baz&,Foo::Baz&) noexcept;
}

#endif
