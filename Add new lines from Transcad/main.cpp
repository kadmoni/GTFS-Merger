#include "just_G.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Transcad.h"
#include <cstdlib>
#include <direct.h>
#include <string>


#define UNIQUE_LOC_STRING "-1"
#define UNIQUE_LOC -1

using namespace std;

enum Print
{
	CurrentParse, NewParse, DupRouteIdCheck, MatchRouteTrip, DupTripIdCheck, DupStopIdCheck, RouteCombine, SpeedCheck, FileCombineSucc, WriteSucc, ParseTrips, ParseStops, ParseRoutes
	, ParseStopTimes, ParseNewTrips, ParseNewStops, ParseNewRoutes, ParseNewStopTimes,StopTimeMatch,StopTimeAdd,AddToFeed,WriteToFile,ParseShapes,ParseNewShapes,AddInfo,ParseAgencies,
	ParseCalendar,ParseFareAtt,ParseFareRules,ParseTranslations,WriteRoutes,WriteTrips,WriteAgency,WriteCalendar,WriteFareAtt,WriteFareRules,WriteShapes,WriteStopTimes,WriteStops,CopyTranslations,
	ZipExtract,ZipCreate,Copier
};

void userPrints(Print toPrint)
{
	switch (toPrint)
	{
	case Print::CurrentParse:
		cout << "Parsing Israel's GTFS package:" << endl;
		break;
	case Print::NewParse:
		cout << "Parsing Transcad GTFS package..." << endl;
		break;
	case Print::DupRouteIdCheck:
		cout << "Checking for duplicate route IDs ..." << endl;
		break;
	case Print::RouteCombine:
		cout << "Combining route GTFS file ..." << endl;
		break;
	case Print::MatchRouteTrip:
		cout << "Matching changed IDs to appropriate trips ..."<<endl;
		break;
	case Print::DupTripIdCheck:
		cout << "Checking for duplicate trip IDs ..." << endl;
		break;
	case Print::DupStopIdCheck:
		cout << "Checking for duplicate stop IDs ..." << endl;
		break;
	case Print::FileCombineSucc:
		cout << "Combined all GTFS files and added relevant info to Transcad's GTFS files." << endl;
		break;
	case Print::WriteSucc:
		cout << "Exporting new combined GTFS files into folder \"Combined\"." << endl;
		break;
	case Print::ParseTrips:
		cout << "Parsing Trips from GTFS Israel: " << endl;
		break;
	case Print::ParseRoutes:
		cout << "Parsing Routes from GTFS Israel: " << endl;
		break;
	case Print::ParseStops:
		cout << "Parsing Stops from GTFS Israel: " << endl;
		break;
	case Print::ParseStopTimes:
		cout << "Parsing Stop-Times from GTFS Israel: " << endl;
		break;
	case Print::ParseNewTrips:
		cout << "Parsing Trips from Transcad's GTFS... " << endl;
		break;
	case Print::ParseNewRoutes:
		cout << "Parsing Routes from Transcad's GTFS..." << endl;
		break;
	case Print::ParseNewStops:
		cout << "Parsing Stops from Transcad's GTFS..." << endl;
		break;
	case Print::ParseNewStopTimes:
		cout << "Parsing Stop-Times from Transcad's GTFS..." << endl;
		break;
	case Print::StopTimeMatch:
		cout << "Matching Stop-time trip Ids to Trips trip Ids..." << endl;
		break;
	case Print::StopTimeAdd:
		cout << "Adding Departures and records to Stop-times..." << endl;
		break;
	case Print::AddToFeed:
		cout << "Combining files..." << endl;
		break;
	case Print::ParseShapes:
		cout << "Parsing Shapes from GTFS Israel: " << endl;
		break;
	case Print::ParseNewShapes:
		cout << "Parsing Shapes from Transcad's GTFS..." << endl;
		break;
	case Print::AddInfo:
		cout << "Adding information required for the accessibility maps program..." << endl;
		break;
	case Print::ParseAgencies:
		cout << "Parsing Agencies from GTFS Israel: " << endl;
		break;
	case Print::ParseCalendar:
		cout << "Parsing Calendar from GTFS Israel: " << endl;
		break;
	case Print::ParseFareAtt:
		cout << "Parsing Fare attributes from GTFS Israel: " << endl;
		break;
	case Print::ParseFareRules:
		cout << "Parsing Fare rules from GTFS Israel:" << endl;
		break;
	case Print::WriteAgency:
		cout << "Writing agency.txt: " << endl;
		break;
	case Print::WriteCalendar:
		cout << "Writing calendar.txt: " << endl;
		break;
	case Print::WriteFareAtt:
		cout << "Writing fare_attributes.txt: " << endl;
		break;
	case Print::WriteFareRules:
		cout << "Writing fare_rules.txt: " << endl;
		break;
	case Print::WriteRoutes:
		cout << "Writing routes.txt: " << endl;
		break;
	case Print::WriteShapes:
		cout << "Writing shapes.txt: " << endl;
		break;
	case Print::WriteStopTimes:
		cout << "Writing stop_times.txt: " << endl;
		break;
	case Print::WriteStops:
		cout << "Writing stops.txt: " << endl;
		break;
	case Print::CopyTranslations:
		cout << "Copying translations.txt: " << endl;
		break;
	case Print::WriteTrips:
		cout << "Writing trips.txt: " << endl;
		break;
	case Print::ZipExtract:
		cout << "Extracting files from current israel GTFS zip: " << endl;
		break;
	case Print::ZipCreate:
		cout << "Creating zipped folder of new combined GTFS: " << endl;
		break;
	case Print::Copier:
		cout << "Copying unchanged GTFS files..." << endl;
		break;
		
		

	}

}

