#ifndef _BUILDINGS_
#define _BUILDINGS_
#include <BWAPI.h>
#include <BWTA.h>

int build(BWAPI::Unit* builder, BWAPI::UnitType building_type, BWAPI::TilePosition p);
int build_in_main_base(BWAPI::Unit* builder, BWAPI::UnitType building_type);


#endif