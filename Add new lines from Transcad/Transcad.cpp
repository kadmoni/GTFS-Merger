#include "Transcad.h"
#include "just_G.h"
#include <string>
#include <iostream>
#define UNIQUE_LOC_STRING "-1"
#define UNIQUE_LOC -1

std::string removeChar(std::string str, char charToRemove) {
	str.erase(std::remove(str.begin(), str.end(), charToRemove), str.end());
	return str;
}

long long int removeUnder(string str)
{
	return stoll(removeChar(str, '_'));
}

long long int maxLong(long long int num, long long int unique)
{
	if (num > unique)
		unique=num;
	return ++unique;
}

void Transcad::addInfoRoutes(gtfs::Routes& routes)
{
	for (gtfs::Route& route : routes)
	{
		route.agency_id = "18";
		route.route_long_name = route.route_short_name;
		route.route_desc = route.route_short_name+" desc";
		if (route.route_short_name.length() > 8)
			route.route_short_name = route.route_short_name.substr(0,7)+"S";
		else
			route.route_short_name = route.route_short_name + "S";
		route.route_type = gtfs::RouteType::Bus;
	}
}

void Transcad::addInfoTrips(gtfs::Trips& trips)
{
	for (gtfs::Trip& trip : trips)
	{
		trip.trip_headsign = "Auto_Placeholder";
		trip.wheelchair_accessible = gtfs::TripAccess::Yes;
		trip.service_id = "76";
	}
}
void Transcad::addInfoStops(gtfs::Stops& stops)
{
	for (gtfs::Stop& stop : stops)
	{
		stop.stop_code = stop.stop_id;
		stop.stop_desc = stop.stop_name+" desc";
		stop.zone_id = stop.stop_id;
	}
}

void Transcad::checkRoutesDup(gtfs::Routes& routes, vector <long long int>& Ids, unordered_map<long long int,long long int>& changedIds, unordered_map<long long int, int>&routeFreq, unordered_map<long long int, int>&routeSpeeds)
{
	unordered_map <long long int, int> freqMap;
	long long int tempUnique = 1;
	for (long long int num : Ids)
	{
		tempUnique = maxLong(num, tempUnique);
		freqMap[num]++;
	}
	for (gtfs::Route& route : routes)
	{
		tempUnique = maxLong(stoll(route.route_id), tempUnique);
		freqMap[stoll(route.route_id)]++;
	}
	for (gtfs::Route& route : routes)
	{
		if (freqMap[stoll(route.route_id)] > 1)
		{
			changedIds[stoll(route.route_id)]= tempUnique;
			routeFreq[tempUnique] = routeFreq[stoll(route.route_id)];
			routeSpeeds[tempUnique] = routeSpeeds[stoll(route.route_id)];
			routeFreq.erase(stoll(route.route_id));
			routeSpeeds.erase(stoll(route.route_id));
			route.route_id = to_string(tempUnique);
			cout << "Route:" << route.route_short_name << " Id is a duplicate. Id changed to:" << route.route_id << endl;
			tempUnique = maxLong(tempUnique, tempUnique);
		}
	}
}

void Transcad::checkStopsDup(gtfs::Stops& stops, vector <long long int>& Ids, unordered_map<long long int,long long int>& changedIds,unordered_map<long long int, string>stopIdToHeadsign)
{
	unordered_map <long long int, int> freqMap;
	long long int tempUnique = 1;
	for (long long int num : Ids)
	{
		tempUnique = maxLong(num, tempUnique);
		freqMap[num]++;
	}
	for (gtfs::Stop& stop : stops)
	{
		tempUnique = maxLong(stoll(stop.stop_id), tempUnique);
		freqMap[stoll(stop.stop_id)]++;
	}
	for (gtfs::Stop& stop : stops)
	{
		if (freqMap[stoll(stop.stop_id)] > 1)
		{
			changedIds[stoll(stop.stop_id)] = tempUnique;
			stop.stop_id = to_string(tempUnique);
			cout << "Stop: " << stop.stop_name << " Id is a duplicate. Id changed to:" << stop.stop_id << endl;
			tempUnique = maxLong(tempUnique, tempUnique);
		}
		stopIdToHeadsign[stoll(stop.stop_id)] = stop.stop_name;
	}
}

