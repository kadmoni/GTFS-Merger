#pragma once
#include "just_G.h"
using namespace std;

class Transcad
{
public :
	static void addInfoStops(gtfs::Stops& stops);
	static void addInfoRoutes(gtfs::Routes& routes);
	static void addInfoTrips(gtfs::Trips& trips);
	static void checkRoutesDup(gtfs::Routes& routes, vector <long long int>& Ids, unordered_map<long long int, long long int>& changedIds, unordered_map<long long int, int>& routeFreq, unordered_map<long long int, int>& routeSpeeds);
	static void checkStopsDup(gtfs::Stops& stops, vector <long long int>& Ids, unordered_map<long long int, long long int>& changedIds,unordered_map<long long int, string> stopIdToHeadsign);
	static void checkTripsDup(gtfs::Trips& trips, vector<long long int>& shapeIds, vector<std::string>& tripIds, unordered_map<long long int, long long int>& changedShapeIds, unordered_map<std::string, long long int>& changedTripIds);
	static void matchRouteTripIds(gtfs::Trips& trips, unordered_map<long long int,long long int>& changedRouteIds);
	static void addTrips(gtfs::Trips& trips, gtfs::Time& startT, gtfs::Time& endT, unordered_map<long long int, gtfs::Time>& durations, unordered_map<long long int, int>& routeFreq,int freq,long long int& unique,bool startEndT);
	static void updateStopTimeIds(gtfs::StopTimes& stoptimes, unordered_map<std::string, long long int>changedTripIds,unordered_map<long long int, long long int>changedStopIds);
	static gtfs::Trip newTrip(long long int& uniqueNum, gtfs::Trip& tripOrigin);
	static void addStopTimesDepartures(gtfs::Trips& trips, unordered_map <long long int, string> stopIdToHeadsign, gtfs::StopTimes& stoptimes, unordered_map<long long int, int>&routeFreq, int& freq, gtfs::Time& startT, gtfs::Time& endT, unordered_map<long long int, pair<gtfs::Time, gtfs::Time>>& routeStartEndT, bool startEndT, int& speed, unordered_map<long long int, int>& routeSpeeds);
	static void sortStopTimes(gtfs::StopTimes& stoptimes);
	static bool stopTimesIdSort(gtfs::StopTime& stop1, gtfs::StopTime& stop2);
	static bool stopTimesSeqSort(gtfs::StopTime& stop1, gtfs::StopTime& stop2);
	static void matchShapeId(gtfs::Shapes& shapes, unordered_map<long long int, long long int>& changedShapeIds);
	static string getStartDate(gtfs::Calendar& calendar);




};

