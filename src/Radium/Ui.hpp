#pragma once

#include <functional>
#include <string>

namespace Radium::Ui {

    void init();
    void toggle();
    bool isOpen();
    bool isReady();
    bool isTyping();
    void addTab(std::string name, std::function<void()> draw);

}
