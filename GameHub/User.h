#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Config.h"
#include "Exceptions.h"
#include "Logger.h"

class User 
{
protected:
    int id;
    std::string username;
    std::string passwordHash;
    std::string email;
    std::string phone;
    std::string role;
    bool isApproved;
    std::string registrationDate;
    std::string unlockDate;

public:
    User();
    User(int id, const std::string& username, const std::string& passwordHash,
        const std::string& email, const std::string& phone, const std::string& role);
    virtual ~User() = default;

    int getId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getRole() const;
    bool getIsApproved() const;
    std::string getRegistrationDate() const;
    std::string getUnlockDate() const;

    void setUsername(const std::string& newUsername);
    void setPasswordHash(const std::string& newHash);
    void setEmail(const std::string& newEmail);
    void setPhone(const std::string& newPhone);
    void setRole(const std::string& newRole);
    void setIsApproved(bool approved);
    void setUnlockDate(const std::string& date);

    // Виртуальные методы
    virtual void displayProfile() const = 0;
    virtual std::string getUserType() const = 0;

    virtual void serialize(std::ostream& os) const;
    virtual void deserialize(std::istream& is);

    bool isBlocked() const;

};

class Admin : public User 
{
public:
    Admin();
    Admin(int id, const std::string& username, const std::string& passwordHash,
        const std::string& email, const std::string& phone);

    void displayProfile() const override;
    std::string getUserType() const override;
};

class Customer : public User 
{
private:
    double totalSpent;
    std::string birthday;
    int lastPurchasedProductId;

public:
    Customer();
    Customer(int id, const std::string& username, const std::string& passwordHash,
        const std::string& email, const std::string& phone, const std::string& birthday);

    double getTotalSpent() const;
    std::string getBirthday() const;
    int getLastPurchasedProductId() const;

    void setTotalSpent(double spent);
    void setLastPurchasedProductId(int productId);

    void displayProfile() const override;
    std::string getUserType() const override;

    void addPurchase(double amount);

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
};
