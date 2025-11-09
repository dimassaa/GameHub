#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <iomanip>
#include <algorithm>
#include "Config.h"
#include "Product.h"
#include "User.h"
#include "Sale.h"
#include "Store.h"
#include "Report.h"

class TableFormatter 
{
public: 
    static std::string truncateString(const std::string& str, size_t maxLength);
    static void printHorizontalLine(const std::vector<int>& widths);

    static void displayProductsTable(const std::vector<std::shared_ptr<Product>>& products, int page = 1);
    static void displayProductsTableWithPagination(const std::vector<std::shared_ptr<Product>>& products, int page, int totalPages, int totalProducts);
    static void displayUsersTable(const std::vector<std::shared_ptr<User>>& users, int page = 1);
    static void displayUsersTableWithPagination(const std::vector<std::shared_ptr<User>>& users, int page, int totalPages, int totalUsers);
    static void displaySalesTable(const std::vector<Sale>& sales, int page = 1);
    static void displayMenu(const std::vector<std::string>& options, const std::string& title = "лемч");
    static void displayDiscountsTable(const std::vector<Discount>& discounts, int page = 1);
    static void displayDiscountsTableWithPagination(const std::vector<Discount>& discounts, int page, int totalPages, int totalDiscounts);
    static void displayDiscountedProducts(const std::vector<std::shared_ptr<Product>>& products, const std::vector<double>& discountedPrices);
    static void displayLogsTable(const std::vector<std::string>& logs, int page, int totalPages, int totalLogs);
    static void displayReportMenu(const std::vector<std::string>& options, const std::string& title = "нрверш");
    static void displaySalesReport(const Report::SalesReport& report, Store& store);
    static void displayFinancialReport(const Report::FinancialReport& report);
    static void displaySalesTableWithPagination(const std::vector<Sale>& sales, int page, int totalPages, int totalSales);
    static void displaySalesTableDetailed(const std::vector<Sale>& sales, Store& store, int customerId = -1);

    static void displaySuccessMessage(const std::string& message);
    static void displayErrorMessage(const std::string& message);
    static void displayWarningMessage(const std::string& message);

    static void clearScreen();
    static void pause();

    static void displayProductDetails(std::shared_ptr<Product> product);
};