#include "monitoring.h"

Monitoring::Monitoring()
    : workers(100)
    , time(0)
{};

void Monitoring::notifyAlarm(std::string sensor_id, AlarmLevel level) {
    std::string req = "/alarms/notify?sensor_id=" + sensor_id + "&level=";

    if (level == AlarmLevel::NORM) {
        req += "NORM";
    } else if (level == AlarmLevel::WARN) {
        req += "WARN";
    } else if (level == AlarmLevel::CRIT) {
        req += "CRIT";
    }

    std::cout << "Trying to notify alarm for sensor_id=" << sensor_id << std::endl; 

    auto successCb = [sensor_id](const std::string& response) {
        std::cout << "Succesfull sended alarm notify for sensor_id=" << sensor_id << std::endl;
    };

    auto failCb = [sensor_id](const std::string& err_msg) {
        std::cout << "Failed to send alarm notify for sensor_id=" << sensor_id << std::endl;
    };

    AsyncRequest(req, std::move(successCb), std::move(failCb), false);
}

void Monitoring::run() {

    while (true) {
        for (auto& sensor : sensors) {
            if (sensor.needCheckProbe(time)) {

                const std::string req = "/sensors/probe?sensor_id=" + sensor.id;

                std::cout << "Need to check probe for sensor_id=" << sensor.id << std::endl;

                auto successCb = [self=this, time=time, &sensor](const std::string& response) {
                    auto successCbOnWorkers = [self, time=time, &sensor, response=response]() {

                        std::cout << "Success checked probe for sensor_id=" << sensor.id << std::endl;

                        double probe = std::stod(response);
                        if (sensor.needSwitchAlarmLevel(probe)) {
                            self->notifyAlarm(sensor.id, sensor.level);
                            sensor.last_notify = time;
                        }

                        if (sensor.needNotifyAlarm(time)) {
                            self->notifyAlarm(sensor.id, sensor.level);
                            sensor.last_notify = time;
                        }
                    };

                    self->workers.submit_task(std::move(successCbOnWorkers));
                };

                auto failCb = [&sensor](const std::string& err_msg) {
                    std::cout << "Failed to check probe for sensor_id=" << sensor.id << ", error=" << err_msg << std::endl;
                };

                AsyncRequest(req, std::move(successCb), std::move(failCb));
            }
        }
        
        ++time;
        sleep(1);
    }
}

void Monitoring::getConfig() { 
    std::cout << "Getting config..." << std::endl;

    std::string req = "/sensors/list";

    std::promise<std::string> raw_config_promise;
    std::future<std::string> raw_config = raw_config_promise.get_future();

    auto successCb = [prom=std::move(raw_config_promise)](const std::string& response) mutable {
        std::cout << "Success fetched config" << std::endl;
        prom.set_value(response);
    };

    auto failCb = [](const std::string& err_msg) {
        std::cout << "Failed to fetch config from server, breaking down" << std::endl;
        exit(1);
    };

    AsyncRequest(req, std::ref(successCb), std::ref(failCb));
    

    const std::vector<std::string> col_names = {"id", "warn_threshold", "crit_threshold", "probe_interval", "notify_interval"};

    csv::CSVFormat format;
    format.delimiter('\t')
        .no_header()
        .column_names(col_names);

    auto rows = csv::parse(raw_config.get(), format);

    
    for (auto& row : rows) {
        auto sensor = Sensor(
            row["id"].get<>(),
            row["warn_threshold"].get<double>(),
            row["crit_threshold"].get<double>(),
            row["probe_interval"].get<int>(),
            row["notify_interval"].get<int>()
        );
        sensors.push_back(std::move(sensor));
    }
}

void Monitoring::AsyncRequest(const std::string& path, std::function<void(const std::string&)> successCb, std::function<void(const std::string&)> failCb, bool isGet) {
    std::unique_lock<std::recursive_mutex> guard(request_mutex);

    auto http_cb = [successCb = std::move(successCb), failCb = std::move(failCb), path=path](cpr::Response r) {
        if (r.status_code != 200) {
            failCb("Status code: " + std::to_string(r.status_code) + ", error: " + r.error.message);
        } else {
            successCb(r.text);
        }
        return r.text;
    };
    
    cpr::Url url{"http://158.160.44.144:3000" + path};

    if (isGet) {
        cpr::GetCallback(http_cb, url, cpr::Timeout{500});
    } else {
        cpr::AsyncResponse r = cpr::PostAsync(url, cpr::Payload{}, cpr::Timeout{500});
    }
}