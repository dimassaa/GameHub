#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include "Product.h"
#include "User.h"
#include "Sale.h"
#include "FileManager.h"
#include "Logger.h"
#include "Exceptions.h"

struct Discount 
{
    std::string type;
    std::vector<std::string> targets;
    double percentage;
    std::string startDate;
    std::string endDate;

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);
    bool isValid() const;
};

class Store 
{
private:
    std::vector<std::shared_ptr<Product>> products;
    std::vector<Discount> discounts;
    std::vector<Sale> sales;
    FileManager fileManager;
    Logger* logger;

    int generateProductId();
    int generateSaleId();

public:
    Store();

    void addProduct(std::shared_ptr<Product> product);
    bool updateProduct(int productId, std::shared_ptr<Product> updatedProduct);
    bool deleteProduct(int productId);
    std::shared_ptr<Product> getProductById(int id);
    std::vector<std::shared_ptr<Product>> searchProducts(const std::string& query);
    std::vector<std::shared_ptr<Product>> filterProducts(const std::string& filterType, const std::string& filterValue = "");

    bool addKeys(int productId, int count);

    void addDiscount(const Discount& discount);
    double getDiscountedPrice(std::shared_ptr<Product> product);

    bool processSale(int customerId, const std::vector<std::pair<int, int>>& items, double discountAmount = 0.0);

    std::vector<std::shared_ptr<Product>> getAllProducts() const;
    std::vector<Discount> getAllDiscounts() const;
    std::vector<Sale> getAllSales() const;

    void loadData();
    void saveProducts();
    void saveSales();
    void saveDiscounts();

    std::vector<std::string> getUniqueGenres();
    std::vector<std::string> getUniqueDevelopers();
};