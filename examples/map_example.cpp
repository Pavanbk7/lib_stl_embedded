#include <iostream>
#include <string>
#include <estl.hpp>

// Example demonstrating the use of estl::map for embedded systems

struct DeviceInfo {
    std::string name;
    float temperature;
    
    DeviceInfo() : name(""), temperature(0.0f) {}
    DeviceInfo(const std::string& n, float t) : name(n), temperature(t) {}
    
    // For demonstration purposes
    friend std::ostream& operator<<(std::ostream& os, const DeviceInfo& d) {
        os << "DeviceInfo(name=" << d.name << ", temperature=" << d.temperature << "°C)";
        return os;
    }
};

int main() {
    // Create a map with a capacity of 8 elements
    estl::map<int, DeviceInfo, estl::less<int>, 8> devices;
    
    std::cout << "Map capacity: " << devices.max_size() << std::endl;
    std::cout << "Map size: " << devices.size() << std::endl;
    std::cout << "Map empty: " << (devices.empty() ? "true" : "false") << std::endl;
    
    // Add elements to the map
    std::cout << "\nAdding elements..." << std::endl;
    devices.insert(std::make_pair(1, DeviceInfo("Temperature Sensor", 25.5f)));
    devices.insert(std::make_pair(2, DeviceInfo("Humidity Sensor", 30.2f)));
    devices.insert(std::make_pair(3, DeviceInfo("Pressure Sensor", 15.7f)));
    
    std::cout << "Map size after adding elements: " << devices.size() << std::endl;
    
    // Access elements using operator[]
    std::cout << "\nAccessing elements using operator[]:" << std::endl;
    std::cout << "Device 1: " << devices[1] << std::endl;
    std::cout << "Device 2: " << devices[2] << std::endl;
    
    // Access elements using at()
    std::cout << "\nAccessing elements using at():" << std::endl;
    std::cout << "Device 3: " << devices.at(3) << std::endl;
    
    // Iterate through the map
    std::cout << "\nIterating through all elements:" << std::endl;
    for (const auto& pair : devices) {
        std::cout << "Device ID: " << pair.first << ", Info: " << pair.second << std::endl;
    }
    
    // Using iterators explicitly
    std::cout << "\nUsing explicit iterators:" << std::endl;
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        std::cout << "Device ID: " << it->first << ", Name: " << it->second.name << std::endl;
    }
    
    // Modify elements
    std::cout << "\nModifying elements..." << std::endl;
    devices[1].temperature = 26.7f;
    std::cout << "Modified device 1: " << devices[1] << std::endl;
    
    // Check if a key exists
    int key_to_find = 4;
    std::cout << "\nChecking if key " << key_to_find << " exists: ";
    if (devices.find(key_to_find) != devices.end()) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    // Add a new element using operator[]
    std::cout << "\nAdding a new element using operator[]..." << std::endl;
    devices[4] = DeviceInfo("Motion Sensor", 22.3f);
    
    std::cout << "Map after adding new element:" << std::endl;
    for (const auto& pair : devices) {
        std::cout << "Device ID: " << pair.first << ", Info: " << pair.second << std::endl;
    }
    
    // Remove an element
    std::cout << "\nRemoving element with key 2..." << std::endl;
    devices.erase(2);
    
    std::cout << "Map after removal:" << std::endl;
    for (const auto& pair : devices) {
        std::cout << "Device ID: " << pair.first << ", Info: " << pair.second << std::endl;
    }
    
    // Clear the map
    std::cout << "\nClearing the map..." << std::endl;
    devices.clear();
    std::cout << "Map size after clearing: " << devices.size() << std::endl;
    std::cout << "Map empty: " << (devices.empty() ? "true" : "false") << std::endl;
    
    return 0;
}
