#pragma once

namespace Radium::Vbot {

    inline constexpr const char* kModId = "t1fy.vbot";

    bool isAvailable();

    void startRecording();
    void stopRecording();
    void startReplay();
    void stopReplay();
    void startRender();
    void stopRender();

    void renderUi();

}
