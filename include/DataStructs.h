/*DataStructs.h
 *Data structures for analysis. To be implemented as a dictionary for ROOT in LinkDef
 *Based on: FocalPlane_SABRE.h
 *Gordon M. Oct. 2019
 */
#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <vector>
#include <cstdint>

struct DPPChannel
{
	double Timestamp;
	int Channel, Board, Energy, EnergyShort;
	int64_t EnergyCal;
	int Flags;
	std::vector<uint16_t> Samples;
};

struct DetectorHit
{
	int globalChannel;
	double energy;
	double timestamp;
};

struct FQQQDetector
{
	std::vector<DetectorHit> rings;
	std::vector<DetectorHit> wedges;
};

struct BarrelDetector
{
	std::vector<DetectorHit> frontsUp;
	std::vector<DetectorHit> frontsDown;
	std::vector<DetectorHit> backs;
};

struct BarcDetector
{
	std::vector<DetectorHit> fronts;
	std::vector<DetectorHit> backs;
};

struct CoincEvent
{
	BarrelDetector barrel[12];
	FQQQDetector fqqq[4];
	BarcDetector barcUp[6];
	BarcDetector barcDown[6];
};

/*
  ROOT does a bad job of ensuring that header-only type dictionaries (the only type they explicity accept)
  are linked when compiled as shared libraries (the recommended method). As a work around, a dummy function that 
  ensures the library is linked (better than no-as-needed which I dont think is in general supported across platforms)
*/
bool EnforceDictonaryLinked();

#endif
