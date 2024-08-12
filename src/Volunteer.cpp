#include "../include/Volunteer.h"
#include "../include/Order.h"
#include <sstream>

// Volunteer class implementation
Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

Volunteer::Volunteer(const Volunteer &other) 
    : completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId),id(other.id), name(other.name) {}

int Volunteer::getId() const {
    return id;
}

const string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
}

// CollectorVolunteer class implementation
CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other) 
    : Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft) {}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}
CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}
void CollectorVolunteer::setTimeLeft(int time) {
    this->timeLeft = time;
}
void CollectorVolunteer::step() {
    if (isBusy() && decreaseCoolDown()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft = timeLeft-1;
        if(timeLeft==0){
            completedOrderId = getActiveOrderId();
            return true;
        }
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true; // Always true for CollectorVolunteer
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setTimeLeft(this->coolDown);
}

string CollectorVolunteer::toString() const {
    std::stringstream res;
    res << "Collector Volunteer: " << getName() << " (ID: " << getId() << ")";
    return res.str();
}

// LimitedCollectorVolunteer class implementation
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}


LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other) 
    : CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft-1;
    setTimeLeft(this->getCoolDown()); 
}

string LimitedCollectorVolunteer::toString() const {
    std::stringstream res;
    res << "Limited Collector Volunteer: " << getName() << " (ID: " << getId() << ")";
    return res.str();
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

// DriverVolunteer class implementation
DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(maxDistance) {}

DriverVolunteer::DriverVolunteer(const DriverVolunteer &other) 
    : Volunteer(other), maxDistance(other.maxDistance), distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

void DriverVolunteer::step() {
    if (isBusy() && decreaseDistanceLeft()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft > 0) {
        distanceLeft -= distancePerStep;
        return distanceLeft <= 0;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true; // Always true for DriverVolunteer
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && order.getDistance() <= maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

string DriverVolunteer::toString() const {
    std::stringstream res;
    res << "Driver Volunteer: " << getName() << " (ID: " << getId() << ")";
    return res.str();
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

// LimitedDriverVolunteer class implementation
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other) 
    : DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order); // Call base class implementation
    ordersLeft = ordersLeft-1;
}

string LimitedDriverVolunteer::toString() const {
    std::stringstream res;
    res << "Limited Driver Volunteer: " << getName() << " (ID: " << getId() << ")";
    return res.str();
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}
