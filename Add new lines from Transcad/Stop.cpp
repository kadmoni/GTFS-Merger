#include "Stop.h"

Stop::Stop(int stopId, string stopName, long double stopLat, long double stopLon, int locationType = 0, int zoneId = 0) :stopId(stopId),
stopName(stopName), stopLat(stopLat), stopLon(stopLon), locationType(locationType), zoneId(zoneId)
{
};