#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class State { Playing, Win };

struct GameData {
  State m_state{State::Playing};
};

#endif