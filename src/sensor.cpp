#include "sensor.hpp"

#include <phosphor-logging/lg2.hpp>

#include <cmath>
#include <fstream>

std::filesystem::path simulationDirPath = "/tmp/sensor/simulation";

// see phosphor-logging/lib/include/phosphor-logging/lg2.hpp
PHOSPHOR_LOG2_USING_WITH_FLAGS;

void Sensor::initSensor()
{
    ValueIface::unit(ValueIface::Unit::DegreesC);
    ValueIface::maxValue(255);
    ValueIface::minValue(0);
}

void Sensor::read()
{
    double value = std::numeric_limits<double>::quiet_NaN();
    std::filesystem::path simulationFilePath = simulationDirPath / name;
    if (std::filesystem::exists(simulationFilePath))
    {
        std::ifstream simFile(simulationFilePath);
        if (simFile.is_open())
        {
            simFile >> value;
            simFile.close();
        }
        else
        {
            lg2::error("Failed to open simulation file {FILE}", "FILE",
                       simulationFilePath.string());
        }
    }
    if (!std::isnan(value))
    {
        value =
            std::clamp(value, ValueIface::minValue(), ValueIface::maxValue());
    }

    // In phosphor-dbus-interfaces Sensor/Value/server.cpp#L47 (Value::value),
    // when both _value and value are NaN, because NaN != NaN evaluates to true,
    // value_interface.property_changed("Value") will be triggered and a
    // property-changed signal will be emitted. However, the actual value has
    // not changed, which results in an unnecessary signal being sent.
    double _value = ValueIface::value();
    if ((std::isnan(_value) && std::isnan(value)) || _value == value)
    {
        return;
    }

    ValueIface::value(value);
}
