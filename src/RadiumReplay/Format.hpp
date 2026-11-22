#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Radium::RadiumReplay {

    inline constexpr std::uint16_t kFormatVersion = 1;
    inline constexpr char const* kExtension = ".radium1";
    inline constexpr std::size_t kMaxNameLength = 255;

    struct Action {
        std::uint32_t frame = 0;
        std::uint8_t button = 1;
        bool down = false;
        bool player2 = false;
    };

    struct Macro {
        float tps = 240.f;
        std::uint32_t frames = 0;
        std::string levelName;
        std::vector<Action> actions;
    };

    std::vector<std::uint8_t> serialize(Macro const& macro);

    std::optional<Macro> parse(std::vector<std::uint8_t> const& data, std::string* error = nullptr);

}
