#include <vector>
#include <map>
#include "Stop.h"

class Route {
public:
	int routeId;
	string routeName;
	int agencyId;
	string routeDesc;
	int routeType;
	vector <Stop&> stops;



	Route(int routeId, string routeName, int agencyId = 0, int routeType = 3) :routeId(routeId), routeName(routeName), agencyId(agencyId), routeDesc(routeName),
		routeType(routeType)
	{
	};

};