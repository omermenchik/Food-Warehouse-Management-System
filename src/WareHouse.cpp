#include "../include/WareHouse.h"
#include "../include/Parser.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

WareHouse::WareHouse(const string &configFilePath)
    : isOpen(false),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(0),
      volunteerCounter(0),
      numOfOrders(0),
      errorCustomer(new CivilianCustomer(-1, "invalidCustomer", -1, -1)),
      errorVolunteer(new CollectorVolunteer(-1, "invalidVolunteer", -1)),
      errorOrder(new Order(-1, -1, -1)) {
    Parser::parseConfigFile(configFilePath, *this);
}

WareHouse::WareHouse(const WareHouse &other) 
    : isOpen(other.isOpen),
    actionsLog(),
    volunteers(), 
    pendingOrders(), 
    inProcessOrders(), 
    completedOrders(), 
    customers(), 
    customerCounter(other.customerCounter), 
    volunteerCounter(other.volunteerCounter),
    numOfOrders(other.numOfOrders),
    errorCustomer(other.errorCustomer->clone()), 
    errorVolunteer(other.errorVolunteer->clone()), 
    errorOrder(other.errorOrder->clone()) {
    for (auto& customer : other.customers){
        if (customer != nullptr) {
            customers.push_back(customer->clone());
        }
    }
    for (auto& volunteer : other.volunteers){
        if (volunteer != nullptr) {
            volunteers.push_back(volunteer->clone());
        }
    }
    for (auto& order : other.pendingOrders){
        if (order != nullptr) {
            pendingOrders.push_back(order->clone());
        }
    }
    for (auto& order : other.inProcessOrders){
        if (order != nullptr) {
            inProcessOrders.push_back(order->clone());
        }
    }
    for (auto& order : other.completedOrders){
        if (order != nullptr) {
            completedOrders.push_back(order->clone());
        }
    }
    for (auto& action : other.actionsLog){
        if (action != nullptr) {
            actionsLog.push_back(action->clone());
        }
    }
}

const WareHouse &WareHouse::operator=(const WareHouse &other) {
    if (this != &other) {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        numOfOrders = other.numOfOrders;
        delWarehouse(); // Call destructor to free existing resources
        for (auto* customer : other.customers){
            customers.push_back(customer->clone()); 
        }
        for (auto* volunteer : other.volunteers){
            volunteers.push_back(volunteer->clone());
        }
        for (auto* order : other.pendingOrders){
            pendingOrders.push_back(order->clone());
        }
        for (auto* order : other.inProcessOrders){
            inProcessOrders.push_back(order->clone());
        }
        for (auto* order : other.completedOrders){
            completedOrders.push_back(order->clone());
        }
        for (auto* action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }
        errorCustomer = new CivilianCustomer(-1, "invalidCustomer", -1, -1);
        errorVolunteer = new CollectorVolunteer(-1, "invalidVolunteer", -1);
        errorOrder = new Order(-1, -1, -1);
    }
    return *this; 
}

const WareHouse &WareHouse::operator=(WareHouse &&other) {
    isOpen = other.isOpen;
    delWarehouse();
    actionsLog = std::move(other.actionsLog);
    volunteers = std::move(other.volunteers);
    pendingOrders = std::move(other.pendingOrders);
    inProcessOrders = std::move(other.inProcessOrders);
    completedOrders = std::move(other.completedOrders);
    customers = std::move(other.customers);
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
    numOfOrders = other.numOfOrders;
    errorCustomer = new CivilianCustomer(-1, "invalidCustomer", -1, -1);
    errorVolunteer = new CollectorVolunteer(-1, "invalidVolunteer", -1);
    errorOrder = new Order(-1, -1, -1);
    return *this;
}

WareHouse::WareHouse(WareHouse &&other)
    : isOpen(other.isOpen),
    actionsLog(std::move(other.actionsLog)),
    volunteers(std::move(other.volunteers)),
    pendingOrders(std::move(other.pendingOrders)),
    inProcessOrders(std::move(other.inProcessOrders)),
    completedOrders(std::move(other.completedOrders)),
    customers(std::move(other.customers)),
    customerCounter(other.customerCounter), 
    volunteerCounter(other.volunteerCounter), 
    numOfOrders(other.numOfOrders),
    errorCustomer(std::move(other.errorCustomer)),
    errorVolunteer(std::move(other.errorVolunteer)),
    errorOrder(std::move(other.errorOrder)){}

