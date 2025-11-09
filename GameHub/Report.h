#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "Sale.h"
#include "Product.h"
#include "Store.h"
#include "FileManager.h"
#include "Logger.h"
#include "Exceptions.h"

class Report 
{
private:
    Store& store;
    FileManager fileManager;
    Logger* logger;

public:
    Report(Store& storeRef);

    struct SalesReport 
    {
        std::string period;
        double totalRevenue;
        int totalSales;
        std::map<int, int> topProducts;
    };

    struct FinancialReport 
    {
        std::string period;
        double totalRevenue;
        double totalDiscounts;
        int totalTransactions;
        double averageTransactionValue;
    };

    SalesReport generateSalesReport(const std::string& startDate, const std::string& endDate);
    FinancialReport generateFinancialReport(const std::string& startDate, const std::string& endDate);

    void saveReportToFile(const SalesReport& report, const std::string& filename);
    void saveReportToFile(const FinancialReport& report, const std::string& filename);

    void loadReportInConsole(const std::string& filename);
    void generateInventoryReport();
};
