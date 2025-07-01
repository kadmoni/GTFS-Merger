#pragma once
#include <vector>
#include "Trip.h"
#include "Route.h"
#include <unordered_map>
using namespace std;
class GTFS
{
public:
	unordered_map <int,Route> routes;
	vector <int> tripIds;
	vector <Trip> trips;
	vector <Stop> stops;


	void addExistingStop(int stopId, string stopName, long double stopLat, long double stopLon, int locationType = 0, int zoneId = 0);
	void addExistingRoute(int routeId, string routeName, int agencyId = 0, int routeType = 3);
	void addExistingTrip(int tripId, int routeId, int serviceId, int dir, int shapeId);


	// function check unique id
};

