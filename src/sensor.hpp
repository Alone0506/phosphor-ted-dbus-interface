#pragma once

#include <com/ted/Oshi/server.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <xyz/openbmc_project/Sensor/Value/server.hpp>

#include <string>

using ValueIface = sdbusplus::server::xyz::openbmc_project::sensor::Value;
using OshiNameIface = sdbusplus::server::com::ted::Oshi;

template <typename... T>
using ServerObject = typename sdbusplus::server::object_t<T...>;
using Iface = ServerObject<ValueIface, OshiNameIface>;

class Sensor : public Iface
{
  public:
    Sensor() = delete;
    virtual ~Sensor() = default;

    /** @brief Constructs Sensor
     *
     * @param[in] conn         - Handle to system dbus
     * @param[in] objPath      - The Dbus path of sensor
     * @param[in] name         - Sensor name
     */
    Sensor(std::shared_ptr<sdbusplus::asio::connection>& conn,
           const char* objPath, std::string name) :
        Iface(*conn, objPath, action::defer_emit), name(name)
    {
        initSensor();
    }

    void read();

  private:
    std::string name;

    void initSensor();

    // override from dbus-interfaces の OshiNameIface::changeOshi method
    std::string changeOshi(std::string oshi) override;
};