void WareHouse::delWarehouse(){
    for (auto* order : pendingOrders) delete order;
    for (auto* order : inProcessOrders) delete order;
    for (auto* order : completedOrders) delete order;
    for (auto* customer : customers) delete customer;
    for (auto* volunteer : volunteers) delete volunteer;
    for (auto* action : WareHouse::actionsLog) delete action;

    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();
    volunteers.clear();
    actionsLog.clear();
    delete errorCustomer; 
    delete errorVolunteer;
    delete errorOrder;
}

WareHouse::~WareHouse() {
    delWarehouse();
}

void WareHouse::start() {
    cout << "Warehouse is open!" << endl;
    isOpen = true;

    std::string command;
    while (isOpen) {
        cout << "Enter action: ";
        getline(cin, command);
        if (command.empty()) continue;

        std::istringstream iss(command);
        std::string actionType;
        iss >> actionType;
        
        if (actionType == "step") {
            int steps;
            iss >> steps;
            // Process simulate step action
            SimulateStep* action = new SimulateStep(steps);
            action->act(*this);
            addAction(action);
        }
        else if (actionType == "customer") {
            std::string customerName, customerType;
            int locationDistance, maxOrders;
            iss >> customerName >> customerType >> locationDistance >> maxOrders;
            if(customerType=="solider")
            {
                AddCustomer* action = new AddCustomer(customerName,"Soldier", locationDistance, maxOrders);
                action->act(*this);
                addAction(action);
            }
            else
            {
                AddCustomer* action = new AddCustomer(customerName,"Civilian", locationDistance, maxOrders);
                action->act(*this);
                addAction(action);
            }
        }

        else if (actionType == "customerStatus") {
            int customerId;
            iss >> customerId;

            PrintCustomerStatus* action = new PrintCustomerStatus(customerId);
            action->act(*this);
            addAction(action);
        }

        else if (actionType == "volunteerStatus") {
            int volunteerId;
            iss >> volunteerId;
            PrintVolunteerStatus* action = new PrintVolunteerStatus(volunteerId);
            action->act(*this);
            addAction(action);
        }

        else if (actionType == "order") {
            int customerId;
            iss >> customerId;
            // Process add order action
            AddOrder* action = new AddOrder(customerId);
            action->act(*this);
            addAction(action);
        }
        else if (actionType == "orderStatus") {
            int orderId;
            iss >> orderId;
            // Process print order status action
            PrintOrderStatus* action = new PrintOrderStatus(orderId);
            action->act(*this);
            addAction(action);
        }
        else if (actionType == "log") {
            PrintActionsLog* action = new PrintActionsLog();
            action->act(*this);
            addAction(action);
        }
        else if (actionType == "close") {
            // Process close action
            Close* action = new Close();
            action->act(*this);
            addAction(action);
            break; // Exit the loop
        }
        else if (actionType == "restore") {
            RestoreWareHouse* action = new RestoreWareHouse();
            action->act(*this);
            addAction(action);
        }
        else if (actionType == "backup") {
            BackupWareHouse* action = new BackupWareHouse();
            action->act(*this);
            addAction(action);
        }
        else {
            cout << "Unknown action: " << actionType << endl;
        }
    }

    cout << "Warehouse is closed." << endl;
}


void WareHouse::addOrder(Order* order) {
    if (order != nullptr) {
        numOfOrders=numOfOrders+1;
        pendingOrders.push_back(order);
    }
}

int WareHouse::getnumOfOrders() const {
    return numOfOrders;
}

void WareHouse::addAction(BaseAction* action){
    if(action!=nullptr){
        actionsLog.push_back(action);
    }
}
int WareHouse::getCustomerCounter() const{
    return customerCounter;
}

int WareHouse::getVolunteerCounter() const {
    return volunteerCounter;
}

Customer& WareHouse::getCustomer(int customerId) const {
    for (auto& customer : customers) {
        if (customer != nullptr && customer->getId() == customerId) {
            return *customer;
        }
    }
    return *errorCustomer;
}

void WareHouse::addCustomer(Customer* customer) {
    if (customer != nullptr) {
        customers.push_back(customer);
        customerCounter = customerCounter + 1;
    }
}

