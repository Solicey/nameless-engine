#pragma once

#include <concepts>
#include <type_traits>

namespace NL
{
    class ComponentBase
    {
    public:
        ComponentBase() = default;
        virtual ~ComponentBase() {}
    };

    template<class T>
    concept Component = std::is_base_of_v<ComponentBase, T>;
}