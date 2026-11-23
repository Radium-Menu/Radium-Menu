#include <Geode/Geode.hpp>

#include "Assist/Speedhack.hpp"
#include "Physics/TpsBypass.hpp"
#include "Utility/MainLevelBypass.hpp"
#include "Utility/Noclip.hpp"
#include "Utility/Ui.hpp"
#include "Utility/UnlockIcons.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    Radium::TpsBypass::load();
    Radium::Speedhack::load();
    Radium::Noclip::load();
    Radium::UnlockIcons::load();
    Radium::MainLevelBypass::load();
    Radium::Ui::init();
}
