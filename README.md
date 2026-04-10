# phosphor-ted-dbus-interface

`phosphor-ted-dbus-interface` is a minimal OpenBMC D-Bus service example. At
startup it requests the bus name `xyz.openbmc_project.TedDbusInterface` and
creates one sensor object at:

- `/xyz/openbmc_project/sensors/temperature/0`

## D-Bus Summary

- Service name: `xyz.openbmc_project.TedDbusInterface`
- Object path: `/xyz/openbmc_project/sensors/temperature/0`

Implemented interfaces on the sensor object:

| Interface                          | Purpose                                |
| ---------------------------------- | -------------------------------------- |
| `xyz.openbmc_project.Sensor.Value` | Standard sensor reading/unit/min/max   |
| `com.ted.Oshi`                     | Custom demo properties/methods/signals |

## Behavior

Current implementation in `src/sensor.hpp` initializes the Value interface as:

- `Unit = DegreesC`
- `MaxValue = 255`
- `MinValue = 0`

The `ChangeOshi` method (from `com.ted.Oshi`) updates `OshiName` and returns the
updated string.

## YAML Interface Sources

This project uses generated C++ file from these YAML interface files:

- `meta-ted/recipes-phosphor/dbus/phosphor-dbus-interfaces/yaml/xyz/openbmc_project/Sensor/Value.interface.yaml`
- `meta-ted/recipes-phosphor/dbus/phosphor-dbus-interfaces/yaml/com/ted/Oshi.interface.yaml`

In code, they are included as generated headers:

- `<xyz/openbmc_project/Sensor/Value/server.hpp>`
- `<com/ted/Oshi/server.hpp>`

## Useful Commands

```bash
# Check whether the service is running and view recent logs/status
systemctl status phosphor-ted-dbus-interface.service

# Show all object paths currently exposed by this D-Bus service
busctl tree xyz.openbmc_project.TedDbusInterface

# Inspect object and interfaces
busctl introspect xyz.openbmc_project.TedDbusInterface /xyz/openbmc_project/sensors/temperature/0

# Set com.ted.Oshi property Love
busctl set-property xyz.openbmc_project.TedDbusInterface /xyz/openbmc_project/sensors/temperature/0 com.ted.Oshi Love s Ted

# Call ChangeOshi method
busctl call xyz.openbmc_project.TedDbusInterface /xyz/openbmc_project/sensors/temperature/0 com.ted.Oshi ChangeOshi s Miko
```
