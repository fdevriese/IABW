#ifndef _ARMY_
#define _ARMY_
#include <BWAPI.h>
#include <BWTA.h>

class Army
{
public:
	void add_unit(BWAPI::Unit* unit);
private:
	std::set<BWAPI::Unit*> army;
};

#endif