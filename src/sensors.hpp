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
        conn(conn)
    {
        createSensor();
    }

  private:
    std::shared_ptr<sdbusplus::asio::connection>& conn;

    /** @brief Map of the object Sensors */
    std::unique_ptr<Sensor> sensorPtr;

    void createSensor()
    {
        std::string name = "0";
        auto objPath = "/xyz/openbmc_project/sensors/temperature/" + name;
        sensorPtr = std::make_unique<Sensor>(conn, objPath.c_str(), name);
        lg2::debug("Created sensor: {PATH}", "PATH", objPath);

        sensorPtr->emit_object_added();
    }
};
