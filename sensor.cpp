#include "sensor.h"

Sensor::Sensor(std::string id, double warn_threshold, double crit_threshold, int probe_interval, int notify_interval) {
    this->id = id;
    this->warn_threshold = warn_threshold;
    this->crit_threshold = crit_threshold;
    this->probe_interval = probe_interval;
    this->notify_interval = notify_interval;
}

void Sensor::printDebug() {
    std::cout << id << " " << warn_threshold << " " << crit_threshold << " " << probe_interval << " " << notify_interval << std::endl;
}