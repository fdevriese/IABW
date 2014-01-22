#include "buildings.h"

using namespace BWAPI;
extern Position position_home;
extern std::set<TilePosition> spotted_minerals;


int build_in_main_base(BWAPI::Unit* builder, BWAPI::UnitType building_type)
{
	static TilePosition last_building_pos = TilePosition(position_home);
	int x_size = building_type.tileWidth();
	int y_size = building_type.tileHeight();
	TilePosition p;
	int dx = rand()%3-1;
	int dy = rand()%3-1;
	bool valid = false;
	while(!valid)
	{
		p = last_building_pos;
		for(int count = 0; count < 4; count++)
		{
			p = TilePosition(p.x() + 2*dx, p.y() + 2*dy);
			if(Broodwar->canBuildHere(builder, p, building_type))
			{
				valid = true;
				break;
			}
		}
		dx = rand()%3-1;
		dy = rand()%3-1;
	}
	last_building_pos = p;


	return build(builder, building_type, p);
}

int build(Unit* builder, UnitType building_type, TilePosition p)
{
	return builder->build(p, building_type);
}

int expand(BWAPI::Unit* builder)
{
	std::set<Unit*> ressource_depot;
	TilePosition* closestFreeMineral = NULL;
	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
	{
		if ((*i)->getType().isResourceDepot())
		{
			ressource_depot.insert(*i);
		}
	}
	for(std::set<TilePosition>::iterator m = spotted_minerals.begin();m!=spotted_minerals.end();m++)
	{
		bool candidate = true;
		Broodwar->pingMinimap((*m).x()*32, (*m).y()*32);
		for(std::set<Unit*>::const_iterator i=ressource_depot.begin();i!=ressource_depot.end();i++) 
		{
			if((*i)->getDistance(Position(*m)) < 320)	// 10 tiles away
			{
				candidate = false;
				break;
			}
		}
		if(candidate)
		{
			if(!closestFreeMineral || Position(*closestFreeMineral).getDistance(position_home) > Position(*m).getDistance(position_home))
			{
				closestFreeMineral = &(*m);
			}
		}
	}
	if(!closestFreeMineral)
		return -1;

	int barx = 0, bary = 0, count = 0;
	for(std::set<TilePosition>::iterator m=spotted_minerals.begin();m!=spotted_minerals.end();m++)
	{
		if(closestFreeMineral->getDistance(*m) < 10)
		{
			count++;
			barx += (*m).x();
			bary += (*m).y();
		}
	}
	barx /= count;
	bary /= count;

	TilePosition pos = TilePosition(barx,bary);
	TilePosition p;
	Broodwar->pingMinimap(p.x()*32, p.y()*32);
	for(int i = 3 ; ; i++)
	{
		p = TilePosition(pos.x() - i - 4, pos.y());	// left
		if(Broodwar->canBuildHere(builder, p, UnitTypes::Terran_Command_Center))
		{
			break;
		}
		p = TilePosition(pos.x(), pos.y() - i - 3); // top
		if(Broodwar->canBuildHere(builder, p, UnitTypes::Terran_Command_Center))
		{
			break;
		}
		p = TilePosition(pos.x() + i, pos.y());	// right
		if(Broodwar->canBuildHere(builder, p, UnitTypes::Terran_Command_Center))
		{
			break;
		}
		p = TilePosition(pos.x(), pos.y() + i); // down
		if(Broodwar->canBuildHere(builder, p, UnitTypes::Terran_Command_Center))
		{
			break;
		}
		if(i == 6)
			return -2;
	}

	return build(builder, UnitTypes::Terran_Command_Center, p);
}