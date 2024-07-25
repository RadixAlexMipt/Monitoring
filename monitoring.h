#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <optional>
#include <memory>
#include <future>
#include <mutex>

#include "third_party/csv-parser/include/csv.hpp"
#include "third_party/thread-pool/include/BS_thread_pool.hpp"
#include "third_party/cpr/include/cpr/cpr.h"

#include "sensor.h"

class Monitoring {
public:
    Monitoring();
    Monitoring(const Monitoring&) = delete;
    Monitoring(Monitoring&&) = default;

    void run();
    void getConfig();

private:
    void notifyAlarm(std::string sensorId, AlarmLevel level);

    void AsyncRequest(const std::string& path, std::function<void(const std::string&)> successCb, std::function<void(const std::string&)> failCb, bool isGet = true);

private:
    static const size_t REQUEST_RETRIES_COUNT = 3;

    BS::thread_pool workers;
    std::recursive_mutex request_mutex;

    std::vector<Sensor> sensors;
    int time;
};