void Transcad::checkTripsDup(gtfs::Trips& trips, vector<long long int>& shapeIds, vector<std::string>& tripIds, unordered_map<long long int,long long int>& changedShapeIds, unordered_map<std::string,long long int>& changedTripIds)
{
	unordered_map<long long int, int> shapeFreqMap;
	unordered_map<std::string, int> tripFreqMap;
	unordered_map<long long int, long long int>routeShapeId;
	long long int tempShapeUnique = 1, tempTripUnique = 1;
	for (std::string tripId : tripIds)
	{
		tempTripUnique = maxLong(removeUnder(tripId), tempTripUnique);
		tripFreqMap[tripId]++;
	}
	for (long long int shapeId : shapeIds)
	{
		tempShapeUnique = maxLong(shapeId, tempShapeUnique);
		shapeFreqMap[shapeId]++;
	}
	for (gtfs::Trip& trip : trips)
	{
		tempTripUnique = maxLong(removeUnder(trip.trip_id), tempTripUnique);
		tripFreqMap[trip.trip_id]++;
		routeShapeId[stoll(trip.route_id)] = stoll(trip.shape_id);
	}
	for (gtfs::Trip& trip : trips)
	{
		if (shapeFreqMap[stoll(trip.shape_id)] > 0)
		{
			changedShapeIds[stoll(trip.shape_id)]= tempShapeUnique;
			routeShapeId[stoll(trip.route_id)] = tempShapeUnique;
			cout << "Trips towards:" << trip.trip_headsign << " corresponding to route Id" << trip.route_id << "Have a duplicate Shape Id.Id changed to : " << tempShapeUnique << endl;
			shapeFreqMap[stoll(trip.shape_id)] = 0;
			tempShapeUnique = maxLong(tempShapeUnique, tempShapeUnique);
		}
	}
	for (gtfs::Trip& trip : trips)
	{
		if (routeShapeId.count(stoll(trip.route_id)) != 0)
		{
			trip.shape_id = to_string(routeShapeId[stoll(trip.route_id)]);
		}
		if (tripFreqMap[trip.trip_id] > 1)
		{
			changedTripIds[trip.trip_id] =tempTripUnique;
			trip.trip_id = to_string(tempTripUnique);
			cout << "Trip towards:" << trip.trip_headsign << " corresponding to route Id" << trip.route_id << "Has a duplicate trip Id.Id changed to : " << trip.trip_id << endl;
			tempTripUnique = maxLong(tempTripUnique, tempTripUnique);
		}
	}
	changedTripIds[UNIQUE_LOC_STRING] = tempTripUnique;
	changedShapeIds[UNIQUE_LOC] = tempShapeUnique;
}


void Transcad::matchRouteTripIds(gtfs::Trips& trips, unordered_map<long long int,long long int>& changedRouteIds)
{
	for (gtfs::Trip& trip : trips)
	{
		try
		{
			trip.route_id = to_string(changedRouteIds.at(stoll(trip.route_id)));
		}
		catch (const std::out_of_range& oor)
		{
			continue;
		}
	}
}

gtfs::Trip Transcad::newTrip(long long int& uniqueNum,gtfs::Trip& tripOrigin)
{
	gtfs::Trip trip(tripOrigin);
	trip.trip_id = to_string(uniqueNum);
	uniqueNum++;
	return trip;
}


