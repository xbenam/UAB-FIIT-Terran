#include "Player_AttackWeakest.h"

using namespace SparCraft;

Player_AttackWeakest::Player_AttackWeakest (const size_t & playerID) 
{
	_playerID = playerID;
}

void Player_AttackWeakest::getMoves(const GameState & state, std::vector<Action> & moveVec)
{
    MoveArray moves;
    ActionGenerators::GenerateCompassActions(state, _playerID, moves);

    moveVec.clear();
	for (size_t u(0); u<moves.numUnits(); ++u)
	{
		bool foundAction						(false);
		size_t actionMoveIndex					(0);
		size_t closestMoveIndex					(0);
		size_t actionLowestHP					(1000000);
		unsigned long long closestMoveDist		(std::numeric_limits<unsigned long long>::max());
		
		const Unit & ourUnit				(state.getUnit(_playerID, u));
		const Unit & closestUnit			(ourUnit.canHeal() ? AITools::GetClosestOurUnit(state, _playerID, u) : AITools::GetClosestEnemyUnit(state, _playerID, u));

		for (size_t m(0); m<moves.numMoves(u); ++m)
		{
			const Action move						(moves.getMove(u, m));
				
			if (move.type() == ActionTypes::ATTACK)
			{
				const Unit & target				(state.getUnit(state.getEnemy(move.getPlayerID()), move.getTargetID()));

				if ((size_t)target.currentHP() < actionLowestHP)
				{
					actionLowestHP = target.currentHP();
					actionMoveIndex = m;
					foundAction = true;
				}
			}
			else if (move.type() == ActionTypes::HEAL)
			{
				const Unit & target				(state.getUnit(move.getPlayerID(), move.getTargetID()));

				if ((size_t)target.currentHP() < actionLowestHP)
				{
					actionLowestHP = target.currentHP();
					actionMoveIndex = m;
					foundAction = true;
				}
			}
			else if (move.type() == ActionTypes::RELOAD)
			{
				if (ourUnit.canAttackTarget(closestUnit, state.getTime()))
				{
					closestMoveIndex = m;
					break;
				}
			}
			else if (move.type() == ActionTypes::MOVE)
			{
				Position ourDest				(ourUnit.x() + Constants::Move_Dir[move.getTargetID()][0], 
												 ourUnit.y() + Constants::Move_Dir[move.getTargetID()][1]);
				size_t dist						(closestUnit.getDistanceSqToPosition(ourDest, state.getTime()));

				if (dist < closestMoveDist)
				{
					closestMoveDist = dist;
					closestMoveIndex = m;
				}
			}
		}

		size_t bestMoveIndex(foundAction ? actionMoveIndex : closestMoveIndex);
			
		moveVec.push_back(moves.getMove(u, bestMoveIndex));
	}
}

PlayerPtr Player_AttackWeakest::clone()
{
    return PlayerPtr(new Player_AttackWeakest(*this));
}
