#include <iostream>
#include <chrono>
#include "Sailing.h"  // Include your sailing status function

int main() {
    std::string sailingID = "YVR:29:14"; // Must match seeded reservations

    auto start = std::chrono::high_resolution_clock::now();

    // Run the function you're timing
    Sailing::printStatus(sailingID);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken: " << elapsed.count() << " seconds\n";
    return 0;
}
