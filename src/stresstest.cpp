#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <fstream>
#include "Reservation.h"
#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"

using namespace std;
using namespace std::chrono;
using namespace FerrySys; // For VehicleRecord

// Helper to reset binary files
void clearFile(const string &filename) {
    ofstream file(filename, ios::binary | ios::trunc);
}

// Create a simple vehicle record
VehicleRecord makeVehicle(const string &plate) {
    VehicleRecord v;
    v.license = plate;
    v.phone = "6045550000";
    v.length_cm = 1;    // minimal space
    v.height_cm = 100;  // low-ceiling
    return v;
}

int main() {
    // 1) Clear all files
    clearFile("sailings.dat");
    clearFile("reservations.dat");
    clearFile("vehicles.dat");

    // 2) Write sailing (ID=5001)
    FileIO_Sailings::writeSailing("5001", "StressFerry", 1200, 1200);

    // 3) Preload 1000 vehicle records
    for (int i = 0; i < 1000; i++) {
        string plate = "TST" + to_string(1000 + i).substr(1); // TST0000..TST0999
        VehicleRecord v = makeVehicle(plate);
        FileIO_VehicleRecord::writeVehicle(v); // Assumes this function exists
    }

    // 4) Stress test reservations
    bool allWithinTime = true;
    double maxTimeMs = 0;

    for (int i = 0; i < 1000; i++) {
        string plate = "TST" + to_string(1000 + i).substr(1);
        VehicleRecord v;
        FileIO_VehicleRecord::findVehicle(plate, v); // retrieve from file

        auto start = high_resolution_clock::now();

        if (!Reservation::newCustomerReservation(v, "5001")) {
            cerr << "Failed reservation for plate: " << plate << "\n";
            allWithinTime = false;
        }

        auto end = high_resolution_clock::now();
        double elapsedMs = duration<double, std::milli>(end - start).count();
        if (elapsedMs > 500.0) allWithinTime = false;
        if (elapsedMs > maxTimeMs) maxTimeMs = elapsedMs;
    }

    // 5) Final status
    int count = FileIO_Reservations::countReservationsForSailing("5001");
    cout << "Stress Test Completed\n";
    cout << "Max transaction time: " << fixed << setprecision(2) << maxTimeMs << " ms\n";
    //cout << "Result: " << (allWithinTime ? "PASS" : "FAIL") << "\n";
    cout << "Booked: " << count << " / 1200 vehicles\n";

    return 0;
}
