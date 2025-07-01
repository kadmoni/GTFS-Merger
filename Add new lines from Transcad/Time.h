#pragma once
class Time
{
public:
	int hours;
	int minutes;
	int seconds;

    Time(int hours, int minutes, int seconds);

    // Function to convert time into total seconds since midnight
    int to_seconds(const Time& t);

    // Function to convert seconds since midnight back into a Time struct
    Time from_seconds(int total_seconds);

    // Function to subtract two times
    Time subtract_times(const Time& t1, const Time& t2);

    Time correctForSpeed(const Time& t1, const double correction);

    Time add_times(const Time& t1, const Time& t2);
};