void Transcad::addTrips(gtfs::Trips& trips, gtfs::Time& startT, gtfs::Time& endT, unordered_map<long long int, gtfs::Time>& durations, unordered_map<long long int, int>& routeFreq, int freq, long long int& uniqueNum, bool startEndT)
{
	if (startEndT == true)
	{
		if (freq != -1)
		{
			gtfs::Time duration;
			gtfs::Trips tempTrips;
			duration = endT - startT;
			tuple time = duration.get_hh_mm_ss();
			int tripAmount = ((int)(get<0>(time)) + (((double)(get<1>(time))) / 60)) * freq;
			for (gtfs::Trip& trip : trips)
			{
				for (int i = 0; i < tripAmount-1/*accounting for existing trips*/; i++)
				{
					tempTrips.push_back(newTrip(uniqueNum, trip));
				}
			}
			trips.insert(trips.end(), tempTrips.begin(), tempTrips.end());
		}
		else
		{
			gtfs::Time duration;
			gtfs::Trips tempTrips;
			duration = endT - startT;
			tuple time = duration.get_hh_mm_ss();
			for (gtfs::Trip& trip : trips)
			{
				int tripAmount = ((double)(get<0>(time)) + (((double)(get<1>(time))) / 60)) * routeFreq[stoll(trip.route_id)];
				for (int i = 0; i < tripAmount-1/*accounting for existing trips*/; i++)
				{
					tempTrips.push_back(newTrip(uniqueNum, trip));
				}
			}
			trips.insert(trips.end(), tempTrips.begin(), tempTrips.end());

		}
	}
	else
	{
		for (gtfs::Trip& trip : trips)
		{
			tuple time =durations[stoll(trip.route_id)].get_hh_mm_ss();
			int tripAmount = ((int)(get<0>(time)) + (((double)(get<1>(time))) / 60)) * freq;
			for (int i = 0;i < tripAmount-1/*acounting for existing trips*/; i++)
			{
				trips.push_back(newTrip(uniqueNum, trip));
			}
		}

	}

}
void Transcad::updateStopTimeIds(gtfs::StopTimes& stoptimes, unordered_map<std::string, long long int>changedTripIds, unordered_map<long long int, long long int>changedStopIds)
{
	for (gtfs::StopTime& stoptime : stoptimes)
	{
		string tempTrip = stoptime.trip_id;
		try
		{
			stoptime.trip_id = changedTripIds.at(stoptime.trip_id);
		}
		catch (const std::out_of_range& oor)
		{
			stoptime.trip_id = tempTrip;
		}
		string tempStop = stoptime.stop_id;
		try
		{
			stoptime.stop_id = to_string(changedStopIds.at(stoll(stoptime.stop_id)));
		}
		catch (const std::out_of_range& oor)
		{
			stoptime.stop_id = tempStop;
		}
	}

}


