#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "Config.h"
#include "Exceptions.h"

class Sale 
{
private:
    int saleId;
    int customerId;
    std::string saleDateTime;
    std::vector<std::pair<int, int>> items;
    double totalAmount;
    double discountAmount;
    std::string status;

public:
    Sale();
    Sale(int saleId, int customerId, const std::vector<std::pair<int, int>>& items,
        double totalAmount, double discountAmount, const std::string& status = "completed");

    int getSaleId() const;
    int getCustomerId() const;
    std::string getSaleDateTime() const;
    std::vector<std::pair<int, int>> getItems() const;
    double getTotalAmount() const;
    double getDiscountAmount() const;
    std::string getStatus() const;
    double getFinalAmount() const;

    void setSaleId(int id);
    void setCustomerId(int id);
    void setSaleDateTime(const std::string& datetime);
    void setItems(const std::vector<std::pair<int, int>>& newItems);
    void setTotalAmount(double amount);
    void setDiscountAmount(double amount);
    void setStatus(const std::string& newStatus);

    void addItem(int productId, int quantity);
    bool removeItem(int productId);
    int getItemQuantity(int productId) const;

    void displaySaleInfo() const;

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);

    bool containsProduct(int productId) const;
    int getTotalItemsCount() const;
};
