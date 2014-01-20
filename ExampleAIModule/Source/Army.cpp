#include "Army.h"

using namespace BWAPI;

void Army::add_unit(BWAPI::Unit* unit) 
{
	army.insert(unit);
}