void Transcad::addStopTimesDepartures(gtfs::Trips& trips, unordered_map <long long int, string> stopIdToHeadsign, gtfs::StopTimes& stoptimes, unordered_map<long long int, int>&routeFreq, int& freq, gtfs::Time& startT, gtfs::Time& endT, unordered_map<long long int, pair<gtfs::Time, gtfs::Time>>& routeStartEndT, bool startEndT, int& speed, unordered_map<long long int, int>&routeSpeeds)
{
	if (startEndT == true)//same start and end times for all lines
	{
		unordered_map<std::string, long long int>tripToRouteIds;
		unordered_map<std::string, gtfs::Time>tripToStartTime;
		unordered_map<long long int, vector<std::string>> routeToTripsIds;
		gtfs::StopTimes tempStopTimes;
		for (gtfs::Trip& trip : trips)
		{
			routeToTripsIds[stoll(trip.route_id)].push_back(trip.trip_id);
			tripToRouteIds[trip.trip_id] = stoll(trip.route_id);
		}
		for (pair <const long long, vector<std::string>>& i : routeToTripsIds)
		{
			if (freq != -1)
			{
				gtfs::Time addTime(0, 60 / freq, 0);
				gtfs::Time tempStartT(startT);
				for (auto Id : i.second)
				{
					tripToStartTime[Id] = tempStartT;
					tempStartT = tempStartT + addTime;
				}
			}
			else
			{
				gtfs::Time addTime(0, 60 / routeFreq[i.first], 0);
				gtfs::Time tempStartT(startT);
				for (auto Id : i.second)
				{
					tripToStartTime[Id] = tempStartT;
					tempStartT = tempStartT + addTime;
				}
			}
		}
		gtfs::Time duration;
		duration = endT - startT;
		tuple time = duration.get_hh_mm_ss();
		for (gtfs::StopTime& stoptime : stoptimes)
		{
			stoptime.route_id = to_string(tripToRouteIds[stoptime.trip_id]);
			stoptime.pickup_type = gtfs::StopTimeBoarding::RegularlyScheduled;
			stoptime.drop_off_type = gtfs::StopTimeBoarding::RegularlyScheduled;
			try
			{
				stoptime.stop_headsign = stopIdToHeadsign.at(stoll(stoptime.stop_id));

			}
			catch (const std::out_of_range& oor)
			{
				stoptime.stop_headsign = "Auto_Placeholder";
			}
			if (stoptime.stop_sequence == 1)
			{
				tuple startTime = startT.get_hh_mm_ss();
				stoptime.arrival_time.setTime(int(get<0>(startTime)), int(get<1>(startTime)), int(get<2>(startTime)));
				stoptime.departure_time = stoptime.arrival_time;
				stoptime.shape_dist_traveled = stoptime.shape_dist_traveled * 1000;

			}
			else
			{
				double tempTravelTime;
				if (speed != -1)
					tempTravelTime = stoptime.shape_dist_traveled / speed;
				else
					tempTravelTime = stoptime.shape_dist_traveled / routeSpeeds[stoll(stoptime.route_id)];
				gtfs::Time travelTime(tempTravelTime * 60 * 60);
				gtfs::Time currentTime = startT + travelTime;
				tuple current = currentTime.get_hh_mm_ss();
				stoptime.arrival_time.setTime(int(get<0>(current)), int(get<1>(current)), int(get<2>(current)));
				stoptime.departure_time = stoptime.arrival_time;
				stoptime.shape_dist_traveled = stoptime.shape_dist_traveled*1000;
			}
		}

		if (freq != -1) //same freq for all lines
		{
			int perc = 0, count = 0;
			int tripAmount = ((int)(get<0>(time)) + (((double)(get<1>(time))) / 60)) * freq;
			for (gtfs::StopTime& stoptime : stoptimes)
			{
				for (int i = 0; i < tripAmount; i++)
				{
					count++;
					if (count / (83275 * tripAmount) > perc)
					{
						count = 0;
						cout << "Loading: " << i++ << "%" << "\r";
						perc++;
						if (perc>=99)
							std::cout << "\r" << "Loading: Done" << std::endl;
					}
					gtfs::StopTime tempStop;
					tempStop.trip_id = routeToTripsIds[stoll(stoptime.route_id)][i];
					if (tempStop.trip_id == stoptime.trip_id)
						continue;
					tuple startTime = tripToStartTime[tempStop.trip_id].get_hh_mm_ss();
					if (stoptime.stop_sequence == 1)
					{
						tempStop.arrival_time.setTime(int(get<0>(startTime)), int(get<1>(startTime)), int(get<2>(startTime)));
						tempStop.departure_time = tempStop.arrival_time;

					}
					else
					{
						double tempTravelTime;
						if (speed != -1)
							tempTravelTime = ((((double)stoptime.shape_dist_traveled)) / 1000) / ((double)speed);
						else
							tempTravelTime = ((((double)stoptime.shape_dist_traveled)) / 1000) / ((double)routeSpeeds[stoll(stoptime.route_id)]);
						gtfs::Time travelTime(tempTravelTime * 60 * 60);
						gtfs::Time currentTime = tripToStartTime[tempStop.trip_id] + travelTime;
						tuple current = currentTime.get_hh_mm_ss();
						tempStop.arrival_time.setTime(int(get<0>(current)), int(get<1>(current)), int(get<2>(current)));
						tempStop.departure_time = tempStop.arrival_time;
					}
					tempStop.pickup_type = gtfs::StopTimeBoarding::RegularlyScheduled;
					tempStop.drop_off_type = gtfs::StopTimeBoarding::RegularlyScheduled;
					try
					{
						tempStop.stop_headsign = stopIdToHeadsign.at(stoll(stoptime.stop_id));

					}
					catch (const std::out_of_range& oor)
					{
						tempStop.stop_headsign = "Auto_Placeholder";
					}

					tempStop.stop_id = stoptime.stop_id;
					tempStop.stop_sequence = stoptime.stop_sequence;
					tempStop.shape_dist_traveled = stoptime.shape_dist_traveled;
					tempStopTimes.push_back(tempStop);
				}

			}
		}
		else
		{
			for (gtfs::StopTime& stoptime : stoptimes)
			{
				int tripAmount = ((int)(get<0>(time)) + (((double)(get<1>(time))) / 60)) * routeFreq[stoll(stoptime.route_id)];
				for (int i = 0; i < tripAmount; i++)
				{

					gtfs::StopTime tempStop;
					tempStop.trip_id = routeToTripsIds[stoll(stoptime.route_id)][i];
					if (tempStop.trip_id == stoptime.trip_id)
						continue;
					tuple startTime = tripToStartTime[tempStop.trip_id].get_hh_mm_ss();
					if (stoptime.stop_sequence == 1)
					{
						tempStop.arrival_time.setTime(int(get<0>(startTime)), int(get<1>(startTime)), int(get<2>(startTime)));
						tempStop.departure_time = tempStop.arrival_time;

					}
					else
					{
						double tempTravelTime;
						if (speed != -1)
							tempTravelTime = ((((double)stoptime.shape_dist_traveled)) / 1000) / ((double)speed);
						else
							tempTravelTime = ((((double)stoptime.shape_dist_traveled)) / 1000) / ((double)routeSpeeds[stoll(stoptime.route_id)]);
						gtfs::Time travelTime(tempTravelTime * 60 * 60);
						gtfs::Time currentTime = tripToStartTime[tempStop.trip_id] + travelTime;
						tuple current = currentTime.get_hh_mm_ss();
						tempStop.arrival_time.setTime(int(get<0>(current)), int(get<1>(current)), int(get<2>(current)));;
						tempStop.departure_time = tempStop.arrival_time;
					}
					tempStop.pickup_type = gtfs::StopTimeBoarding::RegularlyScheduled;
					tempStop.drop_off_type = gtfs::StopTimeBoarding::RegularlyScheduled;
					try
					{
						tempStop.stop_headsign = stopIdToHeadsign.at(stoll(stoptime.stop_id));

					}
					catch (const std::out_of_range& oor)
					{
						tempStop.stop_headsign = "Auto_Placeholder";
					}

					tempStop.stop_id = stoptime.stop_id;
					tempStop.stop_sequence = stoptime.stop_sequence;
					tempStop.shape_dist_traveled = stoptime.shape_dist_traveled;
					tempStopTimes.push_back(tempStop);
				}

			}

		}
		stoptimes.insert(stoptimes.end(), tempStopTimes.begin(), tempStopTimes.end());
	}
	Transcad::sortStopTimes(stoptimes);
}

