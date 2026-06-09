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

#ifndef CONTAINERSTUPLE_H
#define CONTAINERSTUPLE_H


#include <Templates.h>

template<typename Head = void, typename ... Tail>
class Tuple : public Tuple<Tail ...>
{
    private:
        using valueType = Head;
        valueType value;
    public:
        using baseType = Tuple<Tail ...>;
        Tuple(Head val_, Tail ... args) : baseType(args ...), value(val_) {};

        static constexpr unsigned int length() {return sizeof...(Tail) + 1;}
        valueType getValue() {return value;};

        template<uint16 indx>
        auto get()
        {
            using returnType = cut<indx, Tuple>::process::result;
            return static_cast<returnType>(*this);
        };

        /* 
        Create the class/struct before using the method
        F is this class/struct to pass in method trought template argument.
        F must contain a static method/s named "perform"
        that will perform something action for every value in tuple object.
        F must contain as many implementations
        as there are different types in the template
        F::perform() for the first argument accept tuple value,
        next arguments optional
        */
        template<typename F, uint16 If = length() - 1, uint16 ... I,
                 typename Mid = void, typename ... T>
        void foreach(T ... arg)
        {
            if constexpr(If != NULL)
            {
                foreach<F, If - 1, If, I ...>(arg ...);
            }
            else
            {
                (F::perform(get<If>().getValue(), arg ...));
                (F::perform(get<I>().getValue(), arg ...), ...);
            }
        };
};

template<>
class Tuple<>
{
    public:
        static constexpr unsigned int length() {return 0;}
};

template<typename Head>
class Tuple<Head>
{
    private:
        using valueType = Head;
        valueType value;
    public:
        using baseType = Tuple<>;
        Tuple(Head val_) : value(val_) {};
        Tuple() : value(valueType()) {};

        static constexpr unsigned int length() {return 1;}
        valueType getValue() {return value;};

        template<uint16 indx>
        auto get()
        {
            if constexpr (indx == NULL) return *this;
            else static_assert(!indx, "Tuple::get<indx>: Out of bounds");
        };

        /*
        Create the class/struct before using the method
        F is this class/struct to pass in method trought template argument.
        F must contain a static method/s named "perform"
        that will perform something action for every value in tuple object.
        F must contain as many implementations
        as there are different types in the template
        F::perform() for the first argument accept tuple value,
        next arguments optional
        */
        template<typename F, typename ... T>
        void foreach(T ... arg)
        {
            F::perform(value, arg ...);
        };
};

#endif
