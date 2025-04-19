#include <iostream>

class DeviceConfig {
public:
    // Defining the configuration settings with bit fields
    struct ConfigFlags {
        unsigned char powerMode : 1;            // 1 bit for power mode (on/off)
        unsigned char commProtocol : 2;         // 2 bits for communication protocol
        unsigned char sensorStatus : 3;         // 3 bits for sensor states
        unsigned char errorFlags : 2;           // 2 bits for error states
    };

    ConfigFlags config;

    // Constructor to initialize all flags to 0
    DeviceConfig() {
        config.powerMode = 0;
        config.commProtocol = 0;
        config.sensorStatus = 0;
        config.errorFlags = 0;
    }

    // Set values for different configuration settings
    void setConfig(unsigned int power, unsigned int protocol, unsigned int sensors, unsigned int errors) {
        config.powerMode = power;
        config.commProtocol = protocol;
        config.sensorStatus = sensors;
        config.errorFlags = errors;
    }

    // Print configuration settings
    void printConfig() {
        std::cout << "Power Mode: " << (config.powerMode == 0 ? "Off" : "On") << std::endl;
        std::cout << "Communication Protocol: " << config.commProtocol << std::endl;
        std::cout << "Sensor Status: " << config.sensorStatus << std::endl;
        std::cout << "Error Flags: " << config.errorFlags << std::endl;
    }

    // Display the binary representation of the config
    void displayBinary() {
        std::cout << "Binary Configuration: ";
        std::cout << (config.powerMode ? "1" : "0");
        std::cout << (config.commProtocol & 0x02 ? "1" : "0");
        std::cout << (config.commProtocol & 0x01 ? "1" : "0");
        std::cout << (config.sensorStatus & 0x04 ? "1" : "0");
        std::cout << (config.sensorStatus & 0x02 ? "1" : "0");
        std::cout << (config.sensorStatus & 0x01 ? "1" : "0");
        std::cout << (config.errorFlags & 0x02 ? "1" : "0");
        std::cout << (config.errorFlags & 0x01 ? "1" : "0");
        std::cout << std::endl;
    }

    // Show the size of the configuration struct
    void showSize() {
        std::cout << "Size of ConfigFlags struct: " << sizeof(ConfigFlags) << " bytes" << std::endl;
    }
};

int main() {
    // Create a device configuration instance
    DeviceConfig device;

    // Set configuration (Power mode: On, Protocol: SPI (01), Sensor: 5 (binary 101), Error Flags: 1 (binary 01))
    device.setConfig(1, 1, 5, 1);

    // Print the configuration values
    device.printConfig();

    // Display binary representation of the configuration
    device.displayBinary();

    // Show size of the configuration structure
    device.showSize();

    return 0;
}
