#include "RecommendationSystem.h"

RecommendationSystem::RecommendationSystem(Store& storeRef) : store(storeRef) 
{
    logger = Logger::getInstance();
}

std::vector<std::shared_ptr<Product>> RecommendationSystem::getSimilarGames(std::shared_ptr<Customer> customer) 
{
    std::vector<std::shared_ptr<Product>> recommendations;

    if (customer->getLastPurchasedProductId() == -1) 
    {
        return recommendations;
    }

    auto lastPurchasedProduct = store.getProductById(customer->getLastPurchasedProductId());
    if (!lastPurchasedProduct) 
    {
        logger->log(LogLevel::WARNING, "Last purchased product not found for customer: " + customer->getUsername());
        return recommendations;
    }

    std::string targetGenre = lastPurchasedProduct->getGenre();
    std::vector<std::string> targetTags = lastPurchasedProduct->getTags();

    auto allProducts = store.getAllProducts();
    for (const auto& product : allProducts) 
    {
        if (product->getId() == customer->getLastPurchasedProductId()) 
        {
            continue;
        }

        if (product->getGenre() == targetGenre) 
        {
            recommendations.push_back(product);
        }
        else
        {
            for (const auto& tag : targetTags) 
            {
                if (product->hasTag(tag))
                {
                    recommendations.push_back(product);
                    break;
                }
            }
        }
    }

    std::sort(recommendations.begin(), recommendations.end(),
        [this](const std::shared_ptr<Product>& a, const std::shared_ptr<Product>& b) 
        {
            double discountA = (a->getBasePrice() - store.getDiscountedPrice(a)) / a->getBasePrice();
            double discountB = (b->getBasePrice() - store.getDiscountedPrice(b)) / b->getBasePrice();
            return discountA > discountB;
        });

    if (recommendations.size() > 5) 
    {
        recommendations.resize(5);
    }

    logger->log(LogLevel::INFO, "Generated " + std::to_string(recommendations.size()) +
        " recommendations for customer: " + customer->getUsername());

    return recommendations;
}

std::vector<std::shared_ptr<Product>> RecommendationSystem::getDiscountedProducts() 
{
    auto allProducts = store.getAllProducts();
    std::vector<std::shared_ptr<Product>> discounted;

    for (const auto& product : allProducts) 
    {
        if (store.getDiscountedPrice(product) < product->getBasePrice())
        {
            discounted.push_back(product);
        }
    }

    std::sort(discounted.begin(), discounted.end(),
        [this](const std::shared_ptr<Product>& a, const std::shared_ptr<Product>& b) 
        {
            double discountA = (a->getBasePrice() - store.getDiscountedPrice(a)) / a->getBasePrice();
            double discountB = (b->getBasePrice() - store.getDiscountedPrice(b)) / b->getBasePrice();
            return discountA > discountB;
        });

    return discounted;
}

std::vector<std::shared_ptr<Product>> RecommendationSystem::getPopularProducts() 
{
    return getDiscountedProducts();
}