#ifdef CPP20
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <iostream>
#include <memory>
#include <vector>

namespace Templates1
{
    // useless parameterization
    template<class T> struct Container
    {
        template<class X> Container(T,X);
    };

    // defining the constructor 
    template<class T>
        template<class X>
        Container<T>::Container(T,X) {}

    // a doomed deduction guide
    template<class T, class X>
        Container(T,X) -> Container<typename X::some_type>;

    struct my_type { using some_type = int; };

    // Container error("const char *", my_type());
}

namespace Templates2
{
    template<typename T, typename P = int, int SIZE = 100>
        void print(T t) {
            std::cout<<t<<std::endl;
        }

    // full function template specialization
    // note how it is subject to the ODR
    template<>
        inline void print<int,double>(int i) {
            std::cout<<"int: "<<i<<std::endl;
        }

    //    // partial function template specialization
    //    template<typename T>
    //    inline void printf<double>(double i) {
    //        std::cout<<"double: "<<i<<std::endl;
    //    }
}

namespace Templates2_1
{
    // function template overload resolution
    // note that deducing the type of parameter T in the first
    // function below is impossible
    template<typename T> void f(bool) { std::cout<<"f(bool)\n"; }
    template<typename T> void f(T) { std::cout<<"f(T)\n"; }

    inline void f() {
        f(true);
        f<bool>(true);
    }
}

namespace Templates2_2
{
    // fold expression: unary left fold
    template<typename... Args>
        constexpr auto fold_test1(Args... vals) {
            return (... + vals);
        }
    inline constexpr int res = fold_test1(1,2,3,4);
    static_assert(res == 10);

    // fold expression: unary right fold
    template<typename... Args>
        constexpr auto fold_test2(Args... vals) {
            return (vals - ...);
        }
    inline constexpr int res2 = fold_test2(24,8,12,12,4);
    static_assert(res2 == 20);

    // fold expression: binary right fold
    template<typename... Args>
        constexpr auto fold_test3(Args... vals) {
            return (vals - ... - 4);
        }
    inline constexpr int res3 = fold_test3(20,24,12);
    static_assert(res3 == 4);

    // fold expression: binary left fold
    template<typename... Args>
        constexpr auto fold_test4(Args... vals) {
            return (20 - ... - vals);
        }
    inline constexpr int res4 = fold_test4(1,1,1,1);
    static_assert(res4 == 16);
}

namespace Templates2_3
{
    // variadic template
    template<typename T>
        constexpr T sum(T x) { return x; }
    template<typename T, typename... Args>
        constexpr T sum(T first, Args... rest) {
            return first + sum(rest...);
        }
    inline constexpr int res = sum(1,2,3,4);
    static_assert(res == 10);

    // variadic template
    template<typename none = void>
        constexpr int sum() {
            return 0;
        }
    template<int T, int... Args>
        constexpr int sum() {
            return T + sum<Args...>();
        }
    inline constexpr int res2 = sum<1,2,3,4>();
    static_assert(res2 == 10);

    // template metaprogramming 
    template<int V> struct IConst { 
        static constexpr int data = V;
        constexpr operator int() const {
            return data;
        }
    };

    template<int...> struct wtf;

    template<> struct wtf<> : IConst<0> {};

    template<int First, int... Rest> struct wtf<First,Rest...> 
        : IConst<First + wtf<Rest...>::data> {};

    inline constexpr wtf<1,2,3> res3;
    static_assert(res3 == 6);
}

namespace Templates3
{
    // alias template
    template<typename T>
        using alias_template = std::vector<T>;
    // using it
    inline alias_template<int> v{1,2,3};

    // variable template
    template<typename T> 
        T pi = T(3.1415);
    // using it
    inline double x = pi<int>;

    // template template parameters
    template<template<typename>class T,typename P>
        struct Bar {
            T<P> data;
        };
}

namespace Templates4
{
    template<typename T>
        class Qux {
            T t_;
            public:
            Qux(T t) : t_(t) {}
        };

    // full class template specialization
    template<>
        class Qux<double> {
            public:
                typedef int type;
                Qux(double) {}
                Qux(double,double) {}
                constexpr double get() const { return 3.14; }
        };

    // deduction guide (since c++17)
    // Qux(double,double) -> Qux<double>;
    // or 'make' functions:
    template<typename T>
        Qux<T> make_qux(T&& a, T&& b) {
            return {std::forward<T>(a),std::forward<T>(b)};
        }
    template<typename T>
        Qux<T> make_qux(T&& a) {
            return {std::forward<T>(a)};
        }
}

