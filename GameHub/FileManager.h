#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Config.h"
#include "Exceptions.h"
#include "Logger.h"

class User;
class Product;
class Sale;
class Game;
class Admin;
class Customer;

class FileManager 
{
private:
    Logger* logger;

public:
    FileManager();

    template<typename T>
    bool saveToFile(const std::string& filename, const std::vector<T>& data)
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

            for (const auto& item : data) 
            {
                item.serialize(file);
            }

            file.close();
            if (logger) 
            {
                logger->log(LogLevel::INFO, "Data saved to " + filename + ", items: " + std::to_string(size));
            }
            return true;
        }
        catch (const std::exception& e) 
        {
            if (logger) 
            {
                logger->log(LogLevel::ERR, "Failed to save data to " + filename + ": " + e.what());
            }
            return false;
        }
    }

    template<typename T>
    std::vector<T> loadFromFile(const std::string& filename)
    {
        std::vector<T> data;
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
                data[i].deserialize(file);
            }

            file.close();
            if (logger) 
            {
                logger->log(LogLevel::INFO, "Data loaded from " + filename + ", items: " + std::to_string(size));
            }
        }
        catch (const std::exception& e)
        {
            if (logger)
            {
                logger->log(LogLevel::ERR, "Failed to load data from " + filename + ": " + e.what());
            }
        }
        return data;
    }

    template<typename T>
    bool appendToFile(const std::string& filename, const T& item) 
    {
        auto data = loadFromFile<T>(filename);
        data.push_back(item);
        return saveToFile(filename, data);
    }

    std::vector<std::string> loadStringList(const std::string& filename);
    bool saveStringList(const std::string& filename, const std::vector<std::string>& list);
    bool appendToStringList(const std::string& filename, const std::string& item);
};

template<>
bool FileManager::saveToFile<std::string>(const std::string& filename, const std::vector<std::string>& data);

template<>
std::vector<std::string> FileManager::loadFromFile<std::string>(const std::string& filename);

template<>
bool FileManager::saveToFile<std::shared_ptr<User>>(const std::string& filename, const std::vector<std::shared_ptr<User>>& data);

template<>
std::vector<std::shared_ptr<User>> FileManager::loadFromFile<std::shared_ptr<User>>(const std::string& filename);

template<>
bool FileManager::saveToFile<std::shared_ptr<Product>>(const std::string& filename, const std::vector<std::shared_ptr<Product>>& data);

template<>
std::vector<std::shared_ptr<Product>> FileManager::loadFromFile<std::shared_ptr<Product>>(const std::string& filename);