#pragma once
#include <vector>
#include "Customer.h"
#include "Volunteer.h"
#include <string>
#include "WareHouse.h"
class Parser {
public:
    static void parseConfigFile(const std::string& configFilePath, WareHouse& warehouse);
};
