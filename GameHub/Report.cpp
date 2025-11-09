#include "Report.h"

Report::Report(Store& storeRef) : store(storeRef) 
{
    logger = Logger::getInstance();
}

Report::SalesReport Report::generateSalesReport(const std::string& startDate, const std::string& endDate) 
{
    SalesReport report;
    report.period = startDate + " - " + endDate;
    report.totalRevenue = 0.0;
    report.totalSales = 0;

    std::map<int, int> productSales;

    auto allSales = store.getAllSales();
    for (const auto& sale : allSales) 
    {
        if (sale.getSaleDateTime() >= startDate && sale.getSaleDateTime() <= endDate) 
        {
            report.totalRevenue += sale.getFinalAmount();
            report.totalSales++;

            for (const auto& item : sale.getItems()) 
            {
                productSales[item.first] += item.second;
            }
        }
    }

    std::vector<std::pair<int, int>> productVector(productSales.begin(), productSales.end());
    std::sort(productVector.begin(), productVector.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) 
        {
            return a.second > b.second;
        });

    for (size_t i = 0; i < (std::min)(productVector.size(), size_t(5)); ++i) 
    {
        report.topProducts[productVector[i].first] = productVector[i].second;
    }

    logger->log(LogLevel::INFO, "Sales report generated: " + report.period +
        ", Revenue: " + std::to_string(report.totalRevenue));

    return report;
}

Report::FinancialReport Report::generateFinancialReport(const std::string& startDate, const std::string& endDate) 
{
    FinancialReport report;
    report.period = startDate + " - " + endDate;
    report.totalRevenue = 0.0;
    report.totalDiscounts = 0.0;
    report.totalTransactions = 0;

    auto allSales = store.getAllSales();
    for (const auto& sale : allSales) 
    {
        if (sale.getSaleDateTime() >= startDate && sale.getSaleDateTime() <= endDate) 
        {
            report.totalRevenue += sale.getFinalAmount();
            report.totalDiscounts += sale.getDiscountAmount();
            report.totalTransactions++;
        }
    }

    report.averageTransactionValue = report.totalTransactions > 0 ?
        report.totalRevenue / report.totalTransactions : 0.0;

    logger->log(LogLevel::INFO, "Financial report generated: " + report.period +
        ", Revenue: " + std::to_string(report.totalRevenue));

    return report;
}

void Report::saveReportToFile(const SalesReport& report, const std::string& filename) 
{
    std::ofstream file(filename);
    if (!file.is_open()) 
    {
        throw FileIOException("Невозможно открыть файл для записи: " + filename);
    }

    file << "ОТЧЕТ О ПРОДАЖАХ" << std::endl;
    file << "Период: " << report.period << std::endl;
    file << "Общая выручка: " << report.totalRevenue << " руб." << std::endl;
    file << "Количество продаж: " << report.totalSales << std::endl;
    file << std::endl << "ТОП-5 ТОВАРОВ:" << std::endl;

    int rank = 1;
    for (const auto& productEntry : report.topProducts) 
    {
        auto product = store.getProductById(productEntry.first);
        if (product) 
        {
            file << rank << ". " << product->getTitle() << " - " << productEntry.second << " шт." << std::endl;
        }
        rank++;
    }

    file.close();
    logger->log(LogLevel::INFO, "Sales report saved to: " + filename);
}

void Report::saveReportToFile(const FinancialReport& report, const std::string& filename) 
{
    std::ofstream file(filename);
    if (!file.is_open()) 
    {
        throw FileIOException("Cannot open file for writing: " + filename);
    }

    file << "ФИНАНСОВЫЙ ОТЧЕТ" << std::endl;
    file << "Период: " << report.period << std::endl;
    file << "Общая выручка: " << report.totalRevenue << " руб." << std::endl;
    file << "Сумма предоставленных скидок: " << report.totalDiscounts << " руб." << std::endl;
    file << "Количество транзакций: " << report.totalTransactions << std::endl;
    file << "Средний чек: " << report.averageTransactionValue << " руб." << std::endl;

    file.close();
    logger->log(LogLevel::INFO, "Financial report saved to: " + filename);
}

void Report::loadReportInConsole(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        throw FileIOException("Cannot open file: " + filename);
    }

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== СОДЕРЖИМОЕ ОТЧЕТА ===" << std::endl;
    Config::resetColor();

    std::string line;
    while (std::getline(file, line)) 
    {
        std::cout << line << std::endl;
    }
    file.close();
}

void Report::generateInventoryReport() 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ОТЧЕТ ПО ОСТАТКАМ КЛЮЧЕЙ ===" << std::endl;
    Config::resetColor();

    auto products = store.getAllProducts();
    int lowStockCount = 0;

    for (const auto& product : products) 
    {
        int keys = product->getActivationKeys();
        std::cout << product->getTitle() << ": " << keys << " ключей" << std::endl;

        if (keys < 5) 
        {
            Config::setColor(Config::COLOR_WARNING);
            std::cout << "МАЛО КЛЮЧЕЙ!" << std::endl;
            Config::resetColor();
            lowStockCount++;
        }
    }

    if (lowStockCount > 0) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Всего товаров с малым запасом: " << lowStockCount << std::endl;
        Config::resetColor();
    }
}