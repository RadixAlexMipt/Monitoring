#include "sensor.h"

Sensor::Sensor(std::string id, double warn_threshold, double crit_threshold, int probe_interval, int notify_interval) {
    this->id = id;
    this->warn_threshold = warn_threshold;
    this->crit_threshold = crit_threshold;
    this->probe_interval = probe_interval;
    this->notify_interval = notify_interval;
    this->level = AlarmLevel::NORM;
    this->last_prob = -1e8;
    this->last_notify = -1e8;
}

bool Sensor::needCheckProbe(int time) const {
    if (time - probe_interval - last_prob >= 0) {
        return true;
    }

    return false;
}

bool Sensor::needNotifyAlarm(int time) const {
    if (level == AlarmLevel::NORM) {
        return false;
    }

    if (time - notify_interval - last_notify >= 0) {
        return true;
    }

    return false;
}

bool Sensor::needSwitchAlarmLevel(double probe) {
    AlarmLevel newLevel = AlarmLevel::NORM;

    if (probe >= crit_threshold) {
        newLevel = AlarmLevel::CRIT;
    } else if (probe >= warn_threshold) {
        newLevel = AlarmLevel::WARN;
    }

    if (newLevel != level) {
        level = newLevel;
        return true;
    }

    return false;
}

void Sensor::printDebug() const {

    std::cout << id << " " << warn_threshold << " " << crit_threshold << " " << probe_interval << " " << notify_interval << std::endl;

}