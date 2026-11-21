#pragma once

#include <algorithm>

namespace Radium::Speedhack {

    inline constexpr float kMinSpeed = 0.01f;
    inline constexpr float kMaxSpeed = 10.f;

    inline float clampSpeed(float speed) {
        return std::clamp(speed, kMinSpeed, kMaxSpeed);
    }

    bool isEnabled();
    void setEnabled(bool enabled);

    float getSpeed();
    void setSpeed(float speed);

    void load();
    void renderUi();

}
