#pragma once
#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <utility>
#include <cwchar>     // wint_t
#include <cinttypes>  // intmax_t
#include <cstdio>

namespace printfx {

// ------------ fixed_string ------------
template <std::size_t N>
struct fixed_string {
    char value[N];

    consteval fixed_string(const char (&s)[N]) {
        std::copy_n(s, N, value);
    }

    static constexpr std::size_t size = N;
    constexpr char operator[](std::size_t i) const { return value[i]; }
};
template <std::size_t N> fixed_string(const char (&)[N]) -> fixed_string<N>;

// ------------ typelist helpers ------------
template <class... Ts> struct type_list {};
template <class List, class T> struct tl_append;
template <class... Ts, class T>
struct tl_append<type_list<Ts...>, T> { using type = type_list<Ts..., T>; };

template <class List> struct tl_size;
template <class... Ts>
struct tl_size<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

// ------------ length/spec mapping ------------
enum class length_mod { none, hh, h, l, ll, j, z, t, L };

template <length_mod, char> struct spec_map; // primary (left undefined for bad combos)

// signed d/i
template <> struct spec_map<length_mod::none, 'd'> { using type = int; };
template <> struct spec_map<length_mod::none, 'i'> { using type = int; };
template <> struct spec_map<length_mod::hh,  'd'> { using type = int; };
template <> struct spec_map<length_mod::hh,  'i'> { using type = int; };
template <> struct spec_map<length_mod::h,   'd'> { using type = int; };
template <> struct spec_map<length_mod::h,   'i'> { using type = int; };
template <> struct spec_map<length_mod::l,   'd'> { using type = long; };
template <> struct spec_map<length_mod::l,   'i'> { using type = long; };
template <> struct spec_map<length_mod::ll,  'd'> { using type = long long; };
template <> struct spec_map<length_mod::ll,  'i'> { using type = long long; };
template <> struct spec_map<length_mod::j,   'd'> { using type = std::intmax_t; };
template <> struct spec_map<length_mod::j,   'i'> { using type = std::intmax_t; };
template <> struct spec_map<length_mod::t,   'd'> { using type = std::ptrdiff_t; };
template <> struct spec_map<length_mod::t,   'i'> { using type = std::ptrdiff_t; };

// unsigned u/o/x/X
template <> struct spec_map<length_mod::none, 'u'> { using type = unsigned; };
template <> struct spec_map<length_mod::none, 'o'> { using type = unsigned; };
template <> struct spec_map<length_mod::none, 'x'> { using type = unsigned; };
template <> struct spec_map<length_mod::none, 'X'> { using type = unsigned; };
template <> struct spec_map<length_mod::hh,   'u'> { using type = unsigned; };
template <> struct spec_map<length_mod::hh,   'o'> { using type = unsigned; };
template <> struct spec_map<length_mod::hh,   'x'> { using type = unsigned; };
template <> struct spec_map<length_mod::hh,   'X'> { using type = unsigned; };
template <> struct spec_map<length_mod::h,    'u'> { using type = unsigned; };
template <> struct spec_map<length_mod::h,    'o'> { using type = unsigned; };
template <> struct spec_map<length_mod::h,    'x'> { using type = unsigned; };
template <> struct spec_map<length_mod::h,    'X'> { using type = unsigned; };
template <> struct spec_map<length_mod::l,    'u'> { using type = unsigned long; };
template <> struct spec_map<length_mod::l,    'o'> { using type = unsigned long; };
template <> struct spec_map<length_mod::l,    'x'> { using type = unsigned long; };
template <> struct spec_map<length_mod::l,    'X'> { using type = unsigned long; };
template <> struct spec_map<length_mod::ll,   'u'> { using type = unsigned long long; };
template <> struct spec_map<length_mod::ll,   'o'> { using type = unsigned long long; };
template <> struct spec_map<length_mod::ll,   'x'> { using type = unsigned long long; };
template <> struct spec_map<length_mod::ll,   'X'> { using type = unsigned long long; };
template <> struct spec_map<length_mod::j,    'u'> { using type = std::uintmax_t; };
template <> struct spec_map<length_mod::j,    'o'> { using type = std::uintmax_t; };
template <> struct spec_map<length_mod::j,    'x'> { using type = std::uintmax_t; };
template <> struct spec_map<length_mod::j,    'X'> { using type = std::uintmax_t; };
template <> struct spec_map<length_mod::z,    'u'> { using type = std::size_t; };
template <> struct spec_map<length_mod::z,    'o'> { using type = std::size_t; };
template <> struct spec_map<length_mod::z,    'x'> { using type = std::size_t; };
template <> struct spec_map<length_mod::z,    'X'> { using type = std::size_t; };
template <> struct spec_map<length_mod::t,    'u'> { using type = std::make_unsigned_t<std::ptrdiff_t>; };
template <> struct spec_map<length_mod::t,    'o'> { using type = std::make_unsigned_t<std::ptrdiff_t>; };
template <> struct spec_map<length_mod::t,    'x'> { using type = std::make_unsigned_t<std::ptrdiff_t>; };
template <> struct spec_map<length_mod::t,    'X'> { using type = std::make_unsigned_t<std::ptrdiff_t>; };

// floating fFeEgGaA (no constrained partials; enumerate explicitly)
template <> struct spec_map<length_mod::none, 'f'> { using type = double; };
template <> struct spec_map<length_mod::none, 'F'> { using type = double; };
template <> struct spec_map<length_mod::none, 'e'> { using type = double; };
template <> struct spec_map<length_mod::none, 'E'> { using type = double; };
template <> struct spec_map<length_mod::none, 'g'> { using type = double; };
template <> struct spec_map<length_mod::none, 'G'> { using type = double; };
template <> struct spec_map<length_mod::none, 'a'> { using type = double; };
template <> struct spec_map<length_mod::none, 'A'> { using type = double; };

template <> struct spec_map<length_mod::L, 'f'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'F'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'e'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'E'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'g'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'G'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'a'> { using type = long double; };
template <> struct spec_map<length_mod::L, 'A'> { using type = long double; };

// c / s / p
template <> struct spec_map<length_mod::none, 'c'> { using type = int; };     // promoted
template <> struct spec_map<length_mod::l,    'c'> { using type = wint_t; };

template <> struct spec_map<length_mod::none, 's'> { using type = const char*; };
template <> struct spec_map<length_mod::l,    's'> { using type = const wchar_t*; };

template <> struct spec_map<length_mod::none, 'p'> { using type = const void*; };

// ------------ tiny utilities ------------
consteval bool is_flag(char c) {
    return c=='-'||c=='+'||c==' '||c=='#'||c=='0'||c=='\'';
}
template <fixed_string Fmt, std::size_t I>
consteval std::size_t skip_flags() {
    if constexpr (I < Fmt.size && is_flag(Fmt[I])) return skip_flags<Fmt, I+1>();
    else return I;
}
template <fixed_string Fmt, std::size_t I>
consteval std::size_t skip_digits() {
    if constexpr (I < Fmt.size && (Fmt[I] >= '0' && Fmt[I] <= '9')) return skip_digits<Fmt, I+1>();
    else return I;
}
template <fixed_string Fmt, std::size_t I>
consteval length_mod parse_length_mod() {
    if constexpr (I+1 < Fmt.size && Fmt[I]=='h' && Fmt[I+1]=='h') return length_mod::hh;
    if constexpr (I+1 < Fmt.size && Fmt[I]=='l' && Fmt[I+1]=='l') return length_mod::ll;
    if constexpr (I < Fmt.size && Fmt[I]=='h') return length_mod::h;
    if constexpr (I < Fmt.size && Fmt[I]=='l') return length_mod::l;
    if constexpr (I < Fmt.size && Fmt[I]=='j') return length_mod::j;
    if constexpr (I < Fmt.size && Fmt[I]=='z') return length_mod::z;
    if constexpr (I < Fmt.size && Fmt[I]=='t') return length_mod::t;
    if constexpr (I < Fmt.size && Fmt[I]=='L') return length_mod::L;
    return length_mod::none;
}
template <fixed_string Fmt, std::size_t I>
consteval std::size_t after_length() {
    if constexpr (I+1 < Fmt.size && ((Fmt[I]=='h' && Fmt[I+1]=='h') || (Fmt[I]=='l' && Fmt[I+1]=='l')))
        return I+2;
    else if constexpr (I < Fmt.size && (Fmt[I]=='h'||Fmt[I]=='l'||Fmt[I]=='j'||Fmt[I]=='z'||Fmt[I]=='t'||Fmt[I]=='L'))
        return I+1;
    else
        return I;
}

// ------------ parser ------------
template <fixed_string Fmt, std::size_t I, class Accum>
struct parse_until;

template <fixed_string Fmt, std::size_t I, class Accum>
struct parse_percent {
    static consteval auto run() {
        // 1) Flags
        constexpr std::size_t I0 = skip_flags<Fmt, I>();

        // 2) Width
        if constexpr (I0 < Fmt.size && Fmt[I0] == '*') {
            using Awidth = typename tl_append<Accum, int>::type;
            // Recurse continuing the SAME % sequence after consuming the star
            return parse_percent<Fmt, I0 + 1, Awidth>::run();
        } else {
            // Width digits (maybe zero digits)
            constexpr std::size_t I1 = skip_digits<Fmt, I0>();

            // 3) Precision block
            if constexpr (I1 < Fmt.size && Fmt[I1] == '.') {
                constexpr std::size_t Iprec = I1 + 1;

                if constexpr (Iprec < Fmt.size && Fmt[Iprec] == '*') {
                    using Aprec = typename tl_append<Accum, int>::type;
                    return parse_percent<Fmt, Iprec + 1, Aprec>::run();
                } else {
                    constexpr std::size_t I2 = skip_digits<Fmt, Iprec>();
                    return parse_percent<Fmt, I2, Accum>::run();
                }
            } else {
                // 4) Length -> Specifier
                constexpr auto L  = parse_length_mod<Fmt, I1>();
                constexpr std::size_t J = after_length<Fmt, I1>();

                static_assert(J < Fmt.size, "unterminated % sequence");
                constexpr char spec = Fmt[J];

                if constexpr (spec == '%') {
                    // literal %%
                    return parse_until<Fmt, J + 1, Accum>::run();
                } else {
                    using T = typename spec_map<
                        // Only 'c' and 's' accept 'l'. 'p' never takes a length.
                        (spec == 'p' ? length_mod::none : L),
                        spec
                    >::type;

                    using A2 = typename tl_append<Accum, T>::type;
                    return parse_until<Fmt, J + 1, A2>::run();
                }
            }
        }
    }
};

template <fixed_string Fmt, std::size_t I, class Accum>
struct parse_until {
    static consteval auto run() {
        if constexpr (I >= Fmt.size) {
            return Accum{};
        } else if constexpr (Fmt[I] == '%') {
            if constexpr (I + 1 < Fmt.size && Fmt[I + 1] == '%') {
                return parse_until<Fmt, I + 2, Accum>::run(); // literal %%
            } else {
                return parse_percent<Fmt, I + 1, Accum>::run(); // after '%'
            }
        } else {
            return parse_until<Fmt, I + 1, Accum>::run();
        }
    }
};

template <fixed_string Fmt>
consteval auto expected_arg_types() {
    return parse_until<Fmt, 0, type_list<>>::run();
}

// ------------ checking & API ------------
template <class From, class To>
concept cvref_convertible_to = std::same_as<
    std::remove_cv<std::decay_t<From>>,
    std::remove_cv<std::decay_t<To>>
>;

template <class ExpectedList, class... Given>
struct matches_list;

template <class... Expected, class... Given>
struct matches_list<type_list<Expected...>, Given...>
  : std::bool_constant<(sizeof...(Expected) == sizeof...(Given)) &&
                       (cvref_convertible_to<Given, Expected> && ...)> {};

template <fixed_string Fmt, class... Args>
concept format_matches = []() consteval {
    using expected = decltype(expected_arg_types<Fmt>());
    return matches_list<expected, Args...>::value;
}();

template <fixed_string Fmt>
using expected_types_t = decltype(expected_arg_types<Fmt>());

template<fixed_string Fmt, typename... Args>
    requires format_matches<Fmt, Args...>
constexpr void printfx_impl(Args&&... args) {
    std::printf(Fmt.value, std::forward<Args>(args)...);
}

#define PRINTFX(fmt_literal, ...) \
    ::printfx::printfx_impl< ::printfx::fixed_string{ fmt_literal } >( __VA_ARGS__ )

} // namespace printfx
