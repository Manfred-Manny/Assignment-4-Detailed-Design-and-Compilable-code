#include <fstream>
#include <cstring>
#include <string>

// Match your struct layout (same as ReservationRec in FileIO_Reservations)
#pragma pack(push, 1)
struct ReservationRec {
    char licenseplate[10]; // space padded
    char sailingID[16];    // space padded
    bool checkedIn;
};
#pragma pack(pop)

// Helper: space-pad encode field
void encodeField(const std::string &src, char *dest, std::size_t len) {
    std::memset(dest, ' ', len);
    std::memcpy(dest, src.data(), std::min(len, src.size()));
}

int main() {
    std::ofstream file("reservations.dat", std::ios::binary | std::ios::trunc);
    if (!file) {
      //  std::cerr << "Error: Could not open reservations.dat for writing\n";
        return 1;
    }

    for (int i = 0; i < 1000; ++i) {
        ReservationRec rec{};
        
        // License plate: CAR0, CAR1, etc.
        std::string license = "CAR" + std::to_string(i);
        encodeField(license, rec.licenseplate, 10);

        // Sailing ID (must exist in sailings.dat for status to work)
        encodeField("YVR:29:14", rec.sailingID, 16);

        rec.checkedIn = false;

        file.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    }

    file.close();
   // std::cout << "Generated 1000 reservations in reservations.dat\n";
    return 0;
}
