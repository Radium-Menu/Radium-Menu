#pragma once

#include <algorithm>
#include <cmath>

namespace Radium::TpsBypass {

    inline constexpr float kVanillaTps = 240.f;
    inline constexpr float kMinTps = 60.f;
    inline constexpr float kMaxTps = 10000.f;
    inline constexpr int kMaxStepsPerFrame = 5000;

    struct StepResult {
        double delta;
        double extra;
        int steps;
    };

    inline StepResult computeStep(float dt, double extra, float timeWarp, float tps) {
        tps = std::clamp(tps, kMinTps, kMaxTps);
        float warp = timeWarp > 0.f ? std::min(timeWarp, 1.f) : 1.f;
        double timestep = static_cast<double>(warp) / tps;

        double total = static_cast<double>(std::max(dt, 0.f)) + extra;
        int steps = static_cast<int>(std::floor(total / timestep));

        if (steps > kMaxStepsPerFrame) {
            return { kMaxStepsPerFrame * timestep, 0.0, kMaxStepsPerFrame };
        }

        double delta = steps * timestep;
        return { delta, total - delta, steps };
    }

    bool isEnabled();
    void setEnabled(bool enabled);

    float getTps();
    void setTps(float tps);

    void load();
    void renderUi();

}