namespace Templates5
{
    // using 'typename' to disambiguate a dependent name
    template<template<typename,typename>class T, class P, class D>
        struct Test {
            typedef typename T<P,D>::value_type talias;
        };

    // explicit template instantiation
    template struct Test<std::vector,int,std::allocator<int>>;

    // using 'template' to disambiguate a dependent name
    template<typename T>
        struct S {
            template <typename U> void foo() {}
            template <typename U> static void foo2() {}
            template <typename U> struct S2 { using alias = int; };
        };

    template<typename T>
        using ptr_to_t = typename std::pointer_traits<T*>::template rebind<T>;

    template<typename T>
        void bar() {
            S<T> s;
            auto* ptr = &s;
            ptr_to_t<S<T>> ptr2;

            static_assert(std::is_same<decltype(ptr),decltype(ptr2)>::value);

            s.template foo<int>(); 
            ptr->template foo<int>();
            S<T>::template foo2<int>(); 
            typedef typename S<T>::template S2<int>::alias the_big_one;
            the_big_one x = 0;
            ++x;
        }
}

namespace Templates6 {
    // using if constexpr
    template<typename T>
        constexpr auto length(const T& t) {
            if constexpr (std::is_integral<T>::value)
                return t;
            else
                return t.length();
        }
}

namespace Templates7 {
    // concepts
    template<typename T>
        concept HasMeth = requires(T v, int i) {
            //{v.meth(i)} -> std::convertible_to<int>;
            // equiv to
            //requires std::convertible_to<decltype((v.meth(i))),int>;

            {v.meth(i)} noexcept -> std::convertible_to<int>;

            typename T::value_t;

            v+i;
        };

    //    auto f(HasMeth auto val) {
    //        return val.meth(20);
    //    }
    //    // or
    //    template<typename T>
    //        requires HasMeth<T>
    //        auto f(T val) {
    //            return val.meth(20);
    //        }
    //    // or
    //    template<HasMeth T>
    //        auto f(T val) {
    //            return val.meth(20);
    //        }
    //    // or
    //    template<typename T>
    //        requires requires(T v, int i) {
    //            {v.meth(i)} -> std::convertible_to<int>;
    //        }
    //    auto f(T val) {
    //        return val.meth(20);
    //    }

    struct S {
        int meth(int) noexcept {
            return 42;
        }

        using value_t = float;

        int operator+(int other) {
            return other;
        }
    };
}

namespace Templates8 {
    // universal reference
    template<typename T>
        void f(T&& var) {
            constexpr bool b =
                std::is_same_v<decltype(var),int&&>;
            static_assert(b);
        }

    template<typename T>
        void f2(const T& var) {
            constexpr bool b =
                std::is_same_v<decltype(var),const int&>;
            static_assert(b);
        }

    inline void f() {
        int x = 3;
        int& y = x;
        //f(y);
        //f2(y);
        f(std::move(y));
        f2(std::move(y));
    }
}

namespace Templates8_1 {
    // SFINAE
    struct Test { typedef int foo; };

    template<typename T>
        void f(typename T::foo) {}

    template<typename T>
        void f(T) {}

    inline void g() {
        f<Test>(3);
        f<int>(3); // < -- sfinae
    }
}

namespace Templates9 {
    // perfect forwarding
    // you have to use the type trait to force all calls
    // to forward to explicity provide a template parameter
    template<typename S>
        S&& forward(typename std::remove_reference<S>::type& a) noexcept {
            std::cout<< "@ " << a << "\n";
            return static_cast<S&&>(a);
        }

    template<typename T, typename Arg>
        std::unique_ptr<T> factory(Arg&& arg) {
            return std::unique_ptr<T>(new(1) T(forward<Arg>(arg)));
        }

    template<typename... T>
        void g(T&&... t) {  
            std::cout << "g invoked\n";
            (std::cout << ... << t) << '\n';
        }

    template<typename... T>
        void f(T&&... t) {
            g(forward<T>(t)...);
        }
}

namespace Templates10 {
    template<typename T>
        class Vec : public std::vector<T> {
            public:
                using std::vector<T>::vector;

                T& operator[](int i) {
                    return this->at(i);
                }

                const T& operator[](int i) const {
                    return std::vector<T>::at(i);
                }
        };
}

#endif
#endif
