#include <iostream>
#include <string>

class Sensor {
public:
    std::string id;
    double warn_threshold;
    double crit_threshold;
    int probe_interval;
    int notify_interval;

    Sensor(){};
    Sensor(std::string id, double warn_threshold,
            double crit_threshold, int probe_interval,
            int notify_interval);
    
    void printDebug();    
};
