#pragma once

#include "Common.h"
#include "Player.h"

namespace SparCraft
{
/*----------------------------------------------------------------------
 | Attack HighestDPS Player No Overkill
 |----------------------------------------------------------------------
 | Chooses an action with following priority:
 | 1) If it can attack, ATTACK highest DPS/HP enemy unit to overkill
 | 2) If it cannot attack:
 |    a) If it is in range to attack an enemy, WAIT until attack
 |    b) If it is not in range of enemy, MOVE towards closest
 `----------------------------------------------------------------------*/
class Player_NOKDPS : public Player
{
public:
	Player_NOKDPS (const size_t & playerID);
	void getMoves(const GameState & state, std::vector<Action> & moveVec);
	virtual PlayerPtr clone();
};
}