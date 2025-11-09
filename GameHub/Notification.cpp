#include "Notification.h"

Notification::Notification(const std::string& msg, const std::string& notifType)
    : message(msg), type(notifType), isRead(false) 
{
    timestamp = Config::getCurrentDateTime();
}

std::string Notification::getMessage() const { return message; }
std::string Notification::getType() const { return type; }
std::string Notification::getTimestamp() const { return timestamp; }
bool Notification::getIsRead() const { return isRead; }

void Notification::markAsRead() { isRead = true; }

void Notification::display() const 
{
    int color = Config::COLOR_DEFAULT;
    if (type == "warning") color = Config::COLOR_WARNING;
    else if (type == "error") color = Config::COLOR_ERROR;
    else if (type == "success") color = Config::COLOR_SUCCESS;

    Config::setColor(color);
    std::cout << "[" << timestamp << "] " << message;
    if (!isRead) 
    {
        std::cout << " [НОВОЕ]";
    }
    std::cout << std::endl;
    Config::resetColor();
}

NotificationSystem::NotificationSystem() 
{
    logger = Logger::getInstance();
}

void NotificationSystem::notifyAdmin(const std::string& message, const std::string& type) 
{
    adminNotifications.emplace_back(message, type);
    logger->log(LogLevel::INFO, "Admin notification: " + message);
}

void NotificationSystem::notifyLowStock(int productId, const std::string& productName) 
{
    std::string msg = "Закончились ключи для товара: " + productName + " (ID: " + std::to_string(productId) + ")";
    notifyAdmin(msg, "warning");
}

void NotificationSystem::notifyNewRegistration(const std::string& username) 
{
    std::string msg = "Новая заявка на регистрацию: " + username;
    notifyAdmin(msg, "info");
}

void NotificationSystem::notifyCustomer(int customerId, const std::string& message, const std::string& type) 
{
    customerNotificationsMap[customerId].emplace_back(message, type); 
    logger->log(LogLevel::INFO, "Customer notification (ID: " + std::to_string(customerId) + "): " + message);
}

void NotificationSystem::notifyWishlistDiscount(int customerId, const std::string& productName) 
{
    std::string msg = "Игра из вашего списка желаемого '" + productName + "' продаётся со скидкой!";
    notifyCustomer(customerId, msg, "success");
}

void NotificationSystem::notifyPurchaseComplete(int customerId, const std::string& productName) 
{
    std::string msg = "Заказ завершён, ключ для '" + productName + "' отправлен на email";
    notifyCustomer(customerId, msg, "success");
}

void NotificationSystem::notifyWishlistBackInStock(int customerId, const std::string& productName) 
{
    std::string msg = "Ключи к игре '" + productName + "' из вашего списка желаемого теперь в наличии!";
    notifyCustomer(customerId, msg, "info");
}

std::vector<Notification> NotificationSystem::getAdminNotifications() 
{
    return adminNotifications;
}

std::vector<Notification> NotificationSystem::getCustomerNotifications(int customerId) 
{
    auto it = customerNotificationsMap.find(customerId);
    if (it != customerNotificationsMap.end()) 
    {
        return it->second;
    }
    return std::vector<Notification>(); 
}

void NotificationSystem::markAdminNotificationsAsRead() 
{
    for (auto& notification : adminNotifications) 
    {
        notification.markAsRead();
    }
}

void NotificationSystem::markCustomerNotificationsAsRead(int customerId) 
{
    auto it = customerNotificationsMap.find(customerId);
    if (it != customerNotificationsMap.end()) 
    {
        for (auto& notification : it->second) 
        {
            notification.markAsRead();
        }
    }
}

void NotificationSystem::displayAdminNotifications()
{
    if (adminNotifications.empty())
    {
        Config::setColor(Config::COLOR_DEFAULT);
        std::cout << "Нет новых уведомлений" << std::endl;
        Config::resetColor();
        return;
    }

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== УВЕДОМЛЕНИЯ АДМИНИСТРАТОРА ===" << std::endl;
    Config::resetColor();

    for (const auto& notification : adminNotifications) 
    {
        notification.display();
    }
}

void NotificationSystem::displayCustomerNotifications(int customerId) 
{
    auto notifications = getCustomerNotifications(customerId);
    if (notifications.empty()) 
    {
        Config::setColor(Config::COLOR_DEFAULT);
        std::cout << "Нет новых уведомлений" << std::endl;
        Config::resetColor();
        return;
    }

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ВАШИ УВЕДОМЛЕНИЯ ===" << std::endl;
    Config::resetColor();

    for (const auto& notification : notifications) 
    {
        notification.display();
    }
}

void NotificationSystem::clearOldNotifications()
{
    adminNotifications.clear();
    customerNotificationsMap.clear();
}

void NotificationSystem::clearAdminNotifications() 
{
    adminNotifications.clear();
    logger->log(LogLevel::INFO, "Уведомления админа очищены");
}

void NotificationSystem::clearCustomerNotifications(int customerId) 
{
    auto it = customerNotificationsMap.find(customerId);
    if (it != customerNotificationsMap.end()) 
    {
        it->second.clear();
        logger->log(LogLevel::INFO, "Уведомления очищены для пользователей: " + std::to_string(customerId));
    }
}