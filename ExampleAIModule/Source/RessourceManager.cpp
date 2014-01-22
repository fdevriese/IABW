#include "RessourceManager.h"
#include "buildings.h"

using namespace BWAPI;
using namespace std;

Unit* getWorker()
{
	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
	{
		if ((*i)->getType().isWorker() && ((*i)->isGatheringMinerals() || (*i)->isIdle()))
		{
			return *i;
		}
	}
	return NULL;
}

Unit* getUnitOfType(UnitType ut)
{
	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
	{
		if ((*i)->getType() == ut && (*i)->isCompleted())
		{
			return *i;
		}
	}
	return NULL;
}

void RessourceManager::addToQueue(BWAPI::UnitType unit, int priority)
{
	static int call_count = 0;
	UnitPair up(unit, pair<int, int>(priority, call_count++));
	pqueue.push(up);
}

void RessourceManager::purchaseUnit()
{
	if(pqueue.empty())
		return;
	int availableMinerals = Broodwar->self()->minerals() - reservedMinerals;
	int availableGas = Broodwar->self()->gas() - reservedGas;
	UnitType ut = pqueue.top().first;
	if(ut.gasPrice() <= availableGas && ut.mineralPrice() <= availableMinerals && 
		(ut.supplyRequired() <= (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed()) || ut.supplyRequired() == 0))
	{
		if(ut.isBuilding())
		{
			Unit* worker = getWorker();
			if(worker == NULL)
				return;
			if(ut == BWAPI::UnitTypes::Terran_Command_Center ? expand(worker) : build_in_main_base(worker, ut))
			{
				pqueue.pop();
				reservedMinerals += ut.mineralPrice();
				reservedGas += ut.gasPrice();
			}
		}
		else
		{
			Unit* building = getUnitOfType(ut.whatBuilds().first);
			if(building == NULL)
				return;
			building->train(ut);
			pqueue.pop();
		}
	}
}