void WareHouse::addVolunteer(Volunteer* volunteer) {
    if (volunteer != nullptr) {
        volunteers.push_back(volunteer);
        volunteerCounter = volunteerCounter + 1;
    }
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
    for (auto* volunteer : volunteers) {
        if (volunteer != nullptr && volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    return *errorVolunteer;
    
}

Order& WareHouse::getOrder(int orderId) const{
    for (auto& order : pendingOrders) {
        if (order != nullptr && order->getOrderId() == orderId) {
            return *order;
        }
    }
       for (auto& order : inProcessOrders) {
        if (order != nullptr && order->getOrderId() == orderId) {
            return *order;
        }
    }
       for (auto& order : completedOrders) {
        if (order != nullptr && order->getOrderId() == orderId) {
            return *order;
        }
    }
    return *errorOrder;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}

int WareHouse::getWarehouseCustomerCounter() const {
    return customerCounter;
}

const vector<Order*>& WareHouse::getPendingOrders() const {
    return pendingOrders;
}

const vector<Order*>& WareHouse::getInProcessOrders() const {
    return inProcessOrders;
}

const vector<Order*>& WareHouse::getCompletedOrders() const {
    return completedOrders;
}


CollectorVolunteer* WareHouse::findAvailableCollector() {
    for (auto& volunteer : volunteers) {
        if (auto* collector = dynamic_cast<CollectorVolunteer*>(volunteer)) {
            if (!collector->isBusy() && collector->hasOrdersLeft()) {
                return collector;
            }
        }
    }
    return nullptr;
}
DriverVolunteer* WareHouse::findAvailableDriver() {
    for (auto& volunteer : volunteers) {
        if (auto* driver = dynamic_cast<DriverVolunteer*>(volunteer)) {
            if (!driver->isBusy() && driver->hasOrdersLeft()) {
                return driver;
            }
        }
    }
    return nullptr;
}

void WareHouse::performStep() {
    // Process pending orders for collection
    for (auto it = pendingOrders.begin(); it != pendingOrders.end();) {
        Order* order = *it;
        if (order->getStatus() == OrderStatus::PENDING) {
            CollectorVolunteer* collector = findAvailableCollector();
            if (collector != nullptr && collector->canTakeOrder(*order)) {
                collector->acceptOrder(*order);
                order->setCollectorId(collector->getId());
                order->setStatus(OrderStatus::COLLECTING);
                it = pendingOrders.erase(it);
                inProcessOrders.push_back(order);
                continue;
            }
        }
        ++it;
    }
    // Assign drivers to pending orders that were collected
    for (auto it = pendingOrders.begin(); it != pendingOrders.end();) {
        Order* order = *it;
        if (order->getStatus() == OrderStatus::COLLECTING) {
            DriverVolunteer* driver = findAvailableDriver();
            if (driver != nullptr && driver->canTakeOrder(*order)) {
                driver->acceptOrder(*order);
                order->setDriverId(driver->getId());
                order->setStatus(OrderStatus::DELIVERING);
                it = pendingOrders.erase(it);
                inProcessOrders.push_back(order);
                continue; 
            }
        }
        ++it;
    }
    for (auto& volunteer : volunteers) {
        volunteer->step();
    }
    // Process in-process orders for delivery or completion
    for (auto it = inProcessOrders.begin(); it != inProcessOrders.end();) {
        Order* order = *it;
        if (order->getStatus() == OrderStatus::COLLECTING) {
            int collectorId = order->getCollectorId();
            Volunteer& collector = getVolunteer(collectorId);
            if (!collector.isBusy()) {
                it = inProcessOrders.erase(it);
                pendingOrders.push_back(order);
                continue; 
            }
        } else if (order->getStatus() == OrderStatus::DELIVERING) {
            int driverId = order->getDriverId();
            Volunteer& driver = getVolunteer(driverId);
            if (!driver.isBusy()) {
                order->setStatus(OrderStatus::COMPLETED);
                it = inProcessOrders.erase(it);
                completedOrders.push_back(order);
                continue; 
            }
        }
        ++it;
    }

    // Remove volunteers who have reached their max orders limit
    for (auto it = volunteers.begin(); it != volunteers.end(); ) {
        if (!(*it)->hasOrdersLeft() && !(*it)->isBusy()) {
            Volunteer* volunteer = *it;
            it = volunteers.erase(it);
            delete volunteer;
        } else {
            ++it;
        }
    }
}

