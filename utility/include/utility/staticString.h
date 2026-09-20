#pragma once

#include <string_view>
#include <algorithm>
#include <cstddef>

template<std::size_t N>
struct StaticString {
    char value[N]{};

    constexpr StaticString(const char(&str)[N]) {
        std::copy_n(str, N, value);
    }

    constexpr operator std::string_view() const {
        return { value, N - 1 };
    }
};

template<class T>
struct _StaticStringInfo;

template<std::size_t N>
struct _StaticStringInfo<StaticString<N>> {
    static constexpr std::size_t size = N;
    static constexpr const char* data(const StaticString<N>& str) { return str.value; }
};

template<std::size_t N>
struct _StaticStringInfo<char[N]> {
    static constexpr std::size_t size = N;
    static constexpr const char* data(const char (&s)[N]) { return s; }
};

template <class T>
constexpr void append_one(char* buf, std::size_t& pos, const T& arg)
{
    constexpr std::size_t len = _StaticStringInfo<T>::size - 1;
    const char* src = _StaticStringInfo<T>::data(arg);
    for (std::size_t i = 0; i < len; ++i) buf[pos + i] = src[i];
    pos += len;
}