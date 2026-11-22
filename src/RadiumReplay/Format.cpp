#include "Format.hpp"

#include <cmath>
#include <cstring>
#include <iterator>

namespace Radium::RadiumReplay {

    namespace {

        constexpr char kMagic[6] = { 'R', 'A', 'D', 'I', 'U', 'M' };
        constexpr std::size_t kHeaderSize = 6 + 2 + 4 + 4 + 2;
        constexpr std::size_t kActionSize = 6;
        constexpr std::uint8_t kFlagDown = 1;
        constexpr std::uint8_t kFlagPlayer2 = 2;
        constexpr std::uint8_t kKnownFlags = kFlagDown | kFlagPlayer2;
        constexpr std::uint8_t kMinButton = 1;
        constexpr std::uint8_t kMaxButton = 3;

        void put8(std::vector<std::uint8_t>& out, std::uint8_t value) {
            out.push_back(value);
        }

        void put16(std::vector<std::uint8_t>& out, std::uint16_t value) {
            out.push_back(static_cast<std::uint8_t>(value & 0xFF));
            out.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
        }

        void put32(std::vector<std::uint8_t>& out, std::uint32_t value) {
            for (int shift = 0; shift < 32; shift += 8) {
                out.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
            }
        }

        struct Reader {
            std::vector<std::uint8_t> const& data;
            std::size_t pos = 0;

            std::size_t remaining() const { return data.size() - pos; }

            std::uint8_t u8() { return data[pos++]; }

            std::uint16_t u16() {
                std::uint16_t low = u8();
                std::uint16_t high = u8();
                return static_cast<std::uint16_t>(low | (high << 8));
            }

            std::uint32_t u32() {
                std::uint32_t value = 0;
                for (int shift = 0; shift < 32; shift += 8) {
                    value |= static_cast<std::uint32_t>(u8()) << shift;
                }
                return value;
            }
        };

        std::optional<Macro> fail(std::string* error, char const* message) {
            if (error) {
                *error = message;
            }
            return std::nullopt;
        }

    }

    std::vector<std::uint8_t> serialize(Macro const& macro) {
        std::string name = macro.levelName.substr(0, kMaxNameLength);

        std::vector<std::uint8_t> out;
        out.reserve(kHeaderSize + name.size() + 4 + macro.actions.size() * kActionSize);

        out.insert(out.end(), std::begin(kMagic), std::end(kMagic));
        put16(out, kFormatVersion);

        std::uint32_t tpsBits = 0;
        std::memcpy(&tpsBits, &macro.tps, sizeof(tpsBits));
        put32(out, tpsBits);

        put32(out, macro.frames);
        put16(out, static_cast<std::uint16_t>(name.size()));
        out.insert(out.end(), name.begin(), name.end());
        put32(out, static_cast<std::uint32_t>(macro.actions.size()));

        for (auto const& action : macro.actions) {
            put32(out, action.frame);
            put8(out, action.button);

            std::uint8_t flags = 0;
            if (action.down) flags |= kFlagDown;
            if (action.player2) flags |= kFlagPlayer2;
            put8(out, flags);
        }

        return out;
    }

    std::optional<Macro> parse(std::vector<std::uint8_t> const& data, std::string* error) {
        if (data.size() < kHeaderSize) {
            return fail(error, "File is too small");
        }

        Reader reader{ data };

        for (char expected : kMagic) {
            if (static_cast<char>(reader.u8()) != expected) {
                return fail(error, "Not a Radium Replay file");
            }
        }

        if (reader.u16() != kFormatVersion) {
            return fail(error, "Unsupported format version");
        }

        Macro macro;

        std::uint32_t tpsBits = reader.u32();
        std::memcpy(&macro.tps, &tpsBits, sizeof(tpsBits));
        if (!std::isfinite(macro.tps) || macro.tps <= 0.f) {
            return fail(error, "Invalid TPS");
        }

        macro.frames = reader.u32();

        std::size_t nameLength = reader.u16();
        if (nameLength > kMaxNameLength) {
            return fail(error, "Level name is too long");
        }
        if (reader.remaining() < nameLength + 4) {
            return fail(error, "File is truncated");
        }
        macro.levelName.assign(
            reinterpret_cast<char const*>(data.data() + reader.pos), nameLength
        );
        reader.pos += nameLength;

        std::uint32_t actionCount = reader.u32();
        if (reader.remaining() / kActionSize < actionCount) {
            return fail(error, "File is truncated");
        }
        if (reader.remaining() != static_cast<std::size_t>(actionCount) * kActionSize) {
            return fail(error, "File has extra data at the end");
        }

        macro.actions.reserve(actionCount);
        std::uint32_t previousFrame = 0;

        for (std::uint32_t i = 0; i < actionCount; ++i) {
            Action action;
            action.frame = reader.u32();
            action.button = reader.u8();
            std::uint8_t flags = reader.u8();

            if (action.button < kMinButton || action.button > kMaxButton) {
                return fail(error, "Invalid button");
            }
            if ((flags & ~kKnownFlags) != 0) {
                return fail(error, "Unknown action flags");
            }
            if (action.frame < previousFrame) {
                return fail(error, "Actions are out of order");
            }
            if (action.frame > macro.frames) {
                return fail(error, "Action is past the end of the replay");
            }

            action.down = (flags & kFlagDown) != 0;
            action.player2 = (flags & kFlagPlayer2) != 0;
            previousFrame = action.frame;
            macro.actions.push_back(action);
        }

        return macro;
    }

}
