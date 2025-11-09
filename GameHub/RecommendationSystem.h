#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "Product.h"
#include "User.h"
#include "Store.h"
#include "Logger.h"

class RecommendationSystem 
{
private:
    Store& store;
    Logger* logger;

public:
    RecommendationSystem(Store& storeRef);

    std::vector<std::shared_ptr<Product>> getSimilarGames(std::shared_ptr<Customer> customer);
    std::vector<std::shared_ptr<Product>> getDiscountedProducts();
    std::vector<std::shared_ptr<Product>> getPopularProducts();
};