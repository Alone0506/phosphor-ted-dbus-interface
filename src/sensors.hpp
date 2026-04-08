#pragma once

#include "sensor.hpp"

#include <boost/asio/error.hpp>
#include <boost/asio/steady_timer.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>

#include <chrono>
#include <string>
#include <unordered_map>

PHOSPHOR_LOG2_USING_WITH_FLAGS;

class Sensors
{
  public:
    Sensors() = delete;
    virtual ~Sensors() = default;

    /**
     * @brief Constructs Sensors
     *
     * @param[in] conn - D-Bus object connection
     */
    explicit Sensors(std::shared_ptr<sdbusplus::asio::connection>& conn) :
        conn(conn), objServer(sdbusplus::asio::object_server(conn)),
        timer(conn->get_io_context())
    {
        createSensors();
        updateSensorValues();
    }

  private:
    std::shared_ptr<sdbusplus::asio::connection>& conn;
    sdbusplus::asio::object_server objServer;
    boost::asio::steady_timer timer;

    /** @brief Map of the object Sensors */
    std::unordered_map<std::string, std::unique_ptr<Sensor>> sensorsMap;

    void createSensors()
    {
        for (int i = 0; i < 3; i++)
        {
            std::string name = std::to_string(i);
            auto objPath = "/xyz/openbmc_project/sensors/temperature/" + name;
            auto sensorPtr =
                std::make_unique<Sensor>(conn, objPath.c_str(), name);
            lg2::debug("Created sensor: {PATH}", "PATH", objPath);

            sensorPtr->emit_object_added();
            sensorsMap.emplace(name, std::move(sensorPtr));
        }
    }

    void updateSensorValues()
    {
        timer.expires_after(std::chrono::seconds(1));
        timer.async_wait([this](const boost::system::error_code& ec) {
            if (ec == boost::asio::error::operation_aborted)
            {
                // expected, we were canceled before the timer completed.
                return;
            }
            if (ec)
            {
                lg2::error("Update failed: {ERROR}", "ERROR", ec.message());
            }

            read();
            updateSensorValues();
        });
    }

    void read()
    {
        for (auto& [name, sensor] : sensorsMap)
        {
            sensor->read();
        }
    }
};
