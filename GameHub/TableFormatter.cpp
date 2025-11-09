#include "TableFormatter.h"

std::string TableFormatter::truncateString(const std::string& str, size_t maxLength) 
{
    if (str.length() <= maxLength) return str;
    return str.substr(0, maxLength - 3) + "...";
}

void TableFormatter::printHorizontalLine(const std::vector<int>& widths) 
{
    std::cout << "+";
    for (int width : widths) 
    {
        std::cout << std::string(width + 2, '-') << "+";
    }
    std::cout << std::endl;
}

void TableFormatter::displayProductsTable(const std::vector<std::shared_ptr<Product>>& products, int page) 
{
    if (products.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет товаров для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 6, 30, 10, 20, 15, 8, 12 };
    int itemsPerPage = Config::PAGE_SIZE;
    int totalPages = (products.size() + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)products.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== КАТАЛОГ ИГР (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Название"
        << " | " << std::setw(columnWidths[2]) << std::left << "Цена"
        << " | " << std::setw(columnWidths[3]) << std::left << "Разработчик"
        << " | " << std::setw(columnWidths[4]) << std::left << "Жанр"
        << " | " << std::setw(columnWidths[5]) << std::left << "Рейтинг"
        << " | " << std::setw(columnWidths[6]) << std::left << "В наличии"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        auto product = products[i];

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << product->getId()
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(product->getTitle(), columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::right << std::fixed << std::setprecision(2) << product->getBasePrice()
            << " | " << std::setw(columnWidths[3]) << std::left << truncateString(product->getDeveloper(), columnWidths[3])
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(product->getGenre(), columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::right << product->getAverageRating()
            << " | " << std::setw(columnWidths[6]) << std::right << product->getActivationKeys()
            << " |" << std::endl;
    }

    printHorizontalLine(columnWidths);
    std::cout << "Всего товаров: " << products.size() << std::endl;
}

void TableFormatter::displayUsersTable(const std::vector<std::shared_ptr<User>>& users, int page) 
{
    if (users.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет пользователей для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 6, 20, 25, 15, 12, 15 };
    int itemsPerPage = Config::PAGE_SIZE;
    int totalPages = (users.size() + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)users.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ПОЛЬЗОВАТЕЛИ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Имя"
        << " | " << std::setw(columnWidths[2]) << std::left << "Email"
        << " | " << std::setw(columnWidths[3]) << std::left << "Телефон"
        << " | " << std::setw(columnWidths[4]) << std::left << "Роль"
        << " | " << std::setw(columnWidths[5]) << std::left << "Статус"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++)
    {
        auto user = users[i];
        std::string status = user->getIsApproved() ? "Одобрен" : "Ожидание";
        if (user->isBlocked()) status = "Заблокирован";

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << user->getId()
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(user->getUsername(), columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(user->getEmail(), columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::left << truncateString(user->getPhone(), columnWidths[3])
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(user->getRole(), columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::left << status
            << " |" << std::endl;
    }

    printHorizontalLine(columnWidths);
    std::cout << "Всего пользователей: " << users.size() << std::endl;
}

void TableFormatter::displaySalesTable(const std::vector<Sale>& sales, int page)
{
    if (sales.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет продаж для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 8, 10, 20, 12, 12, 12, 15 };
    int itemsPerPage = Config::PAGE_SIZE;
    int totalPages = (sales.size() + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)sales.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ИСТОРИЯ ПРОДАЖ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID продажи"
        << " | " << std::setw(columnWidths[1]) << std::left << "ID клиента"
        << " | " << std::setw(columnWidths[2]) << std::left << "Дата"
        << " | " << std::setw(columnWidths[3]) << std::left << "Товары"
        << " | " << std::setw(columnWidths[4]) << std::left << "Сумма"
        << " | " << std::setw(columnWidths[5]) << std::left << "Скидка"
        << " | " << std::setw(columnWidths[6]) << std::left << "Итог"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        const Sale& sale = sales[i];
        int totalItems = sale.getTotalItemsCount();

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << sale.getSaleId()
            << " | " << std::setw(columnWidths[1]) << std::right << sale.getCustomerId()
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(sale.getSaleDateTime(), columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::right << totalItems
            << " | " << std::setw(columnWidths[4]) << std::right << std::fixed << std::setprecision(2) << sale.getTotalAmount()
            << " | " << std::setw(columnWidths[5]) << std::right << std::fixed << std::setprecision(2) << sale.getDiscountAmount()
            << " | " << std::setw(columnWidths[6]) << std::right << std::fixed << std::setprecision(2) << sale.getFinalAmount()
            << " |" << std::endl;
    }

    printHorizontalLine(columnWidths);
    std::cout << "Всего продаж: " << sales.size() << std::endl;
}

void TableFormatter::displayMenu(const std::vector<std::string>& options, const std::string& title) 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== " << title << " ===" << std::endl;
    Config::resetColor();

    size_t maxWidth = 0;
    for (const auto& option : options)
    {
        maxWidth = (std::max)(maxWidth, option.length());
    }
    maxWidth += 4;

    std::cout << "+" << std::string(maxWidth + 2, '-') << "+" << std::endl;

    for (size_t i = 0; i < options.size(); i++)
    {
        std::cout << "| " << std::setw(maxWidth) << std::left << (std::to_string(i + 1) + ". " + options[i]) << " |" << std::endl;
    }

    std::cout << "| " << std::setw(maxWidth) << std::left << "0. Выход" << " |" << std::endl;
    std::cout << "+" << std::string(maxWidth + 2, '-') << "+" << std::endl;
}

void TableFormatter::displaySuccessMessage(const std::string& message) 
{
    Config::setColor(Config::COLOR_SUCCESS);
    std::cout << "[OK] " << message << std::endl;
    Config::resetColor();
}

void TableFormatter::displayErrorMessage(const std::string& message) 
{
    Config::setColor(Config::COLOR_ERROR);
    std::cout << "[ERROR] " << message << std::endl;
    Config::resetColor();
}

void TableFormatter::displayWarningMessage(const std::string& message) 
{
    Config::setColor(Config::COLOR_WARNING);
    std::cout << "[WARN] " << message << std::endl;
    Config::resetColor();
}

void TableFormatter::clearScreen() 
{
    system("cls");
}

void TableFormatter::pause() 
{
    std::cout << "Нажмите любую клавишу для продолжения...";
    _getch();
    std::cout << std::endl;
}

void TableFormatter::displayProductDetails(std::shared_ptr<Product> product) 
{
    if (!product) return;

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ПОДРОБНАЯ ИНФОРМАЦИЯ ОБ ИГРЕ ===" << std::endl;
    Config::resetColor();

    std::vector<int> widths = { 20, 60 };
    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "Поле"
        << " | " << std::setw(widths[1]) << std::left << "Значение"
        << " |" << std::endl;

    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "ID"
        << " | " << std::setw(widths[1]) << std::left << std::to_string(product->getId())
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Название"
        << " | " << std::setw(widths[1]) << std::left << product->getTitle()
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Цена"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(product->getBasePrice()) + " руб.")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Разработчик"
        << " | " << std::setw(widths[1]) << std::left << product->getDeveloper()
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Издатель"
        << " | " << std::setw(widths[1]) << std::left << product->getPublisher()
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Жанр"
        << " | " << std::setw(widths[1]) << std::left << product->getGenre()
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Возрастной рейтинг"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(product->getAgeRating()) + "+")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Дата выхода"
        << " | " << std::setw(widths[1]) << std::left << product->getReleaseDate()
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Рейтинг"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(product->getAverageRating()) + "/5")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Ключи в наличии"
        << " | " << std::setw(widths[1]) << std::left << std::to_string(product->getActivationKeys())
        << " |" << std::endl;

    if (!product->getTags().empty()) 
    {
        std::string tags;
        for (size_t i = 0; i < product->getTags().size(); ++i) 
        {
            tags += product->getTags()[i];
            if (i < product->getTags().size() - 1) tags += ", ";
        }
        std::cout << "| " << std::setw(widths[0]) << std::left << "Теги"
            << " | " << std::setw(widths[1]) << std::left << tags
            << " |" << std::endl;
    }

    std::cout << "| " << std::setw(widths[0]) << std::left << "Описание"
        << " | " << std::setw(widths[1]) << std::left << product->getDescription()
        << " |" << std::endl;

    printHorizontalLine(widths);

    if (!product->hasKeys()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "[WARN] Игра временно отсутствует в наличии" << std::endl;
        Config::resetColor();
    }
}


void TableFormatter::displayUsersTableWithPagination(const std::vector<std::shared_ptr<User>>& users, int page, int totalPages, int totalUsers)
{
    if (users.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет пользователей для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 6, 20, 25, 15, 12, 15 };
    int itemsPerPage = 5; 

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)users.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ПОЛЬЗОВАТЕЛИ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    if (totalPages > 1)
    {
        Config::setColor(Config::COLOR_MENUU);
        std::cout << "Используйте опции меню для навигации по страницам" << std::endl;
        Config::resetColor();
    }

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Имя"
        << " | " << std::setw(columnWidths[2]) << std::left << "Email"
        << " | " << std::setw(columnWidths[3]) << std::left << "Телефон"
        << " | " << std::setw(columnWidths[4]) << std::left << "Роль"
        << " | " << std::setw(columnWidths[5]) << std::left << "Статус"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        auto user = users[i];
        std::string status = user->getIsApproved() ? "Одобрен" : "Ожидание";
        if (user->isBlocked()) 
        {
            status = "Заблокирован";
            Config::setColor(Config::COLOR_ERROR);
        }
        else if (!user->getIsApproved()) 
        {
            Config::setColor(Config::COLOR_WARNING);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << user->getId()
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(user->getUsername(), columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(user->getEmail(), columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::left << truncateString(user->getPhone(), columnWidths[3])
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(user->getRole(), columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::left << status
            << " |" << std::endl;

        Config::resetColor();
    }

    printHorizontalLine(columnWidths);
    std::cout << "Показано: " << (endIndex - startIndex) << " из " << totalUsers << " пользователей" << std::endl;

    if (totalPages > 1)
    {
        std::cout << "Текущая страница: " << page << " из " << totalPages << std::endl;
    }
}

void TableFormatter::displayProductsTableWithPagination(const std::vector<std::shared_ptr<Product>>& products, int page, int totalPages, int totalProducts) 
{
    if (products.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет товаров для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 6, 30, 10, 20, 15, 8, 12 };
    int itemsPerPage = 5; 

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)products.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== КАТАЛОГ ИГР (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    if (totalPages > 1) 
    {
        Config::setColor(Config::COLOR_MENUU);
        std::cout << "Используйте опции меню для навигации по страницам" << std::endl;
        Config::resetColor();
    }

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Название"
        << " | " << std::setw(columnWidths[2]) << std::left << "Цена"
        << " | " << std::setw(columnWidths[3]) << std::left << "Разработчик"
        << " | " << std::setw(columnWidths[4]) << std::left << "Жанр"
        << " | " << std::setw(columnWidths[5]) << std::left << "Рейтинг"
        << " | " << std::setw(columnWidths[6]) << std::left << "В наличии"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        auto product = products[i];

        if (!product->hasKeys()) 
        {
            Config::setColor(Config::COLOR_WARNING);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << product->getId()
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(product->getTitle(), columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::right << std::fixed << std::setprecision(2) << product->getBasePrice()
            << " | " << std::setw(columnWidths[3]) << std::left << truncateString(product->getDeveloper(), columnWidths[3])
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(product->getGenre(), columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::right << product->getAverageRating()
            << " | " << std::setw(columnWidths[6]) << std::right << product->getActivationKeys()
            << " |" << std::endl;

        Config::resetColor();
    }

    printHorizontalLine(columnWidths);
    std::cout << "Показано: " << (endIndex - startIndex) << " из " << totalProducts << " товаров" << std::endl;

    if (totalPages > 1) 
    {
        std::cout << "Текущая страница: " << page << " из " << totalPages << std::endl;
    }
}

void TableFormatter::displayDiscountsTable(const std::vector<Discount>& discounts, int page)
{
    if (discounts.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет скидок для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 8, 15, 30, 10, 20, 20, 15 };
    int itemsPerPage = Config::PAGE_SIZE;
    int totalPages = (discounts.size() + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)discounts.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== СКИДКИ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Тип"
        << " | " << std::setw(columnWidths[2]) << std::left << "Цели"
        << " | " << std::setw(columnWidths[3]) << std::left << "Скидка %"
        << " | " << std::setw(columnWidths[4]) << std::left << "Начало"
        << " | " << std::setw(columnWidths[5]) << std::left << "Конец"
        << " | " << std::setw(columnWidths[6]) << std::left << "Статус"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        const Discount& discount = discounts[i];

        std::string targetsStr;
        if (discount.targets.size() <= 2) 
        {
            for (size_t j = 0; j < discount.targets.size(); j++) 
            {
                targetsStr += discount.targets[j];
                if (j < discount.targets.size() - 1) targetsStr += ", ";
            }
        }
        else 
        {
            targetsStr = discount.targets[0] + ", " + discount.targets[1] + "...";
        }

        std::string status = discount.isValid() ? "Активна" : "Неактивна";

        if (!discount.isValid()) 
        {
            Config::setColor(Config::COLOR_WARNING);
        }
        else 
        {
            Config::setColor(Config::COLOR_SUCCESS);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << i + 1
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(discount.type, columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(targetsStr, columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::right << std::fixed << std::setprecision(1) << discount.percentage
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(discount.startDate, columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::left << truncateString(discount.endDate, columnWidths[5])
            << " | " << std::setw(columnWidths[6]) << std::left << status
            << " |" << std::endl;

        Config::resetColor();
    }

    printHorizontalLine(columnWidths);
    std::cout << "Всего скидок: " << discounts.size() << std::endl;
}

void TableFormatter::displayDiscountsTableWithPagination(const std::vector<Discount>& discounts, int page, int totalPages, int totalDiscounts) 
{
    if (discounts.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет скидок для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 8, 15, 30, 10, 20, 20, 15 };
    int itemsPerPage = 5;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)discounts.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== СКИДКИ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    if (totalPages > 1) 
    {
        Config::setColor(Config::COLOR_MENUU);
        std::cout << "Используйте опции меню для навигации по страницам" << std::endl;
        Config::resetColor();
    }

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Тип"
        << " | " << std::setw(columnWidths[2]) << std::left << "Цели"
        << " | " << std::setw(columnWidths[3]) << std::left << "Скидка %"
        << " | " << std::setw(columnWidths[4]) << std::left << "Начало"
        << " | " << std::setw(columnWidths[5]) << std::left << "Конец"
        << " | " << std::setw(columnWidths[6]) << std::left << "Статус"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        const Discount& discount = discounts[i];

        std::string targetsStr;
        if (discount.targets.size() <= 2) 
        {
            for (size_t j = 0; j < discount.targets.size(); j++) 
            {
                targetsStr += discount.targets[j];
                if (j < discount.targets.size() - 1) targetsStr += ", ";
            }
        }
        else 
        {
            targetsStr = discount.targets[0] + ", " + discount.targets[1] + "... (+" +
                std::to_string(discount.targets.size() - 2) + ")";
        }

        std::string status = discount.isValid() ? "Активна" : "Неактивна";

        if (!discount.isValid()) 
        {
            Config::setColor(Config::COLOR_WARNING);
        }
        else 
        {
            Config::setColor(Config::COLOR_SUCCESS);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << i + 1
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(discount.type, columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(targetsStr, columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::right << std::fixed << std::setprecision(1) << discount.percentage
            << " | " << std::setw(columnWidths[4]) << std::left << truncateString(discount.startDate, columnWidths[4])
            << " | " << std::setw(columnWidths[5]) << std::left << truncateString(discount.endDate, columnWidths[5])
            << " | " << std::setw(columnWidths[6]) << std::left << status
            << " |" << std::endl;

        Config::resetColor();
    }

    printHorizontalLine(columnWidths);
    std::cout << "Показано: " << (endIndex - startIndex) << " из " << totalDiscounts << " скидок" << std::endl;

    if (totalPages > 1) 
    {
        std::cout << "Текущая страница: " << page << " из " << totalPages << std::endl;
    }
}

void TableFormatter::displayDiscountedProducts(const std::vector<std::shared_ptr<Product>>& products, const std::vector<double>& discountedPrices) 
{
    if (products.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет товаров со скидками" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 6, 30, 15, 15, 15, 20 };

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ТОВАРЫ СО СКИДКАМИ ===" << std::endl;
    Config::resetColor();

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID"
        << " | " << std::setw(columnWidths[1]) << std::left << "Название"
        << " | " << std::setw(columnWidths[2]) << std::left << "Базовая цена"
        << " | " << std::setw(columnWidths[3]) << std::left << "Цена со скидкой"
        << " | " << std::setw(columnWidths[4]) << std::left << "Скидка"
        << " | " << std::setw(columnWidths[5]) << std::left << "Экономия"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (size_t i = 0; i < products.size(); i++) 
    {
        auto product = products[i];
        double discountedPrice = discountedPrices[i];

        if (discountedPrice < product->getBasePrice()) 
        {
            double discountAmount = product->getBasePrice() - discountedPrice;
            double discountPercent = (discountAmount / product->getBasePrice()) * 100;

            Config::setColor(Config::COLOR_SUCCESS);

            std::cout << "| " << std::setw(columnWidths[0]) << std::right << product->getId()
                << " | " << std::setw(columnWidths[1]) << std::left << truncateString(product->getTitle(), columnWidths[1])
                << " | " << std::setw(columnWidths[2]) << std::right << std::fixed << std::setprecision(2) << product->getBasePrice()
                << " | " << std::setw(columnWidths[3]) << std::right << std::fixed << std::setprecision(2) << discountedPrice
                << " | " << std::setw(columnWidths[4]) << std::right << std::fixed << std::setprecision(1) << discountPercent << "%"
                << " | " << std::setw(columnWidths[5]) << std::right << std::fixed << std::setprecision(2) << discountAmount
                << " |" << std::endl;

            Config::resetColor();
        }
    }

    printHorizontalLine(columnWidths);
    std::cout << "Всего товаров со скидками: " << products.size() << std::endl;
}

void TableFormatter::displayLogsTable(const std::vector<std::string>& logs, int page, int totalPages, int totalLogs) 
{
    if (logs.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет логов для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 25, 10, 60 };
    int itemsPerPage = 10;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)logs.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== СИСТЕМНЫЕ ЛОГИ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    if (totalPages > 1)
    {
        Config::setColor(Config::COLOR_MENUU);
        std::cout << "Используйте опции меню для навигации по страницам" << std::endl;
        Config::resetColor();
    }

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "Время"
        << " | " << std::setw(columnWidths[1]) << std::left << "Уровень"
        << " | " << std::setw(columnWidths[2]) << std::left << "Сообщение"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        const std::string& logEntry = logs[i];

        size_t timeEnd = logEntry.find(']');
        size_t levelStart = logEntry.find('[', timeEnd + 1);
        size_t levelEnd = logEntry.find(']', levelStart + 1);
        std::string timeStr = logEntry.substr(1, timeEnd - 1);
        std::string levelStr = logEntry.substr(levelStart + 1, levelEnd - levelStart - 1);
        std::string message = logEntry.substr(levelEnd + 2);

        if (levelStr == "ERR" || levelStr == "CRITICAL") 
        {
            Config::setColor(Config::COLOR_ERROR);
        }
        else if (levelStr == "WARNING") 
        {
            Config::setColor(Config::COLOR_WARNING);
        }
        else
        {
            Config::setColor(Config::COLOR_DEFAULT);
        }

        std::cout << "| " << std::setw(columnWidths[0]) << std::left << truncateString(timeStr, columnWidths[0])
            << " | " << std::setw(columnWidths[1]) << std::left << truncateString(levelStr, columnWidths[1])
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(message, columnWidths[2])
            << " |" << std::endl;

        Config::resetColor();
    }

    printHorizontalLine(columnWidths);
    std::cout << "Показано: " << (endIndex - startIndex) << " из " << totalLogs << " логов" << std::endl;

    if (totalPages > 1) 
    {
        std::cout << "Текущая страница: " << page << " из " << totalPages << std::endl;
    }
}

void TableFormatter::displayReportMenu(const std::vector<std::string>& options, const std::string& title) 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== " << title << " ===" << std::endl;
    Config::resetColor();

    size_t maxWidth = 0;
    for (const auto& option : options) 
    {
        maxWidth = (std::max)(maxWidth, option.length());
    }
    maxWidth += 4;

    std::cout << "+" << std::string(maxWidth + 2, '-') << "+" << std::endl;

    for (size_t i = 0; i < options.size(); i++) 
    {
        std::cout << "| " << std::setw(maxWidth) << std::left << (std::to_string(i + 1) + ". " + options[i]) << " |" << std::endl;
    }

    std::cout << "| " << std::setw(maxWidth) << std::left << "0. Назад" << " |" << std::endl;
    std::cout << "+" << std::string(maxWidth + 2, '-') << "+" << std::endl;
}

void TableFormatter::displaySalesReport(const Report::SalesReport& report, Store& store) 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ОТЧЕТ ПО ПРОДАЖАМ ===" << std::endl;
    Config::resetColor();

    std::vector<int> widths = { 25, 40 };
    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "Параметр"
        << " | " << std::setw(widths[1]) << std::left << "Значение"
        << " |" << std::endl;

    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "Период"
        << " | " << std::setw(widths[1]) << std::left << report.period
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Общая выручка"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(report.totalRevenue) + " руб.")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Количество продаж"
        << " | " << std::setw(widths[1]) << std::left << std::to_string(report.totalSales)
        << " |" << std::endl;

    printHorizontalLine(widths);

    if (!report.topProducts.empty()) 
    {
        Config::setColor(Config::COLOR_HEADER);
        std::cout << "=== ТОП-5 ТОВАРОВ ===" << std::endl;
        Config::resetColor();

        std::vector<int> productWidths = { 5, 40, 15 };
        printHorizontalLine(productWidths);

        std::cout << "| " << std::setw(productWidths[0]) << std::left << "№"
            << " | " << std::setw(productWidths[1]) << std::left << "Товар"
            << " | " << std::setw(productWidths[2]) << std::left << "Продано"
            << " |" << std::endl;

        printHorizontalLine(productWidths);

        int rank = 1;
        for (const auto& productEntry : report.topProducts) 
        {
            auto product = store.getProductById(productEntry.first);
            if (product) 
            {
                std::cout << "| " << std::setw(productWidths[0]) << std::right << rank
                    << " | " << std::setw(productWidths[1]) << std::left << truncateString(product->getTitle(), productWidths[1])
                    << " | " << std::setw(productWidths[2]) << std::right << productEntry.second
                    << " |" << std::endl;
                rank++;
            }
        }

        printHorizontalLine(productWidths);
    }
}

void TableFormatter::displayFinancialReport(const Report::FinancialReport& report) 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ФИНАНСОВЫЙ ОТЧЕТ ===" << std::endl;
    Config::resetColor();

    std::vector<int> widths = { 30, 35 };
    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "Параметр"
        << " | " << std::setw(widths[1]) << std::left << "Значение"
        << " |" << std::endl;

    printHorizontalLine(widths);

    std::cout << "| " << std::setw(widths[0]) << std::left << "Период"
        << " | " << std::setw(widths[1]) << std::left << report.period
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Общая выручка"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(report.totalRevenue) + " руб.")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Сумма предоставленных скидок"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(report.totalDiscounts) + " руб.")
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Количество транзакций"
        << " | " << std::setw(widths[1]) << std::left << std::to_string(report.totalTransactions)
        << " |" << std::endl;

    std::cout << "| " << std::setw(widths[0]) << std::left << "Средний чек"
        << " | " << std::setw(widths[1]) << std::left << (std::to_string(report.averageTransactionValue) + " руб.")
        << " |" << std::endl;

    printHorizontalLine(widths);
}

void TableFormatter::displaySalesTableWithPagination(const std::vector<Sale>& sales, int page, int totalPages, int totalSales) 
{
    if (sales.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет продаж для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 10, 12, 20, 15, 12, 12, 15 };
    int itemsPerPage = 5;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    int startIndex = (page - 1) * itemsPerPage;
    int endIndex = (std::min)(startIndex + itemsPerPage, (int)sales.size());

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ИСТОРИЯ ПРОДАЖ (Страница " << page << " из " << totalPages << ") ===" << std::endl;
    Config::resetColor();

    if (totalPages > 1) 
    {
        Config::setColor(Config::COLOR_MENUU);
        std::cout << "Используйте опции меню для навигации по страницам" << std::endl;
        Config::resetColor();
    }

    printHorizontalLine(columnWidths);

    std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID продажи"
        << " | " << std::setw(columnWidths[1]) << std::left << "ID клиента"
        << " | " << std::setw(columnWidths[2]) << std::left << "Дата"
        << " | " << std::setw(columnWidths[3]) << std::left << "Товары"
        << " | " << std::setw(columnWidths[4]) << std::left << "Сумма"
        << " | " << std::setw(columnWidths[5]) << std::left << "Скидка"
        << " | " << std::setw(columnWidths[6]) << std::left << "Итог"
        << " |" << std::endl;

    printHorizontalLine(columnWidths);

    for (int i = startIndex; i < endIndex; i++) 
    {
        const Sale& sale = sales[i];
        int totalItems = sale.getTotalItemsCount();

        std::cout << "| " << std::setw(columnWidths[0]) << std::right << sale.getSaleId()
            << " | " << std::setw(columnWidths[1]) << std::right << sale.getCustomerId()
            << " | " << std::setw(columnWidths[2]) << std::left << truncateString(sale.getSaleDateTime(), columnWidths[2])
            << " | " << std::setw(columnWidths[3]) << std::right << totalItems
            << " | " << std::setw(columnWidths[4]) << std::right << std::fixed << std::setprecision(2) << sale.getTotalAmount()
            << " | " << std::setw(columnWidths[5]) << std::right << std::fixed << std::setprecision(2) << sale.getDiscountAmount()
            << " | " << std::setw(columnWidths[6]) << std::right << std::fixed << std::setprecision(2) << sale.getFinalAmount()
            << " |" << std::endl;
    }

    printHorizontalLine(columnWidths);
    std::cout << "Показано: " << (endIndex - startIndex) << " из " << totalSales << " продаж" << std::endl;

    if (totalPages > 1) 
    {
        std::cout << "Текущая страница: " << page << " из " << totalPages << std::endl;
    }
}

void TableFormatter::displaySalesTableDetailed(const std::vector<Sale>& sales, Store& store, int customerId) 
{
    if (sales.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет продаж для отображения" << std::endl;
        Config::resetColor();
        return;
    }

    std::vector<int> columnWidths = { 10, 35, 10, 12, 12, 15 };

    Config::setColor(Config::COLOR_HEADER);
    if (customerId == -1)
    {
        std::cout << "=== ДЕТАЛЬНАЯ ИСТОРИЯ ПРОДАЖ ===" << std::endl;
    }
    else 
    {
        std::cout << "=== ВАША ИСТОРИЯ ПОКУПОК ===" << std::endl;
    }
    Config::resetColor();

    for (const auto& sale : sales) 
    {
        if (customerId != -1 && sale.getCustomerId() != customerId) 
        {
            continue;
        }

        printHorizontalLine(columnWidths);

        Config::setColor(Config::COLOR_HEADER);
        std::cout << "| " << std::setw(columnWidths[0]) << std::left << "Продажа #" + std::to_string(sale.getSaleId())
            << " | " << std::setw(columnWidths[1]) << std::left << "Дата: " + sale.getSaleDateTime()
            << " | " << std::setw(columnWidths[2]) << std::left << ""
            << " | " << std::setw(columnWidths[3]) << std::left << ""
            << " | " << std::setw(columnWidths[4]) << std::left << ""
            << " | " << std::setw(columnWidths[5]) << std::left << ""
            << " |" << std::endl;
        Config::resetColor();

        printHorizontalLine(columnWidths);

        std::cout << "| " << std::setw(columnWidths[0]) << std::left << "ID товара"
            << " | " << std::setw(columnWidths[1]) << std::left << "Название"
            << " | " << std::setw(columnWidths[2]) << std::left << "Кол-во"
            << " | " << std::setw(columnWidths[3]) << std::left << "Цена"
            << " | " << std::setw(columnWidths[4]) << std::left << "Скидка"
            << " | " << std::setw(columnWidths[5]) << std::left << "Сумма"
            << " |" << std::endl;

        printHorizontalLine(columnWidths);

        double saleTotal = 0;
        auto items = sale.getItems();
        for (const auto& item : items) 
        {
            auto product = store.getProductById(item.first);
            if (product) 
            {
                double itemPrice = store.getDiscountedPrice(product);
                double itemTotal = itemPrice * item.second;
                saleTotal += itemTotal;
                double discount = product->getBasePrice() - itemPrice;

                std::cout << "| " << std::setw(columnWidths[0]) << std::right << product->getId()
                    << " | " << std::setw(columnWidths[1]) << std::left << truncateString(product->getTitle(), columnWidths[1])
                    << " | " << std::setw(columnWidths[2]) << std::right << item.second
                    << " | " << std::setw(columnWidths[3]) << std::right << std::fixed << std::setprecision(2) << itemPrice
                    << " | " << std::setw(columnWidths[4]) << std::right << std::fixed << std::setprecision(2) << discount
                    << " | " << std::setw(columnWidths[5]) << std::right << std::fixed << std::setprecision(2) << itemTotal
                    << " |" << std::endl;
            }
        }

        printHorizontalLine(columnWidths);

        Config::setColor(Config::COLOR_SUCCESS);
        std::cout << "| " << std::setw(columnWidths[0] + columnWidths[1] + columnWidths[2] + columnWidths[3] + columnWidths[4] + 10)
            << std::right << "ИТОГО: " << std::fixed << std::setprecision(2) << saleTotal << " руб."
            << " |" << std::endl;
        Config::resetColor();

        std::cout << std::endl;
    }
}