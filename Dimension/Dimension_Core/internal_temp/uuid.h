#ifndef DIMENSION_UUID_H
#define DIMENSION_UUID_H

#include <array>
#include <cstdint>
#include <span>

// #include "strings/string_literal.h"

namespace dimension::detail
{
    [[nodiscard]]
    constexpr std::uint64_t splitmix64_new(std::uint64_t& x) noexcept {
        x += 0x9E3779B97F4A7C15ull;
        std::uint64_t z = x;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }

    /* Compile-time 128-bit UUID derived from a string literal.
    Includes the trailing '\0' exactly like your previous code. */
    template<std::size_t N>
    [[nodiscard]]
    consteval std::array<std::uint64_t, 2>
    uuid128_of_new(const char (&lit)[N]) noexcept {
        std::uint64_t lo = 0;
        std::uint64_t hi = 0x3243F6A8885A308Dull; // fixed constant

        for (std::size_t i = 0; i < N; ++i) {      // includes '\0'
            lo ^= static_cast<std::uint64_t>(static_cast<unsigned char>(lit[i]));
            lo  = splitmix64_new(lo);
            hi ^= lo;
            hi  = splitmix64_new(hi);
        }
        return {lo, hi};
    }

    
    //Optional: a constexpr (not consteval) overload for std::string_view,
    //in case you ever want to hash non-literals.
    [[nodiscard]]
    constexpr std::array<std::uint64_t, 2>
    uuid128_of_new(std::string_view sv) noexcept {
        std::uint64_t lo = 0;
        std::uint64_t hi = 0x3243F6A8885A308Dull;
        // If you want parity with the literal version, include a terminating 0:
        for (char c : sv) {
            lo ^= static_cast<std::uint64_t>(static_cast<unsigned char>(c));
            lo  = splitmix64_new(lo);
            hi ^= lo;
            hi  = splitmix64_new(hi);
        }
        // emulate “including the '\0'”
        {
            lo ^= 0;
            lo  = splitmix64_new(lo);
            hi ^= lo;
            hi  = splitmix64_new(hi);
        }
        return {lo, hi};
    }
    
    template<typename T>
    struct unit_uuid{
        static constexpr std::array<std::uint64_t, 2> value = uuid128_of_new(T::name_view);
    };

    template<typename T>
    inline constexpr std::array<std::uint64_t, 2> unit_uuid_v = unit_uuid<T>::value;

} // namespace dimension::detail




#endif // DIMENSION_UUID_H
