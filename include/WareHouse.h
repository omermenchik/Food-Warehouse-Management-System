#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;
class CollectorVolunteer;
class DriverVolunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.
class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        //added functions:
        WareHouse(const WareHouse& other);
        void delWarehouse();
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        int getWarehouseCustomerCounter() const;
        const vector<Order*>& getPendingOrders() const;
        const vector<Order*>& getInProcessOrders() const;
        const vector<Order*>& getCompletedOrders() const;
        virtual ~WareHouse();
        void performStep();
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getnumOfOrders() const;
        WareHouse(WareHouse &&other); 
        const WareHouse &operator=(const WareHouse &other); 
        const WareHouse &operator=(WareHouse &&other); 
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        //added functions:
        int numOfOrders;
        CollectorVolunteer* findAvailableCollector();
        DriverVolunteer* findAvailableDriver();
        CivilianCustomer* errorCustomer;
        CollectorVolunteer* errorVolunteer;
        Order* errorOrder;
};