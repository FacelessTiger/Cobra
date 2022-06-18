#pragma once

struct Void {};

template <typename ...> struct concat;

template <template <typename ...> class List, typename T>
struct concat<List<Void>, T>
{
    typedef List<T> type;
};

template <template <typename ...> class List, typename ...Types, typename T>
struct concat<List<Types...>, T>
{
    typedef List<Types..., T> type;
};

template <typename...> struct TypeList {};

template <>
struct TypeList<Void> { };
typedef TypeList<Void> TypelistVoid;
#define TYPE_LIST TypelistVoid