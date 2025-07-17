#include "Sailing.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// ---------------------------------------------------------------------------

bool Sailing::CreateSailing(const std::string &,
                            const std::string &,
                            const std::string &,
                            const std::string &)
{
    std::cout << "[STUB] Sailing::CreateSailing()\n";
    return false;
}

bool Sailing::DeleteSailing(SailingID)
{
    std::cout << "[STUB] Sailing::DeleteSailing()\n";
    return false;
}

void Sailing::printStatus(SailingID)
{
    std::cout << "[STUB] Sailing::printStatus()\n";
}

void Sailing::printReport()
{
    std::cout << "[STUB] Sailing::printReport()\n";
}

bool Sailing::isSailingExist(SailingID)
{
    return false;
}

bool Sailing::isVesselExist(const std::string &)
{
    return false;
}

void Sailing::initialize()
{
    std::cout << "[STUB] Sailing::initialize()\n";
}

void Sailing::shutdown()
{
    std::cout << "[STUB] Sailing::shutdown()\n";
}
