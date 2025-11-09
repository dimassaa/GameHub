#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "Product.h"
#include "User.h"
#include "Logger.h"
#include "Exceptions.h"
#include "TableFormatter.h"

class FileManager;

class Wishlist 
{
public:
    struct WishlistItem 
    {
        int customerId;
        int productId;
        std::string addedDate;

        WishlistItem() : customerId(0), productId(0) {}

        WishlistItem(int custId, int prodId, const std::string& date)
            : customerId(custId), productId(prodId), addedDate(date) {
        }

        void serialize(std::ostream& os) const;
        void deserialize(std::istream& is);
    };

private:

    std::vector<WishlistItem> wishlistItems;
    FileManager& fileManager;
    Logger* logger;

public:
    explicit Wishlist(FileManager& fm);

    bool addToWishlist(int customerId, int productId);
    bool removeFromWishlist(int customerId, int productId);
    std::vector<int> getWishlistProducts(int customerId);
    std::vector<WishlistItem> getWishlistProductsForAllUsers();
    bool isInWishlist(int customerId, int productId);
    void clearWishlist(int customerId);
    int getWishlistCount(int customerId);
    void displayWishlist(int customerId, const std::vector<std::shared_ptr<Product>>& allProducts);

private:
    void loadWishlist();
    void saveWishlist();
};
