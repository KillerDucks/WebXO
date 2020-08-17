#include "Interception.hpp"

namespace WebX
{
    Interception::Interception(InterceptSettings interceptSettings)
    {

    }

    Interception::Interception(HTTPMethodTypes method, bool isBlocking)
    {
        // Some
    }

    // void Interception::HookSync(std::function<void()> func)
    // {

    // }

    void Interception::HookAsync(std::function<void()> func)
    {
        func();
    }
};