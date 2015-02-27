#pragma once

#include "Common.h"
#include "BWTA.h"

namespace UAlbertaBot
{
struct UnitInfo
{
	// we need to store all of this data because if the unit is not visible, we
	// can't reference it from the unit pointer

	int					unitID;
	int					lastHealth;
    BWAPI::PlayerInterface *      player;
	BWAPI::UnitInterface *		    unit;
	BWAPI::Position		lastPosition;
	BWAPI::UnitType		type;
    bool                completed;

	bool canCloak() const
	{
		return type == BWAPI::UnitTypes::Protoss_Dark_Templar
			|| type == BWAPI::UnitTypes::Terran_Wraith
			|| type == BWAPI::UnitTypes::Zerg_Lurker;
	}

	bool isFlyer() const
	{
		return type.isFlyer();
	}

	bool isDetector() const
	{
		return type.isDetector();
	}

	bool isWorker() const
	{
		return type.isWorker();
	}
	UnitInfo()
		: unitID(0)
		, lastHealth(0)
        , player(NULL)
		, unit(NULL)
		, lastPosition(BWAPI::Positions::None)
		, type(BWAPI::UnitTypes::None)
        , completed(false)
	{

	}

	UnitInfo(int id, BWAPI::UnitInterface* u, BWAPI::Position last, BWAPI::UnitType t) 
        : unitID(id)
        , lastHealth(u->getHitPoints() + u->getShields())
        , player(u->getPlayer())
		, unit(u)
		, lastPosition(last)
		, type(t)
        , completed(u->isCompleted())
	{
		
	}

	const bool operator == (BWAPI::UnitInterface* unit) const
	{
		return unitID == unit->getID();
	}

	const bool operator == (const UnitInfo & rhs) const
	{
		return (unitID == rhs.unitID);
	}

	const bool operator < (const UnitInfo & rhs) const
	{
		return (unitID < rhs.unitID);
	}
};

typedef std::vector<UnitInfo> UnitInfoVector;
typedef std::map<BWAPI::UnitInterface*, UnitInfo> UIMap;
typedef UIMap::iterator UIMapIter;
typedef UIMap::const_iterator ConstUIMapIter;
#define FOR_EACH_UIMAP(ITER, MAP) for (UIMapIter ITER(MAP.begin()); ITER != MAP.end(); ITER++)
#define FOR_EACH_UIMAP_CONST(ITER, MAP) for (ConstUIMapIter ITER(MAP.begin()); ITER != MAP.end(); ITER++)

class UnitData {

	std::map<BWAPI::UnitInterface*, UnitInfo>		unitMap;

	std::vector<int>						numDeadUnits;
	std::vector<int>						numUnits;
	std::vector<int>						numCompletedUnits;

	int										mineralsLost;
	int										gasLost;

	const bool badUnitInfo(const UnitInfo & ui) const;

public:

	UnitData();
	~UnitData() {}

	void	updateUnit(BWAPI::UnitInterface* unit);
	void	removeUnit(BWAPI::UnitInterface* unit);
	void	removeBadUnits();

	void	getCloakedUnits(std::set<UnitInfo> & v)				const;
	void	getDetectorUnits(std::set<UnitInfo> & v)			const;
	void	getFlyingUnits(std::set<UnitInfo> & v)				const;
	bool	hasCloakedUnits()									const;
	bool	hasDetectorUnits()									const;

	int		getGasLost()										const	{ return gasLost; }
	int		getMineralsLost()									const	{ return mineralsLost; }
	int		getNumUnits(BWAPI::UnitType t)						const	{ return numUnits[t.getID()]; }
	int		getNumCompletedUnits(BWAPI::UnitType t)				const	{ return numCompletedUnits[t.getID()]; }
	int		getNumDeadUnits(BWAPI::UnitType t)					const	{ return numDeadUnits[t.getID()]; }
	const	std::map<BWAPI::UnitInterface*, UnitInfo> & getUnits()		const	{ return unitMap; }
};
}