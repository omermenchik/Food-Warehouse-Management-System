// Parser.cpp
#include <fstream>
#include <sstream>
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/WareHouse.h"
#include "../include/Parser.h"

void Parser::parseConfigFile(const std::string& configFilePath, WareHouse& warehouse) {
    std::ifstream configFile(configFilePath);
    std::string line;
    int customerId = 0;  
    int volunteerId = 0; 

    while (getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "customer") {
            std::string name, customerType;
            int locationDistance, maxOrders;
            iss >> name >> customerType >> locationDistance >> maxOrders;

            Customer *newCustomer = nullptr;
            if (customerType == "soldier") {
                newCustomer = new SoldierCustomer(customerId, name, locationDistance, maxOrders);
            } else if (customerType == "civilian") {
                newCustomer = new CivilianCustomer(customerId, name, locationDistance, maxOrders);
            }
            if (newCustomer) {
                warehouse.addCustomer(newCustomer);
                customerId++;
            }
        } else if (type == "volunteer") {
            std::string name, volunteerRole;
            int coolDownOrMaxDistance, distancePerStep = 0, maxOrders = -1; // Default values
            iss >> name >> volunteerRole >> coolDownOrMaxDistance;

            if (volunteerRole == "driver" || volunteerRole == "limited_driver") {
                iss >> distancePerStep; // Only for drivers
            }
            if (volunteerRole == "limited_collector" || volunteerRole == "limited_driver") {
                iss >> maxOrders; // Only for limited volunteers
            }

            Volunteer *newVolunteer = nullptr;
            if (volunteerRole == "collector") {
                newVolunteer = new CollectorVolunteer(volunteerId, name, coolDownOrMaxDistance);
            } else if (volunteerRole == "limited_collector") {
                newVolunteer = new LimitedCollectorVolunteer(volunteerId, name, coolDownOrMaxDistance, maxOrders);
            } else if (volunteerRole == "driver") {
                newVolunteer = new DriverVolunteer(volunteerId, name, coolDownOrMaxDistance, distancePerStep);
            } else if (volunteerRole == "limited_driver") {
                newVolunteer = new LimitedDriverVolunteer(volunteerId, name, coolDownOrMaxDistance, distancePerStep, maxOrders);
            }
            if (newVolunteer) {
                warehouse.addVolunteer(newVolunteer);
                volunteerId++;
            }
        } else if (type == "Order") {
            int orderId, customerId;
            iss >> orderId >> customerId;
        }
    }
}

