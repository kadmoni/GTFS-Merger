#pragma once
#include <string>
using namespace std;

class Stop
{
public:
	int stopId;
	int stopCode;
	string stopName;
	string stopDesc;
	long double stopLat;
	long double stopLon;
	int locationType;
	int zoneId;

	Stop(int stopId, string stopName, long double stopLat, long double stopLon, int locationType = 0, int zoneId = 0);


};

