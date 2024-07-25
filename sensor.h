#include <iostream>
#include <string>
#include <mutex>
#include <atomic>

enum class AlarmLevel{
    NORM,
    WARN,
    CRIT
};

struct Sensor {
    std::string id;
    std::atomic<double> warn_threshold;
    std::atomic<double> crit_threshold;
    std::atomic<int> probe_interval;
    std::atomic<int> notify_interval;

    std::atomic<int> last_prob;
    std::atomic<int> last_notify;
    std::atomic<AlarmLevel> level;

    Sensor(std::string id, double warn_threshold, double crit_threshold, int probe_interval, int notify_interval);
    
    Sensor(Sensor&& rhs)
        : id(std::move(rhs.id))
        , warn_threshold(rhs.warn_threshold.load())
        , crit_threshold(rhs.crit_threshold.load())
        , probe_interval(rhs.probe_interval.load())
        , notify_interval(rhs.notify_interval.load())
        , last_prob(rhs.last_prob.load())
        , last_notify(rhs.last_notify.load())
        , level(rhs.level.load())
    {}

    bool needCheckProbe(int time) const;
    bool needNotifyAlarm(int time) const;
    bool needSwitchAlarmLevel(double probe);

    void printDebug() const;
};
