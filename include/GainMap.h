#ifndef GAINMAP_H
#define GAINMAP_H
#include <unordered_map>
class GainMap {
public:
	using Container = std::unordered_map<int, double>;
	using Iterator = std::unordered_map<int, double>::iterator;

	GainMap();
	GainMap(std::string& filename);
	GainMap(const char* filename);
	~GainMap();
	inline void SetMapFile(std::string& filename) { FillMap(filename); };
	inline Iterator FindParameters(int gchan) { return paramMap.find(gchan); };
	inline Iterator End() { return paramMap.end(); };
	inline bool IsValid() { return is_valid; };


private:
	bool FillMap(std::string& filename);
	Container paramMap;
	bool is_valid;
};

#endif
