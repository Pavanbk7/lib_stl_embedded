#include <iostream>
#include <estl.hpp>

// Example demonstrating the use of estl::vector for embedded systems

struct Sensor {
    int id;
    float value;
    
    Sensor() : id(0), value(0.0f) {}
    Sensor(int i, float v) : id(i), value(v) {}
    
    // For demonstration purposes
    friend std::ostream& operator<<(std::ostream& os, const Sensor& s) {
        os << "Sensor(id=" << s.id << ", value=" << s.value << ")";
        return os;
    }
};

int main() {
    // Create a vector with a capacity of 10 Sensor objects
    estl::vector<Sensor, 10> sensors;
    
    std::cout << "Vector capacity: " << sensors.capacity() << std::endl;
    std::cout << "Vector size: " << sensors.size() << std::endl;
    std::cout << "Vector empty: " << (sensors.empty() ? "true" : "false") << std::endl;
    
    // Add elements to the vector
    std::cout << "\nAdding elements..." << std::endl;
    sensors.push_back(Sensor(1, 25.5f));
    sensors.push_back(Sensor(2, 30.2f));
    sensors.push_back(Sensor(3, 15.7f));
    
    std::cout << "Vector size after adding elements: " << sensors.size() << std::endl;
    
    // Access elements
    std::cout << "\nAccessing elements:" << std::endl;
    std::cout << "First element: " << sensors.front() << std::endl;
    std::cout << "Last element: " << sensors.back() << std::endl;
    std::cout << "Element at index 1: " << sensors[1] << std::endl;
    
    // Iterate through the vector
    std::cout << "\nIterating through all elements:" << std::endl;
    for (const auto& sensor : sensors) {
        std::cout << sensor << std::endl;
    }
    
    // Using iterators explicitly
    std::cout << "\nUsing explicit iterators:" << std::endl;
    for (auto it = sensors.begin(); it != sensors.end(); ++it) {
        std::cout << "Sensor ID: " << it->id << ", Value: " << it->value << std::endl;
    }
    
    // Modify elements
    std::cout << "\nModifying elements..." << std::endl;
    sensors[0].value = 26.7f;
    std::cout << "Modified first element: " << sensors[0] << std::endl;
    
    // Insert element at position
    std::cout << "\nInserting element at position 1..." << std::endl;
    sensors.insert(sensors.begin() + 1, Sensor(4, 22.3f));
    
    std::cout << "Vector after insertion:" << std::endl;
    for (const auto& sensor : sensors) {
        std::cout << sensor << std::endl;
    }
    
    // Remove last element
    std::cout << "\nRemoving last element..." << std::endl;
    sensors.pop_back();
    
    std::cout << "Vector after removal:" << std::endl;
    for (const auto& sensor : sensors) {
        std::cout << sensor << std::endl;
    }
    
    // Clear the vector
    std::cout << "\nClearing the vector..." << std::endl;
    sensors.clear();
    std::cout << "Vector size after clearing: " << sensors.size() << std::endl;
    std::cout << "Vector empty: " << (sensors.empty() ? "true" : "false") << std::endl;
    
    return 0;
}
