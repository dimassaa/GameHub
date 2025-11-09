#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "User.h"
#include "Product.h"
#include "Store.h"
#include "Wishlist.h"
#include "RecommendationSystem.h"
#include "Report.h"
#include "Notification.h"
#include "InputValidation.h"
#include "TableFormatter.h"
#include "Logger.h"
#include "Exceptions.h"

class Menu 
{
protected:
    Store& store;
    Wishlist& wishlist;
    RecommendationSystem& recommendationSystem;
    Report& report;
    NotificationSystem& notificationSystem;
    std::shared_ptr<User> currentUser;
    Logger* logger;

public:
    Menu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns);
    virtual void show() = 0;
    virtual ~Menu() = default;

protected:
    void displayHeader(const std::string& title);
};

class MainMenu : public Menu 
{
public:
    MainMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns);
    void show() override;

private:
    bool login();
    void registerUser();
    void recoverPassword();
    void showUserMenu();
    void aboutSystem();
};

class AdminMenu : public Menu 
{
public:
    AdminMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns, std::shared_ptr<User> user);
    void show() override;

private:
    void manageUsers();
    void manageProducts();
    void manageDiscounts();
    void viewReports();
    void viewLogs();
    void viewNotifications();
    void approveUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager);
    void blockUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager);
    void unblockUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager);
    void changeUserRole(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager);
    void searchAndFilterUsers();
    std::vector<std::shared_ptr<User>> filterUsers(const std::vector<std::shared_ptr<User>>& users, const std::string& searchType, const std::string& searchValue);
    void clearUnapprovedUsers(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager);
    void changeMyProfile();
    void changeAdminEmail();
    void changeAdminPhone();
    void changeAdminPassword();
    void changeAdminUsername();
    void addNewProduct();
    void editProduct();
    void deleteProduct();
    void addKeysToProduct();
    void addNewGenre();
    void deleteGenre();
    void addNewTag();
    void deleteTag();
    void searchAndFilterProducts();
    std::vector<std::shared_ptr<Product>> filterProducts(const std::vector<std::shared_ptr<Product>>& products, const std::string& searchType, const std::string& searchValue);
    bool confirmAction(const std::string& message);
    void addNewDiscount();
    void editDiscount();
    void deleteDiscount();
    void viewDiscountedProducts();
    void selectProductsForDiscount(std::vector<std::string>& targets);
    void selectGenresForDiscount(std::vector<std::string>& targets);
    void selectTagsForDiscount(std::vector<std::string>& targets);
    void selectDevelopersForDiscount(std::vector<std::string>& targets);
};

class CustomerMenu : public Menu 
{
public:
    CustomerMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns, std::shared_ptr<User> user);
    void show() override;

private:
    void viewCatalog();
    void searchGames();
    void viewPurchaseHistory();
    void manageWishlist();
    void viewRecommendations();
    void viewProfile();
    void viewNotifications();
    void changeEmail();
    void changePhone();
    void changePassword();
    void viewProductDetails();
    void addToWishlistFromCatalog();
    void purchaseFromCatalog();
    void purchaseProduct(std::shared_ptr<Product> product);
    void viewProductDetailsFromSearch(const std::vector<std::shared_ptr<Product>>& searchResults);
    void addToWishlistFromSearch();
    void purchaseFromSearch(const std::vector<std::shared_ptr<Product>>& searchResults);
    void showLimitedMenu();
    bool confirmAction(const std::string& message);
    void viewCatalogLimited();
    void searchGamesLimited();
    void viewProductDetailsLimited();
    void viewSaleDetails(const std::vector<Sale>& userSales);
    void addToWishlistFromRecommendations(const std::vector<std::shared_ptr<Product>>& recommendations);
    void purchaseFromRecommendations(const std::vector<std::shared_ptr<Product>>& recommendations);
};