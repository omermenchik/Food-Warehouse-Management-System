#include "../include/WareHouse.h"
#include "../include/Parser.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include <iostream>
using namespace std;
extern WareHouse* backup; 

// Constructor
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) { 
}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {} 

ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction:: setStatus(ActionStatus status){
    this->status = status;
}
CustomerType BaseAction::getCustomerType(const string& customerTypeString) const {
    if (customerTypeString == "Soldier") {
        return CustomerType::Soldier;
    } else if (customerTypeString == "Civilian") {
        return CustomerType::Civilian;
    } else {
        throw std::invalid_argument("Unknown customer type: " + customerTypeString);
    }
}
string BaseAction::customerTypeToString(CustomerType customerType) const {
    switch (customerType) {
        case CustomerType::Soldier:
            return "Soldier";
        case CustomerType::Civilian:
            return "Civilian";
        default:
            throw std::invalid_argument("Unknown CustomerType enum value");
    }
}

string BaseAction::getActionStatus() const {
    if (getStatus() == ActionStatus::COMPLETED) {
        return "COMPLETED";
    } else {
        return "ERROR";
    }
}
void BaseAction::complete() {
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errMsg;
    cout << "Error: " << errMsg << endl;
}
string BaseAction::getErr() const {

    return errorMsg;
}

//SimulateStep:
SimulateStep :: SimulateStep (int numOfSteps):
    BaseAction(), numOfSteps(numOfSteps){}
SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(other), numOfSteps(other.numOfSteps) {} 
void SimulateStep::act(WareHouse &wareHouse){
    for (int i = 0; i < numOfSteps ; i++)
    {
        wareHouse.performStep();
    }
    complete();
}

string SimulateStep:: toString() const{
    return "simulateStep " + to_string(numOfSteps) + " " + getActionStatus();
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}
//addorder
AddOrder :: AddOrder(int id):
    BaseAction(), customerId(id){}

AddOrder::AddOrder(const AddOrder &other) : BaseAction(other), customerId(other.customerId) {} 

void AddOrder::act(WareHouse &wareHouse) {
    if (customerId < 0 || customerId > wareHouse.getWarehouseCustomerCounter()) {
        cout << "Cannot place this order" << endl;
        setStatus(ActionStatus::ERROR);
    } else {
        Customer& customer = wareHouse.getCustomer(customerId);
        if (!customer.canMakeOrder()) {
            cout << "Error: Cannot place this order" << endl;
            setStatus(ActionStatus::ERROR);
        } else {
            int orderId = wareHouse.getnumOfOrders(); 
            Order* order = new Order(orderId, customerId, customer.getCustomerDistance());
            wareHouse.addOrder(order);
            customer.addOrder(orderId);
            setStatus(ActionStatus::COMPLETED);
        }
    }
}

string AddOrder::toString() const{
    return "order " + to_string(customerId) + " " + getActionStatus();
}
AddOrder* AddOrder:: clone() const{
    return new AddOrder(*this);
}

//AddCustomer
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders):
    BaseAction(), customerName(customerName),customerType(getCustomerType(customerType)),distance(distance), maxOrders(maxOrders){}

AddCustomer::AddCustomer(const AddCustomer &other) 
    : BaseAction(other), customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders) {} 

void AddCustomer::act(WareHouse &wareHouse) {
    int customerId = wareHouse.getWarehouseCustomerCounter();
     if (customerType == CustomerType::Civilian)
    {
        Customer* customer = new CivilianCustomer(customerId, customerName,distance,maxOrders);
        wareHouse.addCustomer(customer);
    }
    if (customerType == CustomerType::Soldier)
    {
        Customer* customer = new SoldierCustomer (customerId, customerName,distance,maxOrders);
        wareHouse.addCustomer(customer);
    }
    setStatus(ActionStatus::COMPLETED);
}

AddCustomer* AddCustomer:: clone() const{
    return new AddCustomer(*this);
}

string AddCustomer:: toString() const{
    return "customer " + customerName + " " + customerTypeToString(customerType) + " " + to_string(distance) + " " + to_string(maxOrders) + " " + getActionStatus();
}

//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id):
    BaseAction(), orderId(id){}
PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other) : BaseAction(other), orderId(other.orderId) {} 

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order& order = wareHouse.getOrder(orderId);
    if (order.getId() < 0 || orderId >= wareHouse.getnumOfOrders()) {
        error("Order doesn't exist");
        return;
    } 
    else
    {
        cout << "OrderId: " << order.getId() << endl;
        cout << "OrderStatus: " << Order::orderStatusToString(order.getStatus()) << endl;
        cout << "CustomerID: " << order.getCustomerId() << endl;
        int collectorId = order.getCollectorId();
        cout << "Collector: " << (collectorId != NO_VOLUNTEER ? to_string(collectorId) : "None") << endl;
        int driverId = order.getDriverId();
        cout << "Driver: " << (driverId != NO_VOLUNTEER ? to_string(driverId) : "None") << endl;
        complete();
    }
}
PrintOrderStatus*  PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const {
    return "orderStatus " + to_string(orderId) + " " + getActionStatus();
}