void Transcad::sortStopTimes(gtfs::StopTimes& stoptimes)
{
	sort(stoptimes.begin(), stoptimes.end(), stopTimesIdSort);
	for (int i = 0; i < stoptimes.size();)
	{
		int j = 0;
		while ((i + j != stoptimes.size()) && (removeUnder(stoptimes[i].trip_id) == removeUnder(stoptimes[i + j].trip_id)))//find a way for i+j to be ok even though its outside vector scope
			j++;
		sort(stoptimes.begin() + i, stoptimes.begin() + i + j, stopTimesSeqSort);
		i=i + j;

	}
}

bool Transcad::stopTimesIdSort(gtfs::StopTime& stop1, gtfs::StopTime& stop2)
{
	return removeUnder(stop1.trip_id) < removeUnder(stop2.trip_id);
}
bool Transcad::stopTimesSeqSort(gtfs::StopTime& stop1, gtfs::StopTime& stop2)
{
	return stop1.stop_sequence < stop2.stop_sequence;
}

void Transcad::matchShapeId(gtfs::Shapes& shapes, unordered_map<long long int, long long int>& changedShapeIds)
{
	for (gtfs::ShapePoint& shape : shapes)
	{
		std::string tempId = shape.shape_id;
		try {
			shape.shape_id = to_string(changedShapeIds.at(stoll(shape.shape_id)));
		}
		catch (const std::out_of_range& oor)
		{
			shape.shape_id = tempId;
		}
	}
}

string Transcad::getStartDate(gtfs::Calendar& calendar)
{

	string year = calendar[0].start_date.getYYYY();
	string month = calendar[0].start_date.getMM();
	string day = calendar[0].start_date.getDD();
	string fullDate = year + month+day;
	return fullDate;
}

void Transcad::shapeDistHandler(gtfs::StopTimes& stoptimes)//for accessibility software - shape_dist_traveled cant be 0 followed by 0
{
	int size = stoptimes.size();
	for (int i = 0; i < size; i++)
	{
		if (i < size - 1)
		{
			if ((stoptimes[i].shape_dist_traveled == 0.0) && (stoptimes[i + 1].shape_dist_traveled == stoptimes[i].shape_dist_traveled))
			{
				stoptimes[i].shape_dist_traveled = -1;
			}
		}
		else
		{
			if (stoptimes[i].shape_dist_traveled == 0.0)
				stoptimes[i].shape_dist_traveled = -1;
		}
	}
}

void Transcad::shapeSeqHandler(gtfs::Shape& shapes)
{
	for (gtfs::ShapePoint& shape : shapes)
	{
		shape.shape_pt_sequence = shape.shape_pt_sequence + 1;
	}
}



