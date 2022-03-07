#ifndef BAZ_H
#define BAZ_H

#include <iostream>
#include <memory>
#include <iterator>
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

            // forward iterator
            struct Iterator {
                using iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = double;
                using pointer           = value_type*;
                using reference         = value_type&;

                Iterator(pointer ptr) : m_ptr(ptr) {}

                reference operator*() const { return *m_ptr; }
                pointer operator->() { return m_ptr; }

                Iterator& operator++() {
                    ++m_ptr;
                    return *this; 
                }
                Iterator operator++(int) {
                    Iterator tmp = *this;
                    ++(*this);
                    return tmp;
                }

                friend bool operator==(const Iterator& a, const Iterator& b) {
                    return a.m_ptr == b.m_ptr;
                }
                friend bool operator!=(const Iterator& a, const Iterator& b) {
                    return !(a == b);
                }

                private:
                    pointer m_ptr;
            };

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
            Iterator begin() { 
                return Iterator(arr2.get());
            }
            Iterator end() { 
                return Iterator(arr2.get() + sz2);
            }

            // operator overloads
            Baz& operator=(Baz);
            double operator[](int) const override;
            double& operator[](int) override;
            Baz operator+(const Baz&) const;

            // friends
            friend void swap(Baz&,Baz&) noexcept;
            friend std::ostream& operator<<(std::ostream&, const Baz&);
    };

    extern void swap(Foo::Baz&,Foo::Baz&) noexcept;
    extern std::ostream& operator<<(std::ostream&, const Baz&);
}

#endif