//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}
PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other) : BaseAction(other), customerId(other.customerId) {} 

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if (customerId < 0 || customerId > wareHouse.getCustomerCounter() || wareHouse.getCustomer(customerId).getMaxOrders() < wareHouse.getCustomer(customerId).getNumOrders()) {
        error("Customer doesn't exist");
        return;
    }   
    else{
        cout << "CustomerID: " << customerId << endl;
        for (auto& orderIds : wareHouse.getCustomer(customerId).getOrdersIds()) {
            if(orderIds>=0)
            {
                cout << "OrderId: " << orderIds << endl;
                cout << "OrderStatus: " << wareHouse.getOrder(orderIds).orderStatusToString(wareHouse.getOrder(orderIds).getStatus()) << endl;
            }
        }
         cout << "numOrdersLeft: " << (wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) << endl; 
        this->complete();
    }
}
string PrintCustomerStatus::toString() const {
    return "customerStatus " + to_string(customerId) + " " + getActionStatus();
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}
PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other) : BaseAction(other), volunteerId(other.volunteerId) {} 

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if (volunteerId < 0 || volunteerId >= wareHouse.getVolunteerCounter()) {
        error("Volunteer doesn't exist");
        return;
    }
    else {
        Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
        if(volunteer.getId() == -1){
            error("Volunteer doesn't exist");
            return;
        }
        else {
            cout << "VolunteerId: " << volunteerId << endl;
            cout << "isBusy: " << (volunteer.isBusy() ? "True" : "False") << endl;
            cout << "OrderID: " << (volunteer.isBusy() ? to_string(volunteer.getActiveOrderId()) : "None") << endl;
            if (auto* driver = dynamic_cast<DriverVolunteer*>(&volunteer)) {
                cout << "TimeLeft: " << (driver->getDistanceLeft() == 0 ? "None" : to_string(driver->getDistanceLeft())) << endl;
            }
            else if (auto* collector = dynamic_cast<CollectorVolunteer*>(&volunteer)) {
                cout << "TimeLeft: " << (collector->getTimeLeft() == 0 ? "None" : to_string(collector->getTimeLeft())) << endl;
            }
            else {
                cout << "TimeLeft: None" << endl;
            }
            if (auto* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(&volunteer)) {
                cout << "OrdersLeft: " << limitedDriver->getNumOrdersLeft() << endl;
            }
            else if (auto* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(&volunteer)) {
                cout << "OrdersLeft: " << limitedCollector->getNumOrdersLeft() << endl;
            }
            else {
                cout << "OrdersLeft: No Limit" << endl;
            }
            this->complete();
        }   
    }
}

string PrintVolunteerStatus::toString() const {
    return "volunteerStatus " + to_string(volunteerId) + " " + getActionStatus();
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog() : BaseAction() {}
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {} 


void PrintActionsLog::act(WareHouse &wareHouse) {
    const vector<BaseAction*>& actions = wareHouse.getActions();
    for (const BaseAction* action : actions) {
        if (action != this) { 
            cout << action->toString() << endl;
        }
    }
    this->complete(); 
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log " + getActionStatus();
}

//Close
Close::Close() : BaseAction() {}
Close::Close(const Close &other) : BaseAction() {} 

void Close::act(WareHouse &wareHouse) {
    auto printOrders = [&](const vector<Order*>& orders) {
        for (const auto& order : orders) {
            if (order != nullptr) {
                cout << "OrderID: " << order->getId() 
                     << ", CustomerID: " << order->getCustomerId() 
                     << ", Status: " << Order::orderStatusToString(order->getStatus()) << endl;
            }
        }
    };
    printOrders(wareHouse.getPendingOrders());
    printOrders(wareHouse.getInProcessOrders());
    printOrders(wareHouse.getCompletedOrders());
    wareHouse.close();
    this->complete(); 
}


Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "Close " + getActionStatus();
}



//BackupWareHouse
BackupWareHouse::BackupWareHouse() : BaseAction() {}
BackupWareHouse::BackupWareHouse(const BackupWareHouse &other) : BaseAction(other) {} 

void BackupWareHouse::act(WareHouse &wareHouse) {
    if(backup!=nullptr){
        delete backup;  
    }
    backup = new WareHouse(wareHouse); 
    complete(); 
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "BackupWareHouse " + getActionStatus();
}

//RestoreWareHouse
RestoreWareHouse::RestoreWareHouse(): BaseAction(){}

RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other) : BaseAction(other) {} 

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("No backup available");
        return;
    }
    wareHouse = *backup;  
    complete();   
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "restore " + getActionStatus();
}





