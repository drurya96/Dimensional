#ifndef DIMENSION_UUID_H
#define DIMENSION_UUID_H

#include <array>
#include <cstdint>
#include <span>

#include "strings/string_literal.h"

namespace dimension::detail
{
    // Simple, fast, fully‑constexpr 64‑bit mixer (SplitMix64)
    [[nodiscard]]
    constexpr std::uint64_t splitmix64(std::uint64_t& x) noexcept
    {
        x += 0x9E3779B97F4A7C15ull;
        std::uint64_t z = x;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }

    /// Compile‑time 128‑bit UUID derived from a `string_literal<N>`.
    template<std::size_t N>
    [[nodiscard]]
    consteval std::array<std::uint64_t,2>
    uuid128_of(const string_literal<N>& lit) noexcept
    {
        std::uint64_t lo = 0;
        std::uint64_t hi = 0x3243F6A8885A308Dull;   // fixed constant

        for (char c : lit.value)                    // includes the '\0'
        {
            lo ^= static_cast<std::uint64_t>(static_cast<unsigned char>(c));
            lo  = splitmix64(lo);

            hi ^= lo;
            hi  = splitmix64(hi);
        }
        return { lo, hi };
    }
} // namespace dimension::detail

#endif // DIMENSION_UUID_H
