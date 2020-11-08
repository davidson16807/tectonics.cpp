#include <iostream>
template<typename T>
struct CtorDtorTracer
{
    T value;
    CtorDtorTracer()
    {
        std::cout << "default ctor" << std::endl;
    }
    CtorDtorTracer(CtorDtorTracer& other)
    {
        std::cout << "copy ctor" << std::endl;
    }
    CtorDtorTracer(T value) : value(value)
    {
        std::cout << "parameter ctor" << std::endl;
    }
    ~CtorDtorTracer()
    {
        std::cout << "default dtor" << std::endl;
    }
};