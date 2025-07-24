#ifndef VESSEL_H
#define VESSEL_H

#include <string>
#include <vector>
#include "CommonTypes.h"

// Status codes for create/delete vessel operations
enum class VesselStatus {
    SUCCESS,
    ALREADY_EXISTS,
    NOT_FOUND
};

class Vessel
{
public:
    static VesselStatus CreateVessel(
        const std::string &vesselName,
        unsigned int laneHCL,
        unsigned int laneLCL);

    static VesselStatus DeleteVessel(
        const std::string &vesselName);

    static bool isVesselExist(
        const std::string &vesselName);

    static void initialize();
    static void shutdown();

private:
    static std::vector<Vessel> vessels;
    static const std::string vesselFilePath;
};

#endif // VESSEL_H
