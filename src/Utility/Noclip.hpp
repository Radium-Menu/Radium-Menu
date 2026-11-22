#pragma once

namespace Radium::Noclip {

    bool isEnabled();
    void setEnabled(bool enabled);

    int blockedDeaths();
    void countBlockedDeath();
    void resetBlockedDeaths();

    void load();
    void renderUi();

}
