#ifndef _RESSOURCEMANAGER_
#define _RESSOURCEMANAGER_
#include <BWAPI.h>
#include <BWTA.h>
#include <queue>
#include <ctime>

class RessourceManager
{
	typedef std::pair<BWAPI::UnitType, std::pair<int, int>> UnitPair;
	class comparison
	{
	public:
		comparison() {}
		bool operator() (const UnitPair& lhs, const UnitPair& rhs) const
		{
			if(lhs.second.first != rhs.second.first)
			{
				return lhs.second.first < rhs.second.first;
			}
			else
			{
				return lhs.second.second > rhs.second.second;
			}
		}
	};

public:
	RessourceManager() { pqueue = std::priority_queue<UnitPair, std::vector<UnitPair>, comparison>(comparison());
		reservedMinerals = 0;
		reservedGas = 0; }
	~RessourceManager() {}
	void addToQueue(BWAPI::UnitType unit, int priority = 0);
	void purchaseUnit();
	void confirmConstruction(BWAPI::UnitType ut)
	{
		reservedMinerals -= ut.mineralPrice();
		reservedGas -= ut.gasPrice();
	}

protected:
	std::priority_queue<UnitPair, std::vector<UnitPair>, comparison> pqueue;
	int reservedMinerals;
	int reservedGas;
};

#endif