#include "Wishlist.h"
#include "FileManager.h"

Wishlist::Wishlist(FileManager& fm) : fileManager(fm) 
{
    logger = Logger::getInstance();
    loadWishlist();
}

void Wishlist::WishlistItem::serialize(std::ostream& os) const 
{
    os.write(reinterpret_cast<const char*>(&customerId), sizeof(customerId));
    os.write(reinterpret_cast<const char*>(&productId), sizeof(productId));

    size_t size = addedDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(addedDate.c_str(), size);
}

void Wishlist::WishlistItem::deserialize(std::istream& is) 
{
    is.read(reinterpret_cast<char*>(&customerId), sizeof(customerId));
    is.read(reinterpret_cast<char*>(&productId), sizeof(productId));

    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    addedDate.resize(size);
    is.read(&addedDate[0], size);
}

bool Wishlist::addToWishlist(int customerId, int productId) 
{
    for (const auto& item : wishlistItems) 
    {
        if (item.customerId == customerId && item.productId == productId) 
        {
            return false;
        }
    }

    WishlistItem newItem(customerId, productId, Config::getCurrentDateTime());
    wishlistItems.push_back(newItem);
    saveWishlist();

    logger->log(LogLevel::INFO, "Product added to wishlist - Customer: " +
        std::to_string(customerId) + ", Product: " + std::to_string(productId));
    return true;
}

bool Wishlist::removeFromWishlist(int customerId, int productId) 
{
    auto it = std::remove_if(wishlistItems.begin(), wishlistItems.end(),
        [customerId, productId](const WishlistItem& item)
        {
            return item.customerId == customerId && item.productId == productId;
        });

    if (it != wishlistItems.end()) 
    {
        wishlistItems.erase(it, wishlistItems.end());
        saveWishlist();

        logger->log(LogLevel::INFO, "Product removed from wishlist - Customer: " +
            std::to_string(customerId) + ", Product: " + std::to_string(productId));
        return true;
    }
    return false;
}

std::vector<int> Wishlist::getWishlistProducts(int customerId) 
{
    std::vector<int> productIds;
    for (const auto& item : wishlistItems) 
    {
        if (item.customerId == customerId) 
        {
            productIds.push_back(item.productId);
        }
    }
    return productIds;
}

bool Wishlist::isInWishlist(int customerId, int productId) 
{
    for (const auto& item : wishlistItems) 
    {
        if (item.customerId == customerId && item.productId == productId) 
        {
            return true;
        }
    }
    return false;
}

void Wishlist::clearWishlist(int customerId) 
{
    auto it = std::remove_if(wishlistItems.begin(), wishlistItems.end(),
        [customerId](const WishlistItem& item) 
        {
            return item.customerId == customerId;
        });

    if (it != wishlistItems.end()) 
    {
        wishlistItems.erase(it, wishlistItems.end());
        saveWishlist();
        logger->log(LogLevel::INFO, "Wishlist cleared for customer: " + std::to_string(customerId));
    }
}

int Wishlist::getWishlistCount(int customerId) 
{
    int count = 0;
    for (const auto& item : wishlistItems) 
    {
        if (item.customerId == customerId) 
        {
            count++;
        }
    }
    return count;
}

void Wishlist::displayWishlist(int customerId, const std::vector<std::shared_ptr<Product>>& allProducts) 
{
    auto productIds = getWishlistProducts(customerId);
    if (productIds.empty()) 
    {
        TableFormatter::displayWarningMessage("Список желаемого пуст");
        return;
    }

    std::vector<std::shared_ptr<Product>> wishlistProducts;
    for (int productId : productIds) 
    {
        for (const auto& product : allProducts) 
        {
            if (product->getId() == productId) 
            {
                wishlistProducts.push_back(product);
                break;
            }
        }
    }

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ВАШ СПИСОК ЖЕЛАЕМОГО ===" << std::endl;
    Config::resetColor();

    std::vector<int> columnWidths = { 6, 30, 15, 20, 12, 15 };

    TableFormatter::printHorizontalLine(columnWidths);
    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Название"
        << " | " << std::setw(columnWidths[2]) << std::left << "Цена"
        << " | " << std::setw(columnWidths[3]) << std::left << "Разработчик"
        << " | " << std::setw(columnWidths[4]) << std::left << "Жанр"
        << " | " << std::setw(columnWidths[5]) << std::left << "В наличии"
        << " |" << std::endl;
    TableFormatter::printHorizontalLine(columnWidths);

    for (const auto& product : wishlistProducts) 
    {
        std::string inStock = product->hasKeys() ? "Да" : "Нет";
        if (!product->hasKeys()) 
        {
            Config::setColor(Config::COLOR_WARNING);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << product->getId()
            << " | " << std::setw(columnWidths[1]) << std::left << TableFormatter::truncateString(product->getTitle(), columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::right << std::fixed << std::setprecision(2) << product->getBasePrice()
            << " | " << std::setw(columnWidths[3]) << std::left << TableFormatter::truncateString(product->getDeveloper(), columnWidths[3])
            << " | " << std::setw(columnWidths[4]) << std::left << TableFormatter::truncateString(product->getGenre(), columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::left << inStock
            << " |" << std::endl;

        Config::resetColor();
    }

    TableFormatter::printHorizontalLine(columnWidths);
    std::cout << "Всего в списке желаемого: " << wishlistProducts.size() << std::endl;
}

void Wishlist::loadWishlist() 
{
    wishlistItems = fileManager.loadFromFile<WishlistItem>(Config::WISHLISTS_FILE);
}

void Wishlist::saveWishlist() 
{
    fileManager.saveToFile(Config::WISHLISTS_FILE, wishlistItems);
}

std::vector<Wishlist::WishlistItem> Wishlist::getWishlistProductsForAllUsers() 
{
    return wishlistItems;
}