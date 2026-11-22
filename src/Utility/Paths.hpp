#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
#define RADIUM_PATHS_HAS_FMT 1
#endif

namespace Radium::Paths {

    inline std::filesystem::path fromUtf8(std::string const& text) {
        return std::filesystem::path(std::u8string(text.begin(), text.end()));
    }

    template <class T>
    inline constexpr bool kAlwaysFalse = false;

    template <class P>
    std::filesystem::path toStd(P const& path) {
        if constexpr (std::is_convertible_v<P const&, std::filesystem::path>) {
            return path;
        } else if constexpr (requires { std::string(path.string()); }) {
            return fromUtf8(std::string(path.string()));
        } else if constexpr (requires { std::string(path.str()); }) {
            return fromUtf8(std::string(path.str()));
        } else if constexpr (requires { std::string(path.toString()); }) {
            return fromUtf8(std::string(path.toString()));
        } else if constexpr (requires { std::string(path.c_str()); }) {
            return fromUtf8(std::string(path.c_str()));
        } else if constexpr (requires { std::string(path); }) {
            return fromUtf8(std::string(path));
        }
#ifdef RADIUM_PATHS_HAS_FMT
        else if constexpr (requires { fmt::format("{}", path); }) {
            return fromUtf8(fmt::format("{}", path));
        }
#endif
        else {
            static_assert(kAlwaysFalse<P>, "Cannot convert this path type to std::filesystem::path");
        }
    }

}
