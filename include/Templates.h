/**
 * Copyright (c) 2026 Alexey Gavrilov <alexey.gavrilov@mail.com>
 *
 * This file is part of ObjectiveOS.
 *
 * ObjectiveOS is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ObjectiveOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ObjectiveOS. If not, see <https://www.gnu.org/licenses/>. 
 */

#ifndef TEMPLATES_H
#define TEMPLATES_H


#include <Types.h>

template<typename T, T val>
struct constant
{
    static constexpr T value = val;
    T& operator()() {return value;}
};

template<typename T, typename U>
struct isSame
{
    using cnst = constant<bool, false>;
    bool static inline constexpr val(){return cnst::value;};
    constexpr operator bool() const {return cnst::value;}
    bool constexpr operator()() {return cnst::value;}
};

template<typename T>
struct isSame<T, T>
{
    using cnst = constant<bool, true>;
    constexpr operator bool() const {return cnst::value;}
    bool static inline constexpr val(){return cnst::value;};
    bool constexpr operator()() {return cnst::value;}
};

template <typename ... params>
struct parametersOperations
{
    template<typename Head, typename ... Tail>
    using first = Head;

    template<typename Head, typename ... Tail>
    using tail = parametersOperations<Tail ...>;

    // Append operation
    template<typename Head, typename ... Tail>
    struct append
    {
        using result = parametersOperations<params ..., Head>::template append<Tail ...>::result;
    };

    template<template <typename ... NestedList> typename NestedType, typename ... NestedList, typename ... OuterList>
    requires(isSame<NestedType<>, parametersOperations<>>::val())
    struct append<NestedType<NestedList ...>, OuterList ...>
    {
        using result = parametersOperations<params..., NestedList ...>::template append<OuterList ...>::result;
    };

    template<template <typename ... NestedList> typename NestedType, typename ... NestedList>
    requires(isSame<NestedType<>, parametersOperations<>>::val())
    struct append<NestedType<NestedList ...>>
    {
        using result = parametersOperations<params..., NestedList ...>;
    };

    template<typename One>
    struct append<One> {using result = parametersOperations<params ..., One>;};
};

template< unsigned I, typename T>
struct cut
{
    using process = cut<I - 1, typename T::baseType>::process;
};

template<typename T>
struct cut<0, T>
{
    using process = cut<0, T>;
    using result = T;
};

template<uint16 N, typename Head, typename ... Args>
struct reper
{
    using toList = typename reper<N-1, Head, Head, Args ...>::toList;
};

template<uint16 N, typename Type>
struct repeat
{
    using toList = typename reper<N-1, Type>::toList;
};

template<typename Type>
struct repeat<0, Type> {};

template<typename Head, typename ... Args>
struct reper<0, Head, Args ...>
{
    using toList = parametersOperations<Head, Args ...>;
};

template<typename Root, typename toType>
struct switchRoot {using result = toType;};

template<template <typename ... RootList> typename RootType, template <typename ... toTypeListArgs> typename toType, typename ... RootList, typename ... toTypeListArgs>
struct switchRoot<RootType<RootList ...>, toType<toTypeListArgs ...>>{using result = toType<RootList ...>;};

template<typename arg, template<typename ... toTypeListArgs> typename toType, typename ... toTypeListArgs>
struct switchRoot<arg, toType<toTypeListArgs ...>>{using result = toType<arg>;};

template<typename arg, typename ... args>
struct parameters
{
    public:
        using baseType = parameters<args ...>;
        using types = parametersOperations<>::template append<arg, typename baseType::types>::result;
        template<template <typename ... > typename T>
        using submitTo = switchRoot<types, T<>>::result;
};

template<typename arg>
struct parameters<arg>
{
    public:
        using types = arg;
        template<template <typename ... > typename T>
        using submitTo = switchRoot<types, T<>>::result;
};
#endif
