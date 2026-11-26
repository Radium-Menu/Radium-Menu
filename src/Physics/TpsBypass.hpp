
#pragma once


#include <algorithm>
#include <cmath>


namespace Radium::TpsBypass {


    inline constexpr float kVanillaTps = 240.f;
    inline constexpr float kMinTps = 240.f;
    inline constexpr float kMaxTps = 10000.f;
    inline constexpr int kMaxTicksPerFrame = 600;


    inline constexpr double kRealTimeBudget = 0.010;


    struct TickPlan {
        int count;
        double overflow;
        double tick;
    };


    inline double tickLength(float tps, float timeWarp) {
        double warp = timeWarp > 0.f ? std::min(timeWarp, 1.f) : 1.f;
        return warp / std::clamp(tps, kMinTps, kMaxTps);
    }


    inline TickPlan planTicks(double dt, double overflow, double tick) {
        double total = std::max(dt, 0.0) + overflow;
        int count = static_cast<int>(std::floor(total / tick));


        if (count > kMaxTicksPerFrame) {
            return { kMaxTicksPerFrame, 0.0, tick };
        }


        return { count, total - count * tick, tick };
    }


    inline TickPlan planRealTime(double dt, double overflow, double tick, double cost) {
        double total = std::max(dt, 0.0) + overflow;
        int count = static_cast<int>(std::floor(total / tick));
        int affordable = std::clamp(
            static_cast<int>(std::floor(kRealTimeBudget / std::max(cost, 1e-6))),
            1,
            kMaxTicksPerFrame
        );


        if (count <= affordable) {
            return { count, total - count * tick, tick };
        }


        return { affordable, 0.0, total / affordable };
    }


    bool isEnabled();
    void setEnabled(bool enabled);


    bool isRealTime();
    void setRealTime(bool realTime);


    float getTps();
    void setTps(float tps);


    void load();
    void renderUi();


}
