#pragma once
#include <variant>
#include <type_traits>

template <typename Variant, typename T, std::size_t Index = 0>
constexpr std::size_t variant_index() {
    if constexpr (Index >= std::variant_size_v<Variant>) {
        return static_cast<std::size_t>(-1);
    } else if constexpr (std::is_same_v<std::variant_alternative_t<Index, Variant>, T>) {
        return Index;
    } else {
        return variant_index<Variant, T, Index + 1>();
    }
}