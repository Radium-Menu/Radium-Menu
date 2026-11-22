#pragma once

#include <cstdint>

#include "Format.hpp"

class GJBaseGameLayer;

namespace Radium::RadiumReplay {

    enum class Mode {
        Idle,
        Recording,
        Playing,
    };

    Mode mode();
    std::uint32_t currentFrame();

    void beginStep(GJBaseGameLayer* layer);
    void endStep(GJBaseGameLayer* layer);
    bool interceptButton(bool down, int button, bool isPlayer1);

    void onLevelReset();
    void onLevelComplete();
    void onLevelQuit();

    void renderUi();

}
