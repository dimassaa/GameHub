#include "FileManager.h"
#include "Store.h"
#include "Wishlist.h"

FileManager::FileManager() 
{
    logger = Logger::getInstance();
}

std::vector<std::string> FileManager::loadStringList(const std::string& filename) 
{
    return loadFromFile<std::string>(filename);
}

bool FileManager::saveStringList(const std::string& filename, const std::vector<std::string>& list) 
{
    return saveToFile<std::string>(filename, list);
}

bool FileManager::appendToStringList(const std::string& filename, const std::string& item) 
{
    auto list = loadStringList(filename);
    list.push_back(item);
    return saveStringList(filename, list);
}

template<>
bool FileManager::saveToFile<std::string>(const std::string& filename, const std::vector<std::string>& data)
{
    try 
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) 
        {
            throw FileIOException("Cannot open file for writing: " + filename);
        }

        size_t size = data.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& str : data)
        {
            size_t strSize = str.size();
            file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
            file.write(str.c_str(), strSize);
        }

        file.close();
        logger->log(LogLevel::INFO, "String list saved to " + filename + ", items: " + std::to_string(size));
        return true;
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to save string list to " + filename + ": " + e.what());
        return false;
    }
}

template<>
std::vector<std::string> FileManager::loadFromFile<std::string>(const std::string& filename) 
{
    std::vector<std::string> data;
    try 
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) 
        {
            return data;
        }

        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        data.resize(size);
        for (size_t i = 0; i < size; ++i) 
        {
            size_t strSize;
            file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));

            std::string str(strSize, ' ');
            file.read(&str[0], strSize);
            data[i] = str;
        }

        file.close();
        logger->log(LogLevel::INFO, "String list loaded from " + filename + ", items: " + std::to_string(size));
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to load string list from " + filename + ": " + e.what());
    }
    return data;
}

template<>
bool FileManager::saveToFile<std::shared_ptr<Product>>(const std::string& filename, const std::vector<std::shared_ptr<Product>>& data) 
{
    try 
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) 
        {
            throw FileIOException("Cannot open file for writing: " + filename);
        }

        size_t size = data.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& product : data) 
        {
            std::string productType = product->getProductType();
            size_t typeSize = productType.size();
            file.write(reinterpret_cast<const char*>(&typeSize), sizeof(typeSize));
            file.write(productType.c_str(), typeSize);

            product->serialize(file);
        }

        file.close();
        logger->log(LogLevel::INFO, "Products saved to " + filename + ", count: " + std::to_string(size));
        return true;
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to save products to " + filename + ": " + e.what());
        return false;
    }
}

template<>
std::vector<std::shared_ptr<Product>> FileManager::loadFromFile<std::shared_ptr<Product>>(const std::string& filename) 
{
    std::vector<std::shared_ptr<Product>> products;
    try 
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return products;
        }

        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        for (size_t i = 0; i < size; ++i) 
        {
            size_t typeSize;
            file.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
            std::string productType(typeSize, ' ');
            file.read(&productType[0], typeSize);

            std::shared_ptr<Product> product;
            if (productType == "Game") 
            {
                product = std::make_shared<Game>();
            }

            product->deserialize(file);
            products.push_back(product);
        }

        file.close();
        logger->log(LogLevel::INFO, "Products loaded from " + filename + ", count: " + std::to_string(size));
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to load products from " + filename + ": " + e.what());
    }
    return products;
}

template<>
bool FileManager::saveToFile<std::shared_ptr<User>>(const std::string& filename, const std::vector<std::shared_ptr<User>>& data) 
{
    try
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) 
        {
            throw FileIOException("Невозможно открыть файл для записи: " + filename);
        }

        size_t size = data.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& user : data) 
        {
            std::string userType = (user->getRole() == "admin") ? "admin" : "customer";
            size_t typeSize = userType.size();
            file.write(reinterpret_cast<const char*>(&typeSize), sizeof(typeSize));
            file.write(userType.c_str(), typeSize);

            user->serialize(file);
        }

        file.close();
        logger->log(LogLevel::INFO, "Users saved to " + filename + ", count: " + std::to_string(size));
        return true;
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to save users to " + filename + ": " + e.what());
        return false;
    }
}

template<>
std::vector<std::shared_ptr<User>> FileManager::loadFromFile<std::shared_ptr<User>>(const std::string& filename) 
{
    std::vector<std::shared_ptr<User>> users;
    try 
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) 
        {
            return users;
        }

        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        for (size_t i = 0; i < size; ++i) 
        {
            size_t typeSize;
            file.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
            std::string userType(typeSize, ' ');
            file.read(&userType[0], typeSize);

            std::shared_ptr<User> user;
            if (userType == "admin") 
            {
                user = std::make_shared<Admin>();
            }
            else 
            {
                user = std::make_shared<Customer>();
            }

            user->deserialize(file);
            users.push_back(user);
        }

        file.close();
        logger->log(LogLevel::INFO, "Users loaded from " + filename + ", count: " + std::to_string(size));
    }
    catch (const std::exception& e) 
    {
        logger->log(LogLevel::ERR, "Failed to load users from " + filename + ": " + e.what());
    }
    return users;
}

template bool FileManager::saveToFile<Sale>(const std::string&, const std::vector<Sale>&);
template std::vector<Sale> FileManager::loadFromFile<Sale>(const std::string&);
template bool FileManager::saveToFile<Discount>(const std::string&, const std::vector<Discount>&);
template std::vector<Discount> FileManager::loadFromFile<Discount>(const std::string&);

template bool FileManager::saveToFile<Wishlist::WishlistItem>(const std::string&, const std::vector<Wishlist::WishlistItem>&);
template std::vector<Wishlist::WishlistItem> FileManager::loadFromFile<Wishlist::WishlistItem>(const std::string&);