void addRoutesToFeed(gtfs::Routes& routes, gtfs::Feed& currentFeed)
{
	for (gtfs::Route& route : routes)
		currentFeed.add_route(route);
}
void addShapesToFeed(gtfs::Shapes& shapes, gtfs::Feed& currentFeed)
{
	for (gtfs::ShapePoint& shape : shapes)
		currentFeed.add_shape(shape);
}
void addStopsToFeed(gtfs::Stops& stops, gtfs::Feed& currentFeed)
{
	for (gtfs::Stop& stop : stops)
		currentFeed.add_stop(stop);
}
void addTripsToFeed(gtfs::Trips& trips, gtfs::Feed& currentFeed)
{
	for (gtfs::Trip& trip : trips)
		currentFeed.add_trip(trip);
}
void addStopTimesToFeed(gtfs::StopTimes& stoptimes, gtfs::Feed& currentFeed)
{
	for (gtfs::StopTime& stoptime : stoptimes)
		currentFeed.add_stop_time(stoptime);
}

void speedAndFreqInput(int& speed, int& freq, unordered_map<long long int, int>& routeSpeeds, unordered_map<long long int, int>& routeFreq,gtfs::Routes& tempRoutes,int& update)
{
	int decSpeed = 0, decFreq = 0, decStartT = 0, decEndT = 0, input = 0;
	cout << "Update the speeds of GTFS Israel? " << endl<<"1: Yes, 2: No" << endl << "Answer: ";
	cin >> update;
	if (update == 1)
	{
		std::cout << "Enter by what percentage of the current speed you want to increase the speed of all lines in GTFS Israel: " << endl << "Answer: ";
		cin >> update;
	}
	else
		update = -1;
	cout << "Give all new lines the same speed?" << endl << "1: Yes, 2: No"<<endl<<"Answer: ";
	cin >> decSpeed;
	while ((decSpeed != 1) && (decSpeed != 2))
	{
		cout << "Input error (choose 1 or 2)" << endl;
		cout << "Give all new lines the same speed?" << endl << "1: Yes, 2: No"<< endl<<"Answer: ";
		cin >> decSpeed;
	}
	if (decSpeed == 1)
	{
		cout << "Input speed for all new lines in kmh: "<<endl<<"Answer: ";
		cin >> speed;
	}
	else
	{
		cout << "Choice per route confirmed. You will be asked for speeds after choosing frequancy preferences" << endl;
	}
	cout << "Give all new lines the same frequancy?" << endl << "1: Yes, 2: No" << endl<<"Answer: ";
	cin >> decFreq;
	while ((decFreq != 1) && (decFreq != 2))
	{
		cout << "Input error (choose 1 or 2)" << endl;
		cout << "Give all new lines the same frequancy?" << endl << "1: Yes, 2: No" << endl << "Answer: ";
		cin >> decFreq;
	}
	if (decFreq == 1)
	{
		cout << "Input frequancy for all new lines (departures per hour):" << endl << "Answer: ";
		cin >> freq;
	}
	if ((decSpeed == 2) && (decFreq == 2))
	{
		cout << "Attention: displayed route IDs are not changed yet to be unique. They are displayed as they appear in the orginial GTFS files from Transcad !" << endl;
		for (gtfs::Route& route : tempRoutes)
		{
			cout << "Input the speed for route: " << route.route_id << " - " << route.route_short_name << ": " << endl << "Answer: ";
			cin >> input;
			routeSpeeds[stoll(route.route_id)] = input;
			cout << "Input the frequancy for route " << route.route_id << " - " << route.route_short_name << ": " << endl << "Answer: ";
			cin >> input;
			routeFreq[stoll(route.route_id)] = input;
		}
	}
	else if (decSpeed == 2)
	{
		cout << "Attention: displayed route IDs are not changed yet to be unique. They are displayed as they appear in the orginial GTFS files from Transcad !" << endl;
		for (gtfs::Route& route : tempRoutes)
		{
			cout << "Input the speed for route " << route.route_id << " - " << route.route_short_name << ": " << endl << "Answer: ";
			cin >> input;
			routeSpeeds[stoll(route.route_id)] = input;
		}
	}
	else if (decFreq == 2)
	{
		cout << "Attention: displayed route IDs are not changed yet to be unique. They are displayed as they appear in the orginial GTFS files from Transcad !" << endl;
		for (gtfs::Route& route : tempRoutes)
		{
			cout << "Input the frequancy for route " << route.route_id << " - " << route.route_short_name << ": " << endl << "Answer: ";
			cin >> input;
			routeFreq[stoll(route.route_id)] = input;
		}
	}
}
bool startEndTInput(gtfs::Routes& routes,unordered_map<long long int,gtfs::Time>& durations, unordered_map<long long int, pair<gtfs::Time, gtfs::Time>>& routeStartEndT,gtfs::Time& startT, gtfs::Time& endT)
{
	int decStartT = 0, decEndT = 0, startH = 0, startM = 0, endH = 0, endM = 0;
	cout << "Give all new lines the same start and end times?" << endl << "1:Yes 2:No" << endl << "Answer: ";
	cin >> decStartT;
	if (decStartT == 1)
	{
		cout << "Input the start time for all new lines:" << endl << "Hour: ";
		cin >> startH;
		cout << "Minutes: ";
		cin >> startM;
		startT.setTime(startH, startM, 0);
		cout << "Input the end time for all new lines:" << endl << "Hour: ";
		cin >> endH;
		cout << "Minutes: ";
		cin >> endM;
		endT.setTime(endH, endM, 0);
		return true;
	}
	else
	{
		for (gtfs::Route& route : routes)
		{
			int startH, startM, endH, endM;
			gtfs::Time startT, endT, duration;
			cout << "Input start time for route:" << route.route_id << " - " << route.route_short_name << " : " << endl<<"Hour: ";
			cin >> startH;
			cout << endl << "Minutes: ";
			cin >> startM;
			cout <<endl<< "Input end time for route:" << route.route_id << " - " << route.route_short_name << " : " << endl << "Hour: ";
			cin >> endH;
			cout << endl << "Minutes: ";
			cin >> endM;
			cout << endl;
			startT.setTime(startH, startM, 0);
			endT.setTime(endH, endM, 0);
			routeStartEndT[stoll(route.route_id)] = make_pair(startT, endT);
			duration = endT - startT;
			durations[stoll(route.route_id)] = duration;
		}
		return false;
	}
	
}
void addToFeedISR(gtfs::Routes& routes, gtfs::Stops& stops, gtfs::Trips& trips, gtfs::StopTimes& stoptimes, gtfs::Shapes& shapes, gtfs::Feed& currentFeed)
{
	addRoutesToFeed(routes, currentFeed);
	addStopsToFeed(stops, currentFeed);
	addTripsToFeed(trips, currentFeed);
	addStopTimesToFeed(stoptimes, currentFeed);
	addShapesToFeed(shapes, currentFeed);
}

