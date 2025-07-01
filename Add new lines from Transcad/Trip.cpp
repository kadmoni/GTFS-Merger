#include "Trip.h"

Trip::Trip(int tripId, Route& route, int serviceId, int dir, int shapeId) :tripId(tripId), route(route),serviceId(serviceId), dir(dir), shapeId(shapeId),
startT(0, 0, 0), endT(0, 0, 0)
{
	this->route
}
