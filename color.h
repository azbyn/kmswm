#pragma once
#include <assert.h>
#include <stdint.h>

namespace azbyn {
class Color {
    uint32_t raw = 0xff000000;

public:
    constexpr Color() = default;
    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
            : raw((a << 24) | (r << 16) | (g << 8) | b) {}
    constexpr Color(uint32_t raw) : raw(raw) {}
    constexpr operator uint32_t() const { return raw; }

    constexpr uint8_t A() const { return (raw >> 24) & 0xff; }
    constexpr uint8_t R() const { return (raw >> 16) & 0xff; }
    constexpr uint8_t G() const { return (raw >> 8) & 0xff; }
    constexpr uint8_t B() const { return raw & 0xff; }
};

namespace colors {
// clang-format off
static constexpr Color white   = Color(0xff, 0xff, 0xff);
static constexpr Color gray    = Color(0x7f, 0x7f, 0x7f);
static constexpr Color black   = Color(0x00, 0x00, 0x00);
static constexpr Color red     = Color(0xff, 0x00, 0x00);
static constexpr Color green   = Color(0x00, 0xff, 0x00);
static constexpr Color blue    = Color(0x00, 0x00, 0xff);
static constexpr Color cyan    = Color(0x00, 0xff, 0xff);
static constexpr Color magenta = Color(0xff, 0x00, 0xff);
static constexpr Color yellow  = Color(0xff, 0xff, 0x00);
// clang-format on
} // namespace colors
// sanity checks
static_assert(sizeof(Color) == 4);
static_assert(Color(1, 2, 3, 4).R() == 1);
static_assert(Color(1, 2, 3, 4).G() == 2);
static_assert(Color(1, 2, 3, 4).B() == 3);
static_assert(Color(1, 2, 3, 4).A() == 4);
static_assert(static_cast<uint32_t>(Color(1, 2, 3, 4)) == 0x04010203);
} // namespace azbyn