void writeISR(gtfs::Feed& currentFeed, string path)
{
	userPrints(WriteRoutes);
	currentFeed.write_routes_ISR(path);
	userPrints(WriteTrips);
	currentFeed.write_trips_ISR(path);
	userPrints(WriteStops);
	currentFeed.write_stops_ISR(path);
	userPrints(WriteStopTimes);
	currentFeed.write_stop_times_ISR(path);
	userPrints(WriteShapes);
	currentFeed.write_shapes_ISR(path);
}



int main()
{

// one trip too many, change tomorrow
	unordered_map<long long int,long long int> changedRouteIds;
	unordered_map<long long int,long long int> changedShapeIds;
	unordered_map<string,long long int>changedTripIds;
	unordered_map<long long int,long long int>changedStopIds;
	unordered_map<long long int,int> routeSpeeds;
	unordered_map<long long int, int>routeFreq;
	unordered_map < long long int, gtfs::Time > durations;
	unordered_map<long long int, pair<gtfs::Time, gtfs::Time>> routeStartEndT;
	unordered_map<long long int, string>stopIdToHeadsign;
	vector <long long int> routeIds;
	vector <string> tripIds;
	vector <long long int> shapeIds;
	vector <long long int> stopIds;
	bool startEndT;
	int speed = -1, freq = -1,tripAmount =0,update = 0,startH,startM,endH,endM;
	gtfs::Time startT, endT;

	_chdir("C://Users//User//Desktop//DEV//GTFS-merger");

	userPrints(ZipExtract);
	system("Utillities\\zipExtractor.bat");
	userPrints(CurrentParse);
	gtfs::Feed currentFeed("./Current/");
	userPrints(ParseTrips);
	currentFeed.read_trips();
	userPrints(ParseRoutes);
	currentFeed.read_routes();
	userPrints(ParseStops);
	currentFeed.read_stops();
	for (const gtfs::Route& route : currentFeed.get_routes())
		routeIds.push_back(stoll(route.route_id));
	for (const gtfs::Trip& trip : currentFeed.get_trips())
	{
		tripIds.push_back(trip.trip_id);
		try
		{
			shapeIds.push_back(stoll(trip.shape_id));
		}
		catch (invalid_argument)
		{
			shapeIds.push_back(0);
		}
	}
	for (const gtfs::Stop& stop : currentFeed.get_stops())
		stopIds.push_back(stoll(stop.stop_id));
	userPrints(NewParse);
	gtfs::Feed newFeed("./New/");
	userPrints(ParseNewTrips);
	newFeed.read_trips();
	userPrints(ParseNewRoutes);
	newFeed.read_routes();
	userPrints(ParseNewShapes);
	newFeed.read_shapes();
	userPrints(ParseNewStops);
	newFeed.read_stops();
	gtfs::Routes tempRoutes(newFeed.get_routes());
	speedAndFreqInput(speed, freq, routeSpeeds, routeFreq, tempRoutes,update); 
	userPrints(DupRouteIdCheck);
	Transcad::checkRoutesDup(tempRoutes, routeIds,changedRouteIds,routeFreq,routeSpeeds);
	userPrints(MatchRouteTrip);
	gtfs::Trips tempTrips(newFeed.get_trips());
	Transcad::matchRouteTripIds(tempTrips, changedRouteIds);
	userPrints(DupTripIdCheck);
	Transcad::checkTripsDup(tempTrips,shapeIds,tripIds,changedShapeIds,changedTripIds);
	userPrints(DupStopIdCheck);
	gtfs::Stops tempStops(newFeed.get_stops());
	Transcad::checkStopsDup(tempStops, stopIds, changedStopIds,stopIdToHeadsign);
	startEndT = startEndTInput(tempRoutes,durations,routeStartEndT,startT, endT);	
	Transcad::addTrips(tempTrips, startT, endT,durations,routeFreq,freq, changedTripIds[UNIQUE_LOC_STRING],startEndT);
	userPrints(ParseShapes);
	currentFeed.read_shapes();
	gtfs::Shapes tempShapes(newFeed.get_shapes());
	Transcad::matchShapeId(tempShapes, changedShapeIds);
	userPrints(ParseStopTimes);
	currentFeed.read_stop_times();
	currentFeed.updateStoptimes(update);//updates stoptimes if needed
	userPrints(ParseNewStopTimes);
	newFeed.read_stop_times();
	gtfs::StopTimes tempStopTimes(newFeed.get_stop_times());
	userPrints(StopTimeMatch);
	Transcad::updateStopTimeIds(tempStopTimes, changedTripIds, changedStopIds);
	userPrints(StopTimeAdd);
	Transcad::addStopTimesDepartures(tempTrips, stopIdToHeadsign, tempStopTimes, routeFreq, freq, startT, endT, routeStartEndT, startEndT, speed,routeSpeeds);
	userPrints(AddInfo);
	Transcad::addInfoTrips(tempTrips);
	Transcad::addInfoRoutes(tempRoutes);
	Transcad::addInfoStops(tempStops);
	userPrints(AddToFeed);
	addToFeedISR(tempRoutes, tempStops, tempTrips, tempStopTimes , tempShapes, currentFeed);
	userPrints(WriteSucc);
	writeISR(currentFeed, "./Combined/");
	userPrints(Copier);
	system("Utillities\\copier.bat");
	userPrints(ZipCreate);
	system("Utillities\\zipCreator.bat");
	currentFeed.read_calendar();
	gtfs::Calendar tempCalendar(currentFeed.get_calendar());
	string israel = "israel";
	string correctName = "Combined_Zip\\" + israel + Transcad::getStartDate(tempCalendar) + ".zip";
	rename("Combined_Zip\\thisnew.zip", correctName.c_str());
	return 0;
}