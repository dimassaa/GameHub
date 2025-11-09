#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Config.h"
#include "Logger.h"

class Notification 
{
private:
    std::string message;
    std::string type;
    std::string timestamp;
    bool isRead;

public:
    Notification(const std::string& msg, const std::string& notifType = "info");

    std::string getMessage() const;
    std::string getType() const;
    std::string getTimestamp() const;
    bool getIsRead() const;

    void markAsRead();
    void display() const;
};

class NotificationSystem
{
private:
    std::vector<Notification> adminNotifications;
    std::map<int, std::vector<Notification>> customerNotificationsMap;
    Logger* logger;

public:
    NotificationSystem();

    void notifyAdmin(const std::string& message, const std::string& type = "info");
    void notifyLowStock(int productId, const std::string& productName);
    void notifyNewRegistration(const std::string& username);
    void clearAdminNotifications();
    void clearCustomerNotifications(int customerId);

    void notifyCustomer(int customerId, const std::string& message, const std::string& type = "info");
    void notifyWishlistDiscount(int customerId, const std::string& productName);
    void notifyPurchaseComplete(int customerId, const std::string& productName);
    void notifyWishlistBackInStock(int customerId, const std::string& productName);

    std::vector<Notification> getAdminNotifications();
    std::vector<Notification> getCustomerNotifications(int customerId);

    void markAdminNotificationsAsRead();
    void markCustomerNotificationsAsRead(int customerId);

    void displayAdminNotifications();
    void displayCustomerNotifications(int customerId);

    void clearOldNotifications();
};
