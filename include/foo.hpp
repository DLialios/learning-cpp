#ifndef FOO_H
#define FOO_H

#include <iostream>
#include <memory>

namespace Foo
{
    class Bar
    {
        protected:
            int sz;
            std::unique_ptr<double> arr;

        public:
            // CTORS
            explicit Bar(int = 9);
            Bar(std::initializer_list<double>);
            Bar(const Bar&);
            Bar(Bar&&) noexcept;

            // DTOR
            virtual ~Bar();

            // conversion functions
            operator double() const { 
                std::cout 
                    << this 
                    << " " 
                    << "\tFoo::Bar::operator double()\n";
                return arr.get()[0];
            }

            // operator overloads
            virtual double operator[](int) const;
            virtual double& operator[](int);
            void* operator new(size_t,int);
            void operator delete(void*);
            Bar& operator=(Bar);
            Bar operator+(const Bar&) const;

            // this assumes there is a swap in this
            // namespace, hence the need for the
            // extern statement
            friend void swap(Bar&,Bar&) noexcept;
    };

    // user-defined literal
    inline Foo::Bar operator""_bar(long double arg)
    {
        std::cout << "Foo::operator\"\"_bar(long double)\n";
        return {static_cast<double>(arg)};
    }

    extern void swap(Bar&,Foo::Bar&) noexcept;
    extern Bar func(int);
    extern Bar func2(int);
}

#endif
