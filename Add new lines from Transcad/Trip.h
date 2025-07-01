#pragma once
#include "Time.h"
#include "Route.h"
#include <string>
using namespace std;
class Trip
{
public:
	int tripId;
	Time startT;
	Time endT;
	Route& route;
	int serviceId;
	string tripHead;
	int dir;
	int shapeId;

	Trip(int tripId,int routeId, int serviceId, int dir, int shapeId);


};

