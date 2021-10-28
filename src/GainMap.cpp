#include "EventBuilder.h"
#include "GainMap.h"

GainMap::GainMap() :
	is_valid(false)
{
}

GainMap::GainMap(std::string& filename) :
	is_valid(false)
{
	FillMap(filename);
}

GainMap::GainMap(const char* filename) :
	is_valid(false)
{
	std::string name = filename;
	FillMap(name);
}

GainMap::~GainMap() {}

bool GainMap::FillMap(std::string& filename) {
	std::ifstream input(filename);
	if(!input.is_open()) {
		is_valid = false;
		return is_valid;
	}

	std::string junk;
	double value;
	int gchan;

	while(input>>junk) {
		input>>gchan>>value;
		paramMap[gchan] = value;
	}

	input.close();
	is_valid = true;
	return is_valid;
}
