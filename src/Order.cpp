#include "../include/Order.h"
#include <sstream>

Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
      collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

const std::string Order::toString() const {
    std::stringstream res;
    res << "Order ID: " << id << ", Customer ID: " << customerId << ", Distance: " << distance
        << ", Status: ";

    switch (status) {
        case OrderStatus::PENDING: res << "Pending"; break;
        case OrderStatus::COLLECTING: res << "Collecting"; break;
        case OrderStatus::DELIVERING: res << "Delivering"; break;
        case OrderStatus::COMPLETED: res << "Completed"; break;
    }

    if (collectorId != NO_VOLUNTEER) {
        res << ", Collector ID: " << collectorId;
    }

    if (driverId != NO_VOLUNTEER) {
        res << ", Driver ID: " << driverId;
    }

    return res.str();
}
int Order::getDistance() const {
    return distance;
}

int Order::getOrderId() const {
    return id;
}

std::string Order::orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING:
            return "PENDING";
        case OrderStatus::COLLECTING:
            return "COLLECTING";
        case OrderStatus::DELIVERING:
            return "DELIVERING";
        case OrderStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "Unknown Status";
    }
}

Order* Order::clone() const{
    return new Order(*this);
}


