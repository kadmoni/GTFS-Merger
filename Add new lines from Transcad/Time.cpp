#include "Time.h"

class Time
{
public:
    int hours;
    int minutes;
    int seconds;

    Time(int hours, int minutes, int seconds) :hours(hours), minutes(minutes), seconds(seconds)
    {
    };

    // Function to convert time into total seconds since midnight
    int to_seconds(const Time& t) {
        return t.hours * 3600 + t.minutes * 60 + t.seconds;
    }

    // Function to convert seconds since midnight back into a Time struct
    Time from_seconds(int total_seconds) {
        total_seconds = (total_seconds + 86400) % 86400;  // Ensures 24-hour format (mod by seconds in 24 hours)
        Time t;
        t.hours = total_seconds / 3600;
        t.minutes = (total_seconds % 3600) / 60;
        t.seconds = total_seconds % 60;
        return t;
    }

    // Function to subtract two times
    Time subtract_times(const Time& t1, const Time& t2) {
        int t1_seconds = to_seconds(t1);
        int t2_seconds = to_seconds(t2);

        int result_seconds = t1_seconds - t2_seconds;
        if (result_seconds < 0) {
            result_seconds += 86400;  // Add 24 hours in seconds
        }

        return from_seconds(result_seconds);
    }
    Time correctForSpeed(const Time& t1, const double correction)
    {
        int t1_seconds = to_seconds(t1);
        double corrected = t1_seconds * correction;
        int correctedInt = (int)corrected;
        return from_seconds(correctedInt);
    }
    Time add_times(const Time& t1, const Time& t2)
    {
        int t1_seconds = to_seconds(t1);
        int t2_seconds = to_seconds(t2);
        int result_seconds = t1_seconds + t2_seconds;
        return from_seconds(result_seconds);
    }

};