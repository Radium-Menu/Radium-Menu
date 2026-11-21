#include <Geode/Geode.hpp>

#include "Radium/MainLevelBypass.hpp"
#include "Radium/Noclip.hpp"
#include "Radium/Speedhack.hpp"
#include "Radium/TpsBypass.hpp"
#include "Radium/Ui.hpp"
#include "Radium/UnlockIcons.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    Radium::TpsBypass::load();
    Radium::Speedhack::load();
    Radium::Noclip::load();
    Radium::UnlockIcons::load();
    Radium::MainLevelBypass::load();
    Radium::Ui::init();
}
