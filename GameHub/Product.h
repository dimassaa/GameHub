#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Config.h"
#include "Exceptions.h"

class Product 
{
protected:
    int id;
    std::string title;
    double basePrice;
    std::string developer;
    std::string publisher;
    std::string genre;
    int ageRating;
    std::string description;
    std::string releaseDate;
    double averageRating;
    std::vector<std::string> tags;
    int activationKeys;

public:
    Product();
    Product(int id, const std::string& title, double basePrice, const std::string& developer,
        const std::string& publisher, const std::string& genre, int ageRating,
        const std::string& description, const std::string& releaseDate);
    virtual ~Product() = default;

    int getId() const;
    std::string getTitle() const;
    double getBasePrice() const;
    std::string getDeveloper() const;
    std::string getPublisher() const;
    std::string getGenre() const;
    int getAgeRating() const;
    std::string getDescription() const;
    std::string getReleaseDate() const;
    double getAverageRating() const;
    std::vector<std::string> getTags() const;
    int getActivationKeys() const;

    void setTitle(const std::string& newTitle);
    void setBasePrice(double price);
    void setDeveloper(const std::string& dev);
    void setPublisher(const std::string& pub);
    void setGenre(const std::string& newGenre);
    void setAgeRating(int rating);
    void setDescription(const std::string& desc);
    void setAverageRating(double rating);
    void setTags(const std::vector<std::string>& newTags);
    void setActivationKeys(int keys);

    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;

    bool hasKeys() const;
    bool reserveKeys(int count);
    void addKeys(int count);

    virtual void displayInfo() const = 0;
    virtual std::string getProductType() const = 0;

    virtual void serialize(std::ostream& os) const;
    virtual void deserialize(std::istream& is);

    std::string getShortTitle() const;
    std::string getShortDescription() const;
};

class Game : public Product 
{
public:
    Game();
    Game(int id, const std::string& title, double basePrice, const std::string& developer,
        const std::string& publisher, const std::string& genre, int ageRating,
        const std::string& description, const std::string& releaseDate);

    void displayInfo() const override;
    std::string getProductType() const override;
};
