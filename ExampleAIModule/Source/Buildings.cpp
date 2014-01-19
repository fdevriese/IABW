#include "buildings.h"

using namespace BWAPI;
extern Position position_home;


int build_in_main_base(BWAPI::Unit* builder, BWAPI::UnitType building_type)
{
	static TilePosition last_building_pos = TilePosition(position_home);
	int x_size = building_type.tileWidth();
	int y_size = building_type.tileHeight();
	TilePosition p = TilePosition(last_building_pos);
	int dx = rand()%2-1;
	int dy = rand()%2-1;
	bool valid = false;
	while(!valid)
	{
		for(int count = 0; count < 3; count++)
		{
			p = TilePosition(p.x() + 2*dx, p.y() + 2*dy);
			if(Broodwar->canBuildHere(builder, p, building_type))
			{
				valid = true;
				break;
			}
		}
		dx = rand()%2-1;
		dy = rand()%2-1;
	}
	last_building_pos = p;


	return build(builder, building_type, p);
}

int build(Unit* builder, UnitType building_type, TilePosition p)
{
	return !builder->build(p, building_type);
}