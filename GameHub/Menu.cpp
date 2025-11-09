#include "Menu.h"
#include "FileManager.h"

Menu::Menu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns)
    : store(s), wishlist(w), recommendationSystem(rs), report(r), notificationSystem(ns) 
{
    logger = Logger::getInstance();
}

void Menu::displayHeader(const std::string& title) 
{
    TableFormatter::clearScreen();
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "==========================================" << std::endl;
    std::cout << "           " << title << std::endl;
    std::cout << "==========================================" << std::endl;
    Config::resetColor();

    if (currentUser) 
    {
        std::cout << "Пользователь: " << currentUser->getUsername();
        std::cout << " (" << currentUser->getUserType() << ")" << std::endl;
    }
    std::cout << std::endl;
}

void MainMenu::aboutSystem()
{
    displayHeader("ИНФОРМАЦИЯ О СИСТЕМЕ");

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== GAMEHUB - СИСТЕМА УЧЕТА ПРОДАЖ ===" << std::endl;
    Config::resetColor();

    std::cout << "Версия: 1.0" << std::endl;
    std::cout << "Разработчик: Курсовой проект по ООП" << std::endl;
    std::cout << "Год: 2024" << std::endl;
    std::cout << std::endl;

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "ОСНОВНЫЕ ВОЗМОЖНОСТИ:" << std::endl;
    Config::resetColor();

    std::cout << " Управление пользователями (администраторы/покупатели)" << std::endl;
    std::cout << " Каталог компьютерных игр с ключами активации" << std::endl;
    std::cout << " Система продаж и транзакций" << std::endl;
    std::cout << " Списки желаемого для покупателей" << std::endl;
    std::cout << " Система скидок и акций" << std::endl;
    std::cout << " Рекомендательная система" << std::endl;
    std::cout << " Отчеты и аналитика" << std::endl;
    std::cout << " Система уведомлений" << std::endl;
    std::cout << std::endl;

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "ТЕХНИЧЕСКИЕ ХАРАКТЕРИСТИКИ:" << std::endl;
    Config::resetColor();

    std::cout << "Язык: C++" << std::endl;
    std::cout << "Архитектура: Многослойная (Data Layer, Business Logic, Presentation)" << std::endl;
    std::cout << "Хранение данных: Бинарные файлы" << std::endl;
    std::cout << "Безопасность: Хэширование паролей с солью" << std::endl;
    std::cout << "Интерфейс: Консольное приложение с цветовым оформлением" << std::endl;
    std::cout << std::endl;

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
    auto products = fileManager.loadFromFile<std::shared_ptr<Product>>(Config::PRODUCTS_FILE);
    auto sales = fileManager.loadFromFile<Sale>(Config::SALES_FILE);

    Config::setColor(Config::COLOR_HEADER);
    std::cout << "СТАТИСТИКА СИСТЕМЫ:" << std::endl;
    Config::resetColor();

    std::cout << "Зарегистрировано пользователей: " << users.size() << std::endl;

    int adminCount = 0;
    int customerCount = 0;
    int approvedCustomers = 0;
    for (const auto& user : users) 
    {
        if (user->getRole() == "admin") 
        {
            adminCount++;
        }
        else
        {
            customerCount++;
            if (user->getIsApproved()) 
            {
                approvedCustomers++;
            }
        }
    }

    std::cout << " - Администраторов: " << adminCount << std::endl;
    std::cout << " - Покупателей: " << customerCount << " (одобрено: " << approvedCustomers << ")" << std::endl;
    std::cout << "Товаров в каталоге: " << products.size() << std::endl;
    std::cout << "Обработано продаж: " << sales.size() << std::endl;
    std::cout << std::endl;

    Config::setColor(Config::COLOR_SUCCESS);
    std::cout << "Система работает стабильно!" << std::endl;
    Config::resetColor();

    TableFormatter::pause();
}

MainMenu::MainMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns)
    : Menu(s, w, rs, r, ns) {
}

void MainMenu::show() 
{
    while (true)
    {
        displayHeader("GAMEHUB - ГЛАВНОЕ МЕНЮ");

        std::vector<std::string> options = 
        {
            "Войти в систему",
            "Зарегистрироваться",
            "Восстановить пароль",
            "Информация о системе"
        };

        TableFormatter::displayMenu(options, "ГЛАВНОЕ МЕНЮ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 4);

        switch (choice) 
        {
        case 1:
            if (login()) 
            {
                showUserMenu();
            }
            break;
        case 2:
            registerUser();
            break;
        case 3:
            recoverPassword();
            break;
        case 4:
            aboutSystem();
            break;
        case 0:
            TableFormatter::displaySuccessMessage("До свидания!");
            return;
        }
    }
}

bool MainMenu::login() 
{
    displayHeader("ВХОД В СИСТЕМУ");

    std::string username = InputValidation::getValidatedString("Имя пользователя: ", 3, 20);
    std::cout << "Пароль: ";
    std::string password = InputValidation::maskPassword();

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (const auto& user : users) 
    {
        if (user->getUsername() == username) 
        {
            if (user->isBlocked()) 
            {
                TableFormatter::displayErrorMessage("Аккаунт заблокирован до: " + user->getUnlockDate());
                TableFormatter::pause();
                return false;
            }

            std::string storedData = user->getPasswordHash();
            size_t separatorPos = storedData.find(':');
            if (separatorPos == std::string::npos) 
            {
                TableFormatter::displayErrorMessage("Ошибка формата пароля!");
                TableFormatter::pause();
                return false;
            }

            std::string storedSalt = storedData.substr(0, separatorPos);
            std::string storedHash = storedData.substr(separatorPos + 1);
            std::string inputHash = InputValidation::hashPassword(password, storedSalt);

            if (inputHash == storedHash) 
            {
                currentUser = user;
                TableFormatter::displaySuccessMessage("Вход выполнен успешно!");
                TableFormatter::pause();
                return true;
            }
            else 
            {
                TableFormatter::displayErrorMessage("Неверный пароль!");
                TableFormatter::pause();
                return false;
            }
        }
    }

    TableFormatter::displayErrorMessage("Пользователь не найден!");
    TableFormatter::pause();
    return false;
}

void MainMenu::registerUser() 
{
    displayHeader("РЕГИСТРАЦИЯ");

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    std::string username;
    while (true) 
    {
        username = InputValidation::getValidatedString("Имя пользователя: ", 3, 20);

        bool usernameExists = false;
        for (const auto& user : users) 
        {
            if (user->getUsername() == username) 
            {
                usernameExists = true;
                break;
            }
        }

        if (!usernameExists) 
        {
            break;
        }
        else 
        {
            TableFormatter::displayErrorMessage("Имя пользователя уже занято!");
        }
    }

    std::string email = InputValidation::getValidatedEmail();
    std::string phone = InputValidation::getValidatedPhone();
    std::string birthday = InputValidation::getValidatedBirthday();

    std::cout << "Пароль: ";
    std::string password = InputValidation::maskPassword();
    std::cout << "Подтвердите пароль: ";
    std::string confirmPassword = InputValidation::maskPassword();

    if (password != confirmPassword) 
    {
        TableFormatter::displayErrorMessage("Пароли не совпадают!");
        TableFormatter::pause();
        return;
    }

    int newId = 1;
    if (!users.empty()) 
    {
        for (const auto& user : users) 
        {
            if (user->getId() >= newId) 
            {
                newId = user->getId() + 1;
            }
        }
    }

    std::string salt = InputValidation::generateSalt();
    std::string hashedPassword = InputValidation::hashPassword(password, salt);
    std::string storedPassword = salt + ":" + hashedPassword;

    auto newCustomer = std::make_shared<Customer>(newId, username, storedPassword, email, phone, birthday);
    users.push_back(newCustomer);

    if (fileManager.saveToFile(Config::USERS_FILE, users)) 
    {
        TableFormatter::displaySuccessMessage("Регистрация успешна! Ожидайте одобрения администратора.");
        logger->log(LogLevel::INFO, "New user registered: " + username);

        notificationSystem.notifyNewRegistration(username);
    }
    else 
    {
        TableFormatter::displayErrorMessage("Ошибка при сохранении данных!");
    }

    TableFormatter::pause();
}

void MainMenu::recoverPassword() 
{
    displayHeader("ВОССТАНОВЛЕНИЕ ПАРОЛЯ");

    std::string email = InputValidation::getValidatedEmail("Введите ваш email: ");
    std::string username = InputValidation::getValidatedString("Введите имя пользователя: ", 3, 20);

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users) 
    {
        if (user->getEmail() == email && user->getUsername() == username) 
        {
            std::string newPassword = InputValidation::generateRandomPassword();

            std::string newSalt = InputValidation::generateSalt();
            std::string newHashedPassword = InputValidation::hashPassword(newPassword, newSalt);
            std::string newStoredPassword = newSalt + ":" + newHashedPassword;

            user->setPasswordHash(newStoredPassword);

            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Пароль успешно сброшен!");
                TableFormatter::displayWarningMessage("Ваш новый пароль: " + newPassword);
                TableFormatter::displayWarningMessage("Сохраните его в безопасном месте и немедленно смените после входа!");
                logger->log(LogLevel::INFO, "Password recovered for user: " + username);
            }
            else 
            {
                TableFormatter::displayErrorMessage("Ошибка при сохранении нового пароля!");
            }
            TableFormatter::pause();
            return;
        }
    }

    TableFormatter::displayErrorMessage("Пользователь с указанным email и именем не найден!");
    TableFormatter::pause();
}

void MainMenu::showUserMenu() 
{
    if (!currentUser) return;

    if (currentUser->getRole() == "admin") 
    {
        AdminMenu adminMenu(store, wishlist, recommendationSystem, report, notificationSystem, currentUser);
        adminMenu.show();
    }
    else 
    {
        CustomerMenu customerMenu(store, wishlist, recommendationSystem, report, notificationSystem, currentUser);
        customerMenu.show();
    }

    currentUser = nullptr;
}

AdminMenu::AdminMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns, std::shared_ptr<User> user)
    : Menu(s, w, rs, r, ns)
{
    currentUser = user;
}

bool AdminMenu::confirmAction(const std::string& message) 
{
    Config::setColor(Config::COLOR_WARNING);
    std::cout << message << std::endl;
    Config::resetColor();

    int choice = InputValidation::getValidatedNumber("Введите 1 для подтверждения или 0 для отмены: ", 0, 1);
    return (choice == 1);
}

void AdminMenu::show() 
{
    while (true) 
    {
        displayHeader("ПАНЕЛЬ АДМИНИСТРАТОРА");

        std::vector<std::string> options =
        {
            "Управление пользователями",
            "Управление товарами",
            "Управление скидками",
            "Просмотр отчетов",
            "Просмотр системных логов",
            "Изменить мой профиль",
            "Просмотр уведомлений"
        };

        TableFormatter::displayMenu(options, "АДМИНИСТРАТОР");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 7);

        switch (choice)
        {
        case 1:
            manageUsers();
            break;
        case 2:
            manageProducts();
            break;
        case 3:
            manageDiscounts();
            break;
        case 4:
            viewReports();
            break;
        case 5:
            viewLogs();
            break;
        case 6:
            changeMyProfile();
            break;
        case 7:
            viewNotifications();
            break;
        case 0:
            return;
        }
    }
}

std::vector<std::shared_ptr<User>> AdminMenu::filterUsers(const std::vector<std::shared_ptr<User>>& users, const std::string& searchType, const std::string& searchValue) 
{
    std::vector<std::shared_ptr<User>> filteredUsers;

    if (searchValue.empty())
    {
        return users;
    }

    std::string lowerSearchValue = searchValue;
    std::transform(lowerSearchValue.begin(), lowerSearchValue.end(), lowerSearchValue.begin(), ::tolower);

    for (const auto& user : users) 
    {
        bool matches = false;

        if (searchType == "all") 
        {
            std::string username = user->getUsername();
            std::string email = user->getEmail();
            std::string phone = user->getPhone();
            std::string role = user->getRole();

            std::transform(username.begin(), username.end(), username.begin(), ::tolower);
            std::transform(email.begin(), email.end(), email.begin(), ::tolower);
            std::transform(phone.begin(), phone.end(), phone.begin(), ::tolower);
            std::transform(role.begin(), role.end(), role.begin(), ::tolower);

            matches = username.find(lowerSearchValue) != std::string::npos ||
                email.find(lowerSearchValue) != std::string::npos ||
                phone.find(lowerSearchValue) != std::string::npos ||
                role.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "username")
        {
            std::string username = user->getUsername();
            std::transform(username.begin(), username.end(), username.begin(), ::tolower);
            matches = username.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "email") 
        {
            std::string email = user->getEmail();
            std::transform(email.begin(), email.end(), email.begin(), ::tolower);
            matches = email.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "phone")
        {
            std::string phone = user->getPhone();
            std::transform(phone.begin(), phone.end(), phone.begin(), ::tolower);
            matches = phone.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "role") 
        {
            std::string role = user->getRole();
            std::transform(role.begin(), role.end(), role.begin(), ::tolower);
            matches = role.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "status") 
        {
            std::string status;
            if (user->isBlocked()) 
            {
                status = "заблокирован";
            }
            else if (user->getIsApproved())
            {
                status = "одобрен";
            }
            else 
            {
                status = "ожидание";
            }
            std::transform(status.begin(), status.end(), status.begin(), ::tolower);
            matches = status.find(lowerSearchValue) != std::string::npos;
        }

        if (matches) 
        {
            filteredUsers.push_back(user);
        }
    }

    return filteredUsers;
}

void AdminMenu::searchAndFilterUsers() 
{
    FileManager fileManager;
    auto allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    std::string searchType = "all";
    std::string searchValue = "";
    int currentPage = 1;
    const int usersPerPage = 5;

    while (true) 
    {
        displayHeader("ПОИСК И ФИЛЬТРАЦИЯ ПОЛЬЗОВАТЕЛЕЙ");

        auto filteredUsers = filterUsers(allUsers, searchType, searchValue);
        int totalPages = (filteredUsers.size() + usersPerPage - 1) / usersPerPage;
        if (totalPages == 0) totalPages = 1;

        if (currentPage > totalPages) currentPage = totalPages;
        if (currentPage < 1) currentPage = 1;

        std::cout << "Текущий фильтр: ";
        if (searchValue.empty()) 
        {
            std::cout << "не установлен" << std::endl;
        }
        else 
        {
            std::cout << searchType << " = '" << searchValue << "'" << std::endl;
        }
        std::cout << "Найдено пользователей: " << filteredUsers.size() << std::endl;

        int blockedCount = 0;
        for (const auto& user : filteredUsers) 
        {
            if (user->isBlocked())
            {
                blockedCount++;
            }
        }
        if (blockedCount > 0) 
        {
            Config::setColor(Config::COLOR_WARNING);
            std::cout << "Заблокированных пользователей: " << blockedCount << std::endl;
            Config::resetColor();
        }

        std::cout << std::endl;

        TableFormatter::displayUsersTableWithPagination(filteredUsers, currentPage, totalPages, filteredUsers.size());

        std::vector<std::string> options = 
        {
            "Изменить тип поиска",
            "Ввести значение для поиска",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Показать только заблокированных",
            "Сбросить фильтры"
        };

        TableFormatter::displayMenu(options, "ПОИСК И ФИЛЬТРАЦИЯ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 7);

        switch (choice) 
        {
        case 1: 
        {
            std::vector<std::string> searchOptions =
            {
                "Все поля",
                "Имя пользователя",
                "Email",
                "Телефон",
                "Роль",
                "Статус"
            };
            TableFormatter::displayMenu(searchOptions, "ВЫБЕРИТЕ ТИП ПОИСКА");
            int searchChoice = InputValidation::getValidatedNumber("Выберите тип поиска: ", 1, 6);
            switch (searchChoice)
            {
            case 1: searchType = "all"; break;
            case 2: searchType = "username"; break;
            case 3: searchType = "email"; break;
            case 4: searchType = "phone"; break;
            case 5: searchType = "role"; break;
            case 6: searchType = "status"; break;
            }
            currentPage = 1;
            break;
        }
        case 2: 
        {
            searchValue = InputValidation::getValidatedString("Введите значение для поиска: ", 0, 100);
            currentPage = 1;
            break;
        }
        case 3:
            if (currentPage < totalPages) currentPage++;
            break;
        case 4:
            if (currentPage > 1) currentPage--;
            break;
        case 5: 
        {
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только 1 страница");
                TableFormatter::pause();
            }
            break;
        }
        case 6: 
            searchType = "status";
            searchValue = "заблокирован";
            currentPage = 1;
            break;
        case 7:
            searchType = "all";
            searchValue = "";
            currentPage = 1;
            break;
        case 0:
            return;
        }
    }
}


void AdminMenu::manageUsers()
{
    FileManager fileManager;
    auto allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
    int currentPage = 1;
    const int usersPerPage = 5;
    int totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
    if (totalPages == 0) totalPages = 1;

    while (true) 
    {
        displayHeader("УПРАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯМИ");

        TableFormatter::displayUsersTableWithPagination(allUsers, currentPage, totalPages, allUsers.size());

        std::vector<std::string> options = 
        {
            "Одобрить пользователя",
            "Заблокировать пользователя",
            "Разблокировать пользователя",
            "Изменить роль пользователя",
            "Поиск и фильтрация пользователей",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Очистить всех неодобренных пользователей"
        };

        TableFormatter::displayMenu(options, "УПРАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 9);

        switch (choice) 
        {
        case 1:
            approveUser(allUsers, fileManager);
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 2:
            blockUser(allUsers, fileManager);
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 3: 
            unblockUser(allUsers, fileManager);
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 4:
            changeUserRole(allUsers, fileManager);
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 5:
            searchAndFilterUsers();
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 6:
            if (currentPage < totalPages) 
            {
                currentPage++;
            }
            else {
                TableFormatter::displayWarningMessage("Вы уже на последней странице!");
                TableFormatter::pause();
            }
            break;
        case 7: 
            if (currentPage > 1) {
                currentPage--;
            }
            else {
                TableFormatter::displayWarningMessage("Вы уже на первой странице!");
                TableFormatter::pause();
            }
            break;
        case 8: 
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
                TableFormatter::pause();
            }
            break;
        case 9: 
            clearUnapprovedUsers(allUsers, fileManager);
            allUsers = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);
            totalPages = (allUsers.size() + usersPerPage - 1) / usersPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 0:
            return;
        }
    }
}

void AdminMenu::clearUnapprovedUsers(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager) 
{
    int unapprovedCount = 0;
    for (const auto& user : users)
    {
        if (user->getRole() == "customer" && !user->getIsApproved()) 
        {
            unapprovedCount++;
        }
    }

    if (unapprovedCount == 0) 
    {
        TableFormatter::displayWarningMessage("Нет неодобренных пользователей для очистки.");
        TableFormatter::pause();
        return;
    }

    if (!confirmAction("Вы уверены, что хотите удалить всех " + std::to_string(unapprovedCount) + " неодобренных пользователей? ЭТО ДЕЙСТВИЕ НЕОБРАТИМО!")) 
    {
        TableFormatter::displayWarningMessage("Очистка неодобренных пользователей отменена.");
        TableFormatter::pause();
        return;
    }

    auto updatedUsers = users;
    auto it = std::remove_if(updatedUsers.begin(), updatedUsers.end(),
        [](const std::shared_ptr<User>& user) 
        {
            return user->getRole() == "customer" && !user->getIsApproved();
        });

    if (it != updatedUsers.end()) 
    {
        updatedUsers.erase(it, updatedUsers.end());
        if (fileManager.saveToFile(Config::USERS_FILE, updatedUsers)) 
        {
            TableFormatter::displaySuccessMessage("Удалено " + std::to_string(unapprovedCount) + " неодобренных пользователей!");
            logger->log(LogLevel::INFO, "Cleared " + std::to_string(unapprovedCount) + " unapproved users");
        }
    }
}

void AdminMenu::approveUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager) 
{
    int userId = InputValidation::getValidatedNumber("Введите ID пользователя для одобрения: ", 1, 1000);

    for (auto& user : const_cast<std::vector<std::shared_ptr<User>>&>(users)) 
    {
        if (user->getId() == userId && user->getRole() == "customer") 
        {
            if (!confirmAction("Вы уверены, что хотите одобрить пользователя '" + user->getUsername() + "'?")) 
            {
                TableFormatter::displayWarningMessage("Одобрение пользователя отменено.");
                return;
            }

            user->setIsApproved(true);
            if (fileManager.saveToFile(Config::USERS_FILE, users))
            {
                TableFormatter::displaySuccessMessage("Пользователь одобрен!");
                logger->log(LogLevel::INFO, "User approved: " + user->getUsername());
            }
            return;
        }
    }
    TableFormatter::displayErrorMessage("Пользователь не найден!");
    TableFormatter::pause();
}

void AdminMenu::blockUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager)
{
    int userId = InputValidation::getValidatedNumber("Введите ID пользователя для блокировки: ", 1, 1000);
    int days = InputValidation::getValidatedNumber("Введите количество дней блокировки: ", 1, 365);

    for (auto& user : const_cast<std::vector<std::shared_ptr<User>>&>(users)) 
    {
        if (user->getId() == userId) 
        {
            if (user->getRole() == "admin") 
            {
                int adminCount = 0;
                for (const auto& u : users) 
                {
                    if (u->getRole() == "admin") adminCount++;
                }
                if (adminCount <= 1) 
                {
                    TableFormatter::displayErrorMessage("Нельзя заблокировать последнего администратора!");
                    TableFormatter::pause();
                    return;
                }
            }

            if (!confirmAction("Вы уверены, что хотите заблокировать пользователя '" + user->getUsername() + "' на " + std::to_string(days) + " дней?")) 
            {
                TableFormatter::displayWarningMessage("Блокировка пользователя отменена.");
                TableFormatter::pause();
                return;
            }

            auto now = std::chrono::system_clock::now();
            auto unlockTime = now + std::chrono::hours(24 * days);
            std::time_t unlockTime_t = std::chrono::system_clock::to_time_t(unlockTime);
            std::tm tm;
            localtime_s(&tm, &unlockTime_t);

            char buffer[80];
            std::strftime(buffer, sizeof(buffer), Config::DATE_FORMAT.c_str(), &tm);
            user->setUnlockDate(buffer);

            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Пользователь заблокирован до: " + std::string(buffer));
                TableFormatter::pause();
                logger->log(LogLevel::INFO, "User blocked: " + user->getUsername());
            }
            return;
        }
    }
    TableFormatter::displayErrorMessage("Пользователь не найден!");
    TableFormatter::pause();
}

void AdminMenu::unblockUser(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager) 
{
    int userId = InputValidation::getValidatedNumber("Введите ID пользователя для разблокировки: ", 1, 1000);

    for (auto& user : const_cast<std::vector<std::shared_ptr<User>>&>(users)) 
    {
        if (user->getId() == userId) 
        {
            if (!user->isBlocked()) 
            {
                TableFormatter::displayWarningMessage("Пользователь не заблокирован!");
                TableFormatter::pause();
                return;
            }

            if (!confirmAction("Вы уверены, что хотите разблокировать пользователя '" + user->getUsername() + "'?")) 
            {
                TableFormatter::displayWarningMessage("Разблокировка пользователя отменена.");
                TableFormatter::pause();
                return;
            }

            user->setUnlockDate("");

            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Пользователь успешно разблокирован!");
                TableFormatter::pause();
                logger->log(LogLevel::INFO, "User unblocked: " + user->getUsername());
            }
            return;
        }
    }
    TableFormatter::displayErrorMessage("Пользователь не найден!");
}

void AdminMenu::changeUserRole(const std::vector<std::shared_ptr<User>>& users, FileManager& fileManager) 
{
    int userId = InputValidation::getValidatedNumber("Введите ID пользователя (0 для отмены): ", 1, 1000);
    if (userId == 0)
    {
        TableFormatter::displayWarningMessage("Операция отменена.");
        TableFormatter::pause();
        return;
    }

    std::vector<std::string> roleOptions = { "admin", "customer" };
    TableFormatter::displayMenu(roleOptions, "ВЫБЕРИТЕ РОЛЬ");

    int roleChoice = InputValidation::getValidatedNumber("Выберите роль (0 для отмены): ", 0, 2);
    if (roleChoice == 0) {
        TableFormatter::displayWarningMessage("Операция отменена.");
        TableFormatter::pause();
        return;
    }
    std::string newRole = (roleChoice == 1) ? "admin" : "customer";

    for (auto& user : const_cast<std::vector<std::shared_ptr<User>>&>(users)) 
    {
        if (user->getId() == userId) 
        {
            if (user->getRole() == "admin" && newRole == "customer") 
            {
                int adminCount = 0;
                for (const auto& u : users) 
                {
                    if (u->getRole() == "admin") adminCount++;
                }
                if (adminCount <= 1) 
                {
                    TableFormatter::displayErrorMessage("Нельзя изменить роль последнему администратору!");
                    TableFormatter::pause();
                    return;
                }
            }

            if (!confirmAction("Вы уверены, что хотите изменить роль пользователя '" + user->getUsername() + "' с '" + user->getRole() + "' на '" + newRole + "'?")) {
                TableFormatter::displayWarningMessage("Изменение роли пользователя отменено.");
                TableFormatter::pause();
                return;
            }

            user->setRole(newRole);
            if (newRole == "admin")
            {
                user->setIsApproved(true);
            }

            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Роль пользователя изменена на: " + newRole);
                TableFormatter::pause();
                logger->log(LogLevel::INFO, "User role changed: " + user->getUsername() + " -> " + newRole);
            }
            return;
        }
    }
    TableFormatter::displayErrorMessage("Пользователь не найден!");
    TableFormatter::pause();
}

void AdminMenu::manageProducts() 
{
    int currentPage = 1;
    const int productsPerPage = 5;

    while (true)
    {
        displayHeader("УПРАВЛЕНИЕ ТОВАРАМИ");

        auto allProducts = store.getAllProducts();
        int totalPages = (allProducts.size() + productsPerPage - 1) / productsPerPage;
        if (totalPages == 0) totalPages = 1;

        TableFormatter::displayProductsTableWithPagination(allProducts, currentPage, totalPages, allProducts.size());

        std::vector<std::string> options = 
        {
            "Добавить новый товар",
            "Редактировать товар",
            "Удалить товар",
            "Пополнить ключи для товара",
            "Добавить новый жанр",
            "Удалить жанр",
            "Добавить новый тег",
            "Удалить тег",
            "Поиск и фильтрация товаров",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице"
        };

        TableFormatter::displayMenu(options, "УПРАВЛЕНИЕ ТОВАРАМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 12);

        switch (choice) 
        {
        case 1:
            addNewProduct();
            break;
        case 2:
            editProduct();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            addKeysToProduct();
            break;
        case 5:
            addNewGenre();
            break;
        case 6:
            deleteGenre();
            break;
        case 7:
            addNewTag();
            break;
        case 8:
            deleteTag();
            break;
        case 9:
            searchAndFilterProducts();
            break;
        case 10:
            if (currentPage < totalPages) currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 11:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 12:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
                TableFormatter::pause();
            }
            break;
        case 0:
            return;
        }
    }
}

void AdminMenu::addNewProduct() 
{
    displayHeader("ДОБАВЛЕНИЕ НОВОГО ТОВАРА");

    if (!confirmAction("Вы уверены, что хотите добавить новый товар?")) 
    {
        TableFormatter::displayWarningMessage("Добавление товара отменено.");
        TableFormatter::pause();
        return;
    }

    FileManager fileManager;
    auto genres = fileManager.loadStringList(Config::GENRES_FILE);
    auto tags = fileManager.loadStringList(Config::TAGS_FILE);

    std::string title = InputValidation::getValidatedString("Название игры: ", 1, 100);
    double price = InputValidation::getValidatedDouble("Цена: ", 0.0, 10000.0);

    auto developers = store.getUniqueDevelopers();
    std::cout << "Существующие разработчики: ";
    for (size_t i = 0; i < developers.size(); ++i)
    {
        std::cout << developers[i];
        if (i < developers.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::string developer = InputValidation::getValidatedString("Разработчик: ", 1, 50);

    if (!genres.empty()) 
    {
        std::cout << "Доступные жанры: ";
        for (size_t i = 0; i < genres.size(); ++i) 
        {
            std::cout << genres[i];
            if (i < genres.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    std::string genre = InputValidation::getValidatedString("Жанр: ", 1, 30);

    std::string publisher = InputValidation::getValidatedString("Издатель: ", 1, 50);
    int ageRating = InputValidation::getValidatedNumber("Возрастной рейтинг: ", 0, 21);
    std::string description = InputValidation::getValidatedString("Описание: ", 1, 500);
    double averageRating = InputValidation::getValidatedDouble("Рейтинг (0.0-5.0): ", 0.0, 5.0);
    std::string releaseDate = InputValidation::getValidatedDate("Дата выхода (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");
    int initialKeys = InputValidation::getValidatedNumber("Начальное количество ключей: ", 0, 10000);

    auto newProduct = std::make_shared<Game>(0, title, price, developer, publisher,
        genre, ageRating, description, releaseDate);
    newProduct->setActivationKeys(initialKeys);

    if (!tags.empty())
    {
        std::cout << "Доступные теги: ";
        for (size_t i = 0; i < tags.size(); ++i) 
        {
            std::cout << tags[i];
            if (i < tags.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;

        std::cout << "Введите теги через запятую (или оставьте пустым): ";
        std::string tagsInput;
        std::getline(std::cin, tagsInput);

        if (!tagsInput.empty()) 
        {
            std::istringstream iss(tagsInput);
            std::string tag;
            while (std::getline(iss, tag, ',')) 
            {
                tag.erase(0, tag.find_first_not_of(' '));
                tag.erase(tag.find_last_not_of(' ') + 1);
                if (!tag.empty())
                {
                    newProduct->addTag(tag);
                }
            }
        }
    }

    store.addProduct(newProduct);
    TableFormatter::displaySuccessMessage("Товар успешно добавлен!");
    TableFormatter::pause();
}

void AdminMenu::editProduct()
{
    displayHeader("РЕДАКТИРОВАНИЕ ТОВАРА");
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5; 
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для редактирования (0 для отмены): ", 0, 10000);
    if (productId == 0) 
    {
        TableFormatter::displayWarningMessage("Редактирование отменено.");
        TableFormatter::pause();
        return;
    }

    auto product = store.getProductById(productId);

    if (!product) 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
        TableFormatter::pause();
        return;
    }

    TableFormatter::displayProductDetails(product);
    std::cout << std::endl;

    if (!confirmAction("Вы уверены, что хотите редактировать этот товар?")) 
    {
        TableFormatter::displayWarningMessage("Редактирование отменено.");
        TableFormatter::pause();
        return;
    }

    FileManager fileManager;
    auto genres = fileManager.loadStringList(Config::GENRES_FILE);
    auto tags = fileManager.loadStringList(Config::TAGS_FILE);

    std::string newTitle = InputValidation::getValidatedString("Новое название (оставьте пустым для сохранения текущего): ", 0, 100);
    if (!newTitle.empty()) product->setTitle(newTitle);

    double newPrice = InputValidation::getValidatedDouble("Новая цена (введите -1 для сохранения текущей): ", -1, 10000.0);
    if (newPrice >= 0) product->setBasePrice(newPrice);

    auto developers = store.getUniqueDevelopers();
    std::cout << "Существующие разработчики: ";
    for (size_t i = 0; i < developers.size(); ++i) 
    {
        std::cout << developers[i];
        if (i < developers.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::string newDeveloper = InputValidation::getValidatedString("Новый разработчик (оставьте пустым для сохранения текущего): ", 0, 50);
    if (!newDeveloper.empty()) product->setDeveloper(newDeveloper);

    if (!genres.empty()) 
    {
        std::cout << "Доступные жанры: ";
        for (size_t i = 0; i < genres.size(); ++i) 
        {
            std::cout << genres[i];
            if (i < genres.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    std::string newGenre = InputValidation::getValidatedString("Новый жанр (оставьте пустым для сохранения текущего): ", 0, 30);
    if (!newGenre.empty()) product->setGenre(newGenre);

    std::string newPublisher = InputValidation::getValidatedString("Новый издатель (оставьте пустым для сохранения текущего): ", 0, 50);
    if (!newPublisher.empty()) product->setPublisher(newPublisher);

    int newAgeRating = InputValidation::getValidatedNumber("Новый возрастной рейтинг (введите -1 для сохранения текущего): ", -1, 21);
    if (newAgeRating >= 0) product->setAgeRating(newAgeRating);

    std::string newDescription = InputValidation::getValidatedString("Новое описание (оставьте пустым для сохранения текущего): ", 0, 500);
    if (!newDescription.empty()) product->setDescription(newDescription);

    if (store.updateProduct(productId, product)) 
    {
        TableFormatter::displaySuccessMessage("Товар успешно обновлен!");
    }
    else 
    {
        TableFormatter::displayErrorMessage("Ошибка при обновлении товара!");
    }
    TableFormatter::pause();
}

void AdminMenu::deleteProduct()
{
    displayHeader("УДАЛЕНИЕ ТОВАРА");
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5; 
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для удаления (0 для отмены): ", 0, 10000);
    if (productId == 0) 
    {
        TableFormatter::displayWarningMessage("Удаление отменено.");
        TableFormatter::pause();
        return;
    }

    auto product = store.getProductById(productId);
    if (product) 
    {
        TableFormatter::displayProductDetails(product);
        std::cout << std::endl;

        if (confirmAction("Вы уверены, что хотите удалить этот товар?"))
        {
            if (store.deleteProduct(productId)) 
            {
                TableFormatter::displaySuccessMessage("Товар успешно удален!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Ошибка при удалении товара!");
            }
        }
        else 
        {
            TableFormatter::displayWarningMessage("Удаление отменено.");
        }
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
    }
    TableFormatter::pause();
}

void AdminMenu::addKeysToProduct() 
{
    displayHeader("ПОПОЛНЕНИЕ КЛЮЧЕЙ");
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5; 
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара (0 для отмены): ", 0, 10000);
    if (productId == 0) 
    {
        TableFormatter::displayWarningMessage("Операция отменена.");
        TableFormatter::pause();
        return;
    }

    auto product = store.getProductById(productId);

    if (!product) 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
        TableFormatter::pause();
        return;
    }

    TableFormatter::displayProductDetails(product);
    std::cout << std::endl;

    int oldKeyCount = product->getActivationKeys();

    int keysToAdd = InputValidation::getValidatedNumber("Сколько ключей добавить? (0 для отмены): ", 0, 10000);
    if (keysToAdd == 0) 
    {
        TableFormatter::displayWarningMessage("Операция отменена.");
        TableFormatter::pause();
        return;
    }

    if (store.addKeys(productId, keysToAdd)) 
    {
        TableFormatter::displaySuccessMessage("Ключи успешно добавлены!");

        auto updatedProduct = store.getProductById(productId);
        if (updatedProduct) 
        {
            if (updatedProduct->getActivationKeys() < 5)
            {
                notificationSystem.notifyLowStock(productId, updatedProduct->getTitle());
            }

            if (oldKeyCount == 0 && updatedProduct->getActivationKeys() > 0) 
            {
                auto wishlistProducts = wishlist.getWishlistProductsForAllUsers();

                for (const auto& wishlistItem : wishlistProducts) 
                {
                    if (wishlistItem.productId == productId) 
                    {
                        notificationSystem.notifyWishlistBackInStock(
                            wishlistItem.customerId,
                            updatedProduct->getTitle()
                        );

                        logger->log(LogLevel::INFO,
                            "Back in stock notification sent to customer " +
                            std::to_string(wishlistItem.customerId) +
                            " for product: " + updatedProduct->getTitle()
                        );
                    }
                }
            }
        }
    }
    else 
    {
        TableFormatter::displayErrorMessage("Ошибка при добавлении ключей!");
    }
    TableFormatter::pause();
}
void AdminMenu::addNewGenre() 
{
    displayHeader("ДОБАВЛЕНИЕ НОВОГО ЖАНРА");

    FileManager fileManager;
    auto genres = fileManager.loadStringList(Config::GENRES_FILE);

    if (!genres.empty()) 
    {
        std::cout << "Текущие жанры: ";
        for (size_t i = 0; i < genres.size(); ++i) 
        {
            std::cout << genres[i];
            if (i < genres.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    std::string newGenre = InputValidation::getValidatedString("Введите новый жанр (0 для отмены): ", 0, 30);
    if (newGenre == "0") 
    {
        TableFormatter::displayWarningMessage("Добавление жанра отменено.");
        TableFormatter::pause();
        return;
    }

    if (std::find(genres.begin(), genres.end(), newGenre) != genres.end()) {
        TableFormatter::displayWarningMessage("Такой жанр уже существует!");
    }
    else 
    {
        genres.push_back(newGenre);
        if (fileManager.saveStringList(Config::GENRES_FILE, genres)) 
        {
            TableFormatter::displaySuccessMessage("Жанр успешно добавлен!");
        }
        else 
        {
            TableFormatter::displayErrorMessage("Ошибка при сохранении жанра!");
        }
    }
    TableFormatter::pause();
}

void AdminMenu::deleteGenre()
{
    displayHeader("УДАЛЕНИЕ ЖАНРА");

    FileManager fileManager;
    auto genres = fileManager.loadStringList(Config::GENRES_FILE);

    if (genres.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет жанров для удаления.");
        TableFormatter::pause();
        return;
    }

    std::cout << "Текущие жанры: ";
    for (size_t i = 0; i < genres.size(); ++i) 
    {
        std::cout << genres[i];
        if (i < genres.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::string genreToDelete = InputValidation::getValidatedString("Введите жанр для удаления (0 для отмены): ", 0, 30);
    if (genreToDelete == "0") 
    {
        TableFormatter::displayWarningMessage("Удаление жанра отменено.");
        TableFormatter::pause();
        return;
    }

    auto it = std::find(genres.begin(), genres.end(), genreToDelete);
    if (it == genres.end()) 
    {
        TableFormatter::displayErrorMessage("Такой жанр не найден!");
    }
    else
    {
        if (confirmAction("Вы уверены, что хотите удалить жанр '" + genreToDelete + "'?")) 
        {
            genres.erase(it);
            if (fileManager.saveStringList(Config::GENRES_FILE, genres)) 
            {
                TableFormatter::displaySuccessMessage("Жанр успешно удален!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Ошибка при сохранении списка жанров!");
            }
        }
        else 
        {
            TableFormatter::displayWarningMessage("Удаление жанра отменено.");
        }
    }
    TableFormatter::pause();
}

void AdminMenu::addNewTag() 
{
    displayHeader("ДОБАВЛЕНИЕ НОВОГО ТЕГА");

    FileManager fileManager;
    auto tags = fileManager.loadStringList(Config::TAGS_FILE);

    if (!tags.empty()) 
    {
        std::cout << "Текущие теги: ";
        for (size_t i = 0; i < tags.size(); ++i)
        {
            std::cout << tags[i];
            if (i < tags.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    std::string newTag = InputValidation::getValidatedString("Введите новый тег (0 для отмены): ", 0, 30);
    if (newTag == "0") 
    {
        TableFormatter::displayWarningMessage("Добавление тега отменено.");
        TableFormatter::pause();
        return;
    }

    if (std::find(tags.begin(), tags.end(), newTag) != tags.end()) 
    {
        TableFormatter::displayWarningMessage("Такой тег уже существует!");
    }
    else 
    {
        tags.push_back(newTag);
        if (fileManager.saveStringList(Config::TAGS_FILE, tags)) 
        {
            TableFormatter::displaySuccessMessage("Тег успешно добавлен!");
        }
        else 
        {
            TableFormatter::displayErrorMessage("Ошибка при сохранении тега!");
        }
    }
    TableFormatter::pause();
}

void AdminMenu::deleteTag()
{
    displayHeader("УДАЛЕНИЕ ТЕГА");

    FileManager fileManager;
    auto tags = fileManager.loadStringList(Config::TAGS_FILE);

    if (tags.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет тегов для удаления.");
        TableFormatter::pause();
        return;
    }

    std::cout << "Текущие теги: ";
    for (size_t i = 0; i < tags.size(); ++i)
    {
        std::cout << tags[i];
        if (i < tags.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::string tagToDelete = InputValidation::getValidatedString("Введите тег для удаления (0 для отмены): ", 0, 30);
    if (tagToDelete == "0")
    {
        TableFormatter::displayWarningMessage("Удаление тега отменено.");
        TableFormatter::pause();
        return;
    }

    auto it = std::find(tags.begin(), tags.end(), tagToDelete);
    if (it == tags.end()) 
    {
        TableFormatter::displayErrorMessage("Такой тег не найден!");
    }
    else 
    {
        if (confirmAction("Вы уверены, что хотите удалить тег '" + tagToDelete + "'?")) 
        {
            tags.erase(it);
            if (fileManager.saveStringList(Config::TAGS_FILE, tags)) 
            {
                TableFormatter::displaySuccessMessage("Тег успешно удален!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Ошибка при сохранении списка тегов!");
            }
        }
        else 
        {
            TableFormatter::displayWarningMessage("Удаление тега отменено.");
        }
    }
    TableFormatter::pause();
}

std::vector<std::shared_ptr<Product>> AdminMenu::filterProducts(const std::vector<std::shared_ptr<Product>>& products, const std::string& searchType, const std::string& searchValue) 
{
    std::vector<std::shared_ptr<Product>> filteredProducts;

    if (searchValue.empty()) 
    {
        return products;
    }

    std::string lowerSearchValue = searchValue;
    std::transform(lowerSearchValue.begin(), lowerSearchValue.end(), lowerSearchValue.begin(), ::tolower);

    for (const auto& product : products) 
    {
        bool matches = false;

        if (searchType == "all") 
        {
            std::string title = product->getTitle();
            std::string developer = product->getDeveloper();
            std::string genre = product->getGenre();
            std::string publisher = product->getPublisher();

            std::transform(title.begin(), title.end(), title.begin(), ::tolower);
            std::transform(developer.begin(), developer.end(), developer.begin(), ::tolower);
            std::transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
            std::transform(publisher.begin(), publisher.end(), publisher.begin(), ::tolower);

            matches = title.find(lowerSearchValue) != std::string::npos ||
                developer.find(lowerSearchValue) != std::string::npos ||
                genre.find(lowerSearchValue) != std::string::npos ||
                publisher.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "title") 
        {
            std::string title = product->getTitle();
            std::transform(title.begin(), title.end(), title.begin(), ::tolower);
            matches = title.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "developer") 
        {
            std::string developer = product->getDeveloper();
            std::transform(developer.begin(), developer.end(), developer.begin(), ::tolower);
            matches = developer.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "genre") 
        {
            std::string genre = product->getGenre();
            std::transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
            matches = genre.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "publisher")
        {
            std::string publisher = product->getPublisher();
            std::transform(publisher.begin(), publisher.end(), publisher.begin(), ::tolower);
            matches = publisher.find(lowerSearchValue) != std::string::npos;
        }
        else if (searchType == "in_stock")
        {
            matches = product->hasKeys();
        }
        else if (searchType == "out_of_stock") 
        {
            matches = !product->hasKeys();
        }

        if (matches) 
        {
            filteredProducts.push_back(product);
        }
    }

    return filteredProducts;
}

void AdminMenu::searchAndFilterProducts() 
{
    displayHeader("ПОИСК И ФИЛЬТРАЦИЯ ТОВАРОВ");

    auto allProducts = store.getAllProducts();
    std::string searchType = "all";
    std::string searchValue = "";
    int currentPage = 1;
    const int productsPerPage = 5;

    while (true) 
    {
        displayHeader("ПОИСК И ФИЛЬТРАЦИЯ ТОВАРОВ");

        auto filteredProducts = filterProducts(allProducts, searchType, searchValue);
        int totalPages = (filteredProducts.size() + productsPerPage - 1) / productsPerPage;
        if (totalPages == 0) totalPages = 1;

        if (currentPage > totalPages) currentPage = totalPages;
        if (currentPage < 1) currentPage = 1;

        std::cout << "Текущий фильтр: ";
        if (searchValue.empty()) 
        {
            std::cout << "не установлен" << std::endl;
        }
        else 
        {
            std::cout << searchType << " = '" << searchValue << "'" << std::endl;
        }
        std::cout << "Найдено товаров: " << filteredProducts.size() << std::endl;
        std::cout << std::endl;

        TableFormatter::displayProductsTableWithPagination(filteredProducts, currentPage, totalPages, filteredProducts.size());

        std::vector<std::string> options = 
        {
            "Изменить тип поиска",
            "Ввести значение для поиска",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Сбросить фильтры"
        };

        TableFormatter::displayMenu(options, "ПОИСК И ФИЛЬТРАЦИЯ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 6);

        switch (choice) 
        {
        case 1:
        {
            std::vector<std::string> searchOptions = 
            {
                "Все поля",
                "Название",
                "Разработчик",
                "Жанр",
                "Издатель",
                "В наличии",
                "Нет в наличии"
            };
            TableFormatter::displayMenu(searchOptions, "ВЫБЕРИТЕ ТИП ПОИСКА");
            int searchChoice = InputValidation::getValidatedNumber("Выберите тип поиска: ", 0, 7);
            switch (searchChoice) 
            {
            case 1: searchType = "all"; break;
            case 2: searchType = "title"; break;
            case 3: searchType = "developer"; break;
            case 4: searchType = "genre"; break;
            case 5: searchType = "publisher"; break;
            case 6: searchType = "in_stock"; break;
            case 7: searchType = "out_of_stock"; break;
            case 0:
                return;
            }
            currentPage = 1;
            break;
        }
        case 2: 
        {
            if (searchType == "in_stock" || searchType == "out_of_stock")
            {
                TableFormatter::displayWarningMessage("Для этого типа поиска не требуется значение!");
            }
            else 
            {
                searchValue = InputValidation::getValidatedString("Введите значение для поиска: ", 0, 100);
            }
            currentPage = 1;
            break;
        }
        case 3:
            if (currentPage < totalPages) currentPage++;
            break;
        case 4:
            if (currentPage > 1) currentPage--;
            break;
        case 5: 
        {
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только 1 страница");
                TableFormatter::pause();
            }
            break;
        }
        case 6:
            searchType = "all";
            searchValue = "";
            currentPage = 1;
            break;
        case 0:
            return;
        }
    }
}

void AdminMenu::manageDiscounts() 
{
    int currentPage = 1;
    const int discountsPerPage = 5;

    while (true) 
    {
        displayHeader("УПРАВЛЕНИЕ СКИДКАМИ");

        auto allDiscounts = store.getAllDiscounts();
        int totalPages = (allDiscounts.size() + discountsPerPage - 1) / discountsPerPage;
        if (totalPages == 0) totalPages = 1;

        TableFormatter::displayDiscountsTableWithPagination(allDiscounts, currentPage, totalPages, allDiscounts.size());

        std::vector<std::string> options = 
        {
            "Добавить новую скидку",
            "Редактировать скидку",
            "Удалить скидку",
            "Просмотреть товары со скидками",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице"
        };

        TableFormatter::displayMenu(options, "УПРАВЛЕНИЕ СКИДКАМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 7);

        switch (choice) 
        {
        case 1:
            addNewDiscount();
            allDiscounts = store.getAllDiscounts();
            totalPages = (allDiscounts.size() + discountsPerPage - 1) / discountsPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 2:
            editDiscount();
            allDiscounts = store.getAllDiscounts();
            totalPages = (allDiscounts.size() + discountsPerPage - 1) / discountsPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 3:
            deleteDiscount();
            allDiscounts = store.getAllDiscounts();
            totalPages = (allDiscounts.size() + discountsPerPage - 1) / discountsPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            break;
        case 4:
            viewDiscountedProducts();
            break;
        case 5:
            if (currentPage < totalPages) currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 6:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 7:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
                TableFormatter::pause();
            }
            break;
        case 0:
            return;
        }
    }
}

void AdminMenu::addNewDiscount() 
{
    displayHeader("ДОБАВЛЕНИЕ НОВОЙ СКИДКИ");

    if (!confirmAction("Вы уверены, что хотите добавить новую скидку?")) 
    {
        TableFormatter::displayWarningMessage("Добавление скидки отменено.");
        TableFormatter::pause();
        return;
    }

    std::vector<std::string> discountTypes =
    {
        "На товары (по ID)",
        "На жанры",
        "На теги",
        "На разработчиков"
    };

    TableFormatter::displayMenu(discountTypes, "ВЫБЕРИТЕ ТИП СКИДКИ");

    int typeChoice = InputValidation::getValidatedNumber("Выберите тип скидки: ", 0, 4);
    std::string discountType;

    switch (typeChoice) 
    {
    case 1: discountType = "product"; break;
    case 2: discountType = "genre"; break;
    case 3: discountType = "tag"; break;
    case 4: discountType = "developer"; break;
    case 0:
        return;
    }

    std::vector<std::string> targets;

    switch (typeChoice) 
    {
    case 1: selectProductsForDiscount(targets); break;
    case 2: selectGenresForDiscount(targets); break;
    case 3: selectTagsForDiscount(targets); break;
    case 4: selectDevelopersForDiscount(targets); break;
    }

    if (targets.empty()) 
    {
        TableFormatter::displayWarningMessage("Не выбрано ни одной цели для скидки!");
        TableFormatter::pause();
        return;
    }

    double percentage = InputValidation::getValidatedDouble("Введите процент скидки (1-100): ", 1.0, 100.0);

    std::string startDate = InputValidation::getValidatedDate("Введите дату начала скидки (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");
    std::string endDate = InputValidation::getValidatedDate("Введите дату окончания скидки (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");

    if (startDate >= endDate) {
        TableFormatter::displayErrorMessage("Дата окончания должна быть позже даты начала!");
        TableFormatter::pause();
        return;
    }

    Discount newDiscount;
    newDiscount.type = discountType;
    newDiscount.targets = targets;
    newDiscount.percentage = percentage;
    newDiscount.startDate = startDate;
    newDiscount.endDate = endDate;

    TableFormatter::displaySuccessMessage("Скидка создана:");
    std::cout << "Тип: " << discountType << std::endl;
    std::cout << "Цели: ";
    for (size_t i = 0; i < targets.size(); ++i) 
    {
        std::cout << targets[i];
        if (i < targets.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "Скидка: " << percentage << "%" << std::endl;
    std::cout << "Период: " << startDate << " - " << endDate << std::endl;

    if (confirmAction("Сохранить эту скидку?")) 
    {
        store.addDiscount(newDiscount);
        TableFormatter::displaySuccessMessage("Скидка успешно добавлена!");
        logger->log(LogLevel::INFO, "Discount added: " + discountType + " " + std::to_string(percentage) + "%");
    }
    else 
    {
        TableFormatter::displayWarningMessage("Скидка не была сохранена.");
    }

    TableFormatter::pause();
}

void AdminMenu::selectProductsForDiscount(std::vector<std::string>& targets) 
{
    displayHeader("ВЫБОР ТОВАРОВ ДЛЯ СКИДКИ");

    auto products = store.getAllProducts();
    TableFormatter::displayProductsTable(products);

    TableFormatter::displaySuccessMessage("Вводите ID товаров для применения скидки (0 для завершения):");

    while (true) 
    {
        int productId = InputValidation::getValidatedNumber("ID товара: ", 0, 10000);

        if (productId == 0) 
        {
            break;
        }

        auto product = store.getProductById(productId);
        if (product) 
        {
            if (std::find(targets.begin(), targets.end(), std::to_string(productId)) == targets.end()) 
            {
                targets.push_back(std::to_string(productId));
                TableFormatter::displaySuccessMessage("Товар добавлен: " + product->getTitle());
            }
            else 
            {
                TableFormatter::displayWarningMessage("Товар уже выбран!");
            }
        }
        else 
        {
            TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
        }

        if (targets.size() >= 10) 
        {
            TableFormatter::displayWarningMessage("Достигнут лимит в 10 товаров.");
            break;
        }
    }
}

void AdminMenu::selectGenresForDiscount(std::vector<std::string>& targets) 
{
    displayHeader("ВЫБОР ЖАНРОВ ДЛЯ СКИДКИ");

    FileManager fileManager;
    auto genres = fileManager.loadStringList(Config::GENRES_FILE);

    if (genres.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет доступных жанров.");
        return;
    }

    for (size_t i = 0; i < genres.size(); ++i) 
    {
        std::cout << i + 1 << ". " << genres[i] << std::endl;
    }

    TableFormatter::displaySuccessMessage("Вводите номера жанров для применения скидки (0 для завершения):");

    while (true) 
    {
        int genreChoice = InputValidation::getValidatedNumber("Номер жанра: ", 0, genres.size());

        if (genreChoice == 0) 
        {
            break;
        }

        std::string selectedGenre = genres[genreChoice - 1];
        if (std::find(targets.begin(), targets.end(), selectedGenre) == targets.end())
        {
            targets.push_back(selectedGenre);
            TableFormatter::displaySuccessMessage("Жанр добавлен: " + selectedGenre);
        }
        else 
        {
            TableFormatter::displayWarningMessage("Жанр уже выбран!");
        }
    }
}

void AdminMenu::selectTagsForDiscount(std::vector<std::string>& targets) 
{
    displayHeader("ВЫБОР ТЕГОВ ДЛЯ СКИДКИ");

    FileManager fileManager;
    auto tags = fileManager.loadStringList(Config::TAGS_FILE);

    if (tags.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет доступных тегов.");
        return;
    }

    for (size_t i = 0; i < tags.size(); ++i) 
    {
        std::cout << i + 1 << ". " << tags[i] << std::endl;
    }

    TableFormatter::displaySuccessMessage("Вводите номера тегов для применения скидки (0 для завершения):");

    while (true)
    {
        int tagChoice = InputValidation::getValidatedNumber("Номер тега: ", 0, tags.size());

        if (tagChoice == 0) 
        {
            break;
        }

        std::string selectedTag = tags[tagChoice - 1];
        if (std::find(targets.begin(), targets.end(), selectedTag) == targets.end()) 
        {
            targets.push_back(selectedTag);
            TableFormatter::displaySuccessMessage("Тег добавлен: " + selectedTag);
        }
        else 
        {
            TableFormatter::displayWarningMessage("Тег уже выбран!");
        }
    }
}

void AdminMenu::selectDevelopersForDiscount(std::vector<std::string>& targets)
{
    displayHeader("ВЫБОР РАЗРАБОТЧИКОВ ДЛЯ СКИДКИ");

    auto developers = store.getUniqueDevelopers();

    if (developers.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет доступных разработчиков.");
        return;
    }

    for (size_t i = 0; i < developers.size(); ++i) 
    {
        std::cout << i + 1 << ". " << developers[i] << std::endl;
    }

    TableFormatter::displaySuccessMessage("Вводите номера разработчиков для применения скидки (0 для завершения):");

    while (true)
    {
        int devChoice = InputValidation::getValidatedNumber("Номер разработчика: ", 0, developers.size());

        if (devChoice == 0) 
        {
            break;
        }

        std::string selectedDev = developers[devChoice - 1];
        if (std::find(targets.begin(), targets.end(), selectedDev) == targets.end()) 
        {
            targets.push_back(selectedDev);
            TableFormatter::displaySuccessMessage("Разработчик добавлен: " + selectedDev);
        }
        else 
        {
            TableFormatter::displayWarningMessage("Разработчик уже выбран!");
        }
    }
}

void AdminMenu::editDiscount() 
{
    displayHeader("РЕДАКТИРОВАНИЕ СКИДКИ");

    auto discounts = store.getAllDiscounts();

    if (discounts.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет скидок для редактирования.");
        TableFormatter::pause();
        return;
    }

    TableFormatter::displayDiscountsTable(discounts);

    int discountIndex = InputValidation::getValidatedNumber("Введите номер скидки для редактирования (0 для отмены): ", 0, discounts.size());

    if (discountIndex == 0) 
    {
        TableFormatter::displayWarningMessage("Редактирование отменено.");
        TableFormatter::pause();
        return;
    }

    Discount& discount = const_cast<Discount&>(discounts[discountIndex - 1]);

    TableFormatter::displaySuccessMessage("Текущие данные скидки:");
    std::cout << "Тип: " << discount.type << std::endl;
    std::cout << "Цели: ";
    for (size_t i = 0; i < discount.targets.size(); ++i) 
    {
        std::cout << discount.targets[i];
        if (i < discount.targets.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "Скидка: " << discount.percentage << "%" << std::endl;
    std::cout << "Начало: " << discount.startDate << std::endl;
    std::cout << "Конец: " << discount.endDate << std::endl;
    std::cout << std::endl;

    if (!confirmAction("Вы уверены, что хотите редактировать эту скидку?")) 
    {
        TableFormatter::displayWarningMessage("Редактирование отменено.");
        TableFormatter::pause();
        return;
    }

    double newPercentage = InputValidation::getValidatedDouble("Новый процент скидки (текущий: " + std::to_string(discount.percentage) + "%, -1 для сохранения): ", -1, 100.0);
    if (newPercentage >= 0)
    {
        discount.percentage = newPercentage;
    }

    std::string newStartDate = InputValidation::getValidatedDate("Новая дата начала (текущая: " + discount.startDate + ", оставьте пустым для сохранения): ");
    if (!newStartDate.empty()) 
    {
        discount.startDate = newStartDate;
    }

    std::string newEndDate = InputValidation::getValidatedDate("Новая дата окончания (текущая: " + discount.endDate + ", оставьте пустым для сохранения): ");
    if (!newEndDate.empty()) 
    {
        discount.endDate = newEndDate;
    }

    if (discount.startDate >= discount.endDate) 
    {
        TableFormatter::displayErrorMessage("Дата окончания должна быть позже даты начала!");
        TableFormatter::pause();
        return;
    }

    store.saveDiscounts();
    TableFormatter::displaySuccessMessage("Скидка успешно обновлена!");
    logger->log(LogLevel::INFO, "Discount edited: " + discount.type + " " + std::to_string(discount.percentage) + "%");
    TableFormatter::pause();
}

void AdminMenu::deleteDiscount() 
{
    displayHeader("УДАЛЕНИЕ СКИДКИ");

    auto discounts = store.getAllDiscounts();

    if (discounts.empty()) 
    {
        TableFormatter::displayWarningMessage("Нет скидок для удаления.");
        TableFormatter::pause();
        return;
    }

    TableFormatter::displayDiscountsTable(discounts);

    int discountIndex = InputValidation::getValidatedNumber("Введите номер скидки для удаления (0 для отмены): ", 0, discounts.size());

    if (discountIndex == 0) 
    {
        TableFormatter::displayWarningMessage("Удаление отменено.");
        TableFormatter::pause();
        return;
    }

    const Discount& discount = discounts[discountIndex - 1];

    TableFormatter::displaySuccessMessage("Информация о скидке для удаления:");
    std::cout << "Тип: " << discount.type << std::endl;
    std::cout << "Цели: ";
    for (size_t i = 0; i < discount.targets.size(); ++i)
    {
        std::cout << discount.targets[i];
        if (i < discount.targets.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "Скидка: " << discount.percentage << "%" << std::endl;
    std::cout << "Период: " << discount.startDate << " - " << discount.endDate << std::endl;
    std::cout << std::endl;

    if (!confirmAction("Вы уверены, что хотите удалить эту скидку?")) 
    {
        TableFormatter::displayWarningMessage("Удаление отменено.");
        TableFormatter::pause();
        return;
    }

    auto updatedDiscounts = discounts;
    updatedDiscounts.erase(updatedDiscounts.begin() + discountIndex - 1);

    FileManager fileManager;
    if (fileManager.saveToFile(Config::DISCOUNTS_FILE, updatedDiscounts)) 
    {
        TableFormatter::displaySuccessMessage("Скидка успешно удалена!");
        logger->log(LogLevel::INFO, "Discount deleted: " + discount.type + " " + std::to_string(discount.percentage) + "%");

        store.loadData();
    }
    else 
    {
        TableFormatter::displayErrorMessage("Ошибка при удалении скидки!");
    }

    TableFormatter::pause();
}

void AdminMenu::viewDiscountedProducts() 
{
    displayHeader("ТОВАРЫ СО СКИДКАМИ");

    auto products = store.getAllProducts();
    std::vector<std::shared_ptr<Product>> discountedProducts;
    std::vector<double> discountedPrices;

    for (const auto& product : products)
    {
        double discountedPrice = store.getDiscountedPrice(product);
        if (discountedPrice < product->getBasePrice()) 
        {
            discountedProducts.push_back(product);
            discountedPrices.push_back(discountedPrice);
        }
    }

    if (discountedProducts.empty()) 
    {
        TableFormatter::displayWarningMessage("В данный момент нет товаров со скидками.");
    }
    else 
    {
        TableFormatter::displayDiscountedProducts(discountedProducts, discountedPrices);
    }

    TableFormatter::pause();
}

void AdminMenu::viewReports() 
{
    int currentPage = 1;
    const int reportsPerPage = 5;

    while (true)
    {
        displayHeader("ПРОСМОТР ОТЧЕТОВ");

        std::vector<std::string> options = 
        {
            "Отчет по продажам",
            "Финансовый отчет",
            "Отчет по остаткам ключей",
            "Загрузить отчет из файла"
        };

        TableFormatter::displayReportMenu(options, "ГЕНЕРАЦИЯ ОТЧЕТОВ");

        int choice = InputValidation::getValidatedNumber("Выберите тип отчета: ", 0, 4);

        switch (choice) 
        {
        case 1: 
        {
            displayHeader("ОТЧЕТ ПО ПРОДАЖАМ");
            std::string startDate = InputValidation::getValidatedDate("Введите начальную дату (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");
            std::string endDate = InputValidation::getValidatedDate("Введите конечную дату (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");

            if (startDate >= endDate) 
            {
                TableFormatter::displayErrorMessage("Начальная дата должна быть раньше конечной!");
                TableFormatter::pause();
                break;
            }

            auto salesReport = report.generateSalesReport(startDate, endDate);
            TableFormatter::displaySalesReport(salesReport, store);

            if (confirmAction("Сохранить отчет в файл?")) 
            {
                std::string filename = "sales_report_" + Config::getCurrentDateTime() + ".txt";
                std::replace(filename.begin(), filename.end(), ':', '-'); 
                report.saveReportToFile(salesReport, filename);
                TableFormatter::displaySuccessMessage("Отчет сохранен в файл: " + filename);
            }

            TableFormatter::pause();
            break;
        }
        case 2:
        {
            displayHeader("ФИНАНСОВЫЙ ОТЧЕТ");
            std::string startDate = InputValidation::getValidatedDate("Введите начальную дату (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");
            std::string endDate = InputValidation::getValidatedDate("Введите конечную дату (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");

            if (startDate >= endDate) 
            {
                TableFormatter::displayErrorMessage("Начальная дата должна быть раньше конечной!");
                TableFormatter::pause();
                break;
            }

            auto financialReport = report.generateFinancialReport(startDate, endDate);
            TableFormatter::displayFinancialReport(financialReport);

            if (confirmAction("Сохранить отчет в файл?")) 
            {
                std::string filename = "financial_report_" + Config::getCurrentDateTime() + ".txt";
                std::replace(filename.begin(), filename.end(), ':', '-');
                report.saveReportToFile(financialReport, filename);
                TableFormatter::displaySuccessMessage("Отчет сохранен в файл: " + filename);
            }

            TableFormatter::pause();
            break;
        }
        case 3:
            report.generateInventoryReport();
            TableFormatter::pause();
            break;
        case 4: 
        {
            displayHeader("ЗАГРУЗКА ОТЧЕТА ИЗ ФАЙЛА");
            std::string filename = InputValidation::getValidatedString("Введите имя файла: ", 1, 100);
            try 
            {
                report.loadReportInConsole(filename);
            }
            catch (const FileIOException& e) 
            {
                TableFormatter::displayErrorMessage(e.what());
            }
            TableFormatter::pause();
            break;
        }
        case 0:
            return;
        }
    }
}

void AdminMenu::viewLogs() 
{
    int currentPage = 1;
    const int logsPerPage = 10;

    std::ifstream logFile(Config::LOG_FILE);
    std::vector<std::string> logs;
    std::string line;

    if (logFile.is_open()) 
    {
        while (std::getline(logFile, line)) 
        {
            logs.push_back(line);
        }
        logFile.close();
    }
    else 
    {
        TableFormatter::displayErrorMessage("Не удалось открыть файл логов!");
        TableFormatter::pause();
        return;
    }

    std::reverse(logs.begin(), logs.end());

    int totalPages = (logs.size() + logsPerPage - 1) / logsPerPage;
    if (totalPages == 0) totalPages = 1;

    while (true) 
    {
        displayHeader("ПРОСМОТР СИСТЕМНЫХ ЛОГОВ");

        TableFormatter::displayLogsTable(logs, currentPage, totalPages, logs.size());

        std::vector<std::string> options =
        {
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Обновить логи",
            "Очистить логи"
        };

        TableFormatter::displayMenu(options, "УПРАВЛЕНИЕ ЛОГАМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 5);

        switch (choice)
        {
        case 1:
            if (currentPage < totalPages) currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 2:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 3:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
                TableFormatter::pause();
            }
            break;
        case 4:
            logs.clear();
            logFile.open(Config::LOG_FILE);
            if (logFile.is_open()) 
            {
                while (std::getline(logFile, line)) 
                {
                    logs.push_back(line);
                }
                logFile.close();
            }
            std::reverse(logs.begin(), logs.end());
            totalPages = (logs.size() + logsPerPage - 1) / logsPerPage;
            if (totalPages == 0) totalPages = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            TableFormatter::displaySuccessMessage("Логи обновлены!");
            break;
        case 5:
            if (confirmAction("Вы уверены, что хотите очистить все логи? Это действие необратимо!")) 
            {
                std::ofstream clearFile(Config::LOG_FILE, std::ios::trunc);
                clearFile.close();
                logs.clear();
                totalPages = 1;
                currentPage = 1;
                TableFormatter::displaySuccessMessage("Логи очищены!");
                TableFormatter::pause();
            }
            break;
        case 0:
            return;
        }
    }
}

void AdminMenu::changeMyProfile()
{
    TableFormatter::clearScreen();
    if (auto admin = std::dynamic_pointer_cast<Admin>(currentUser)) 
    {
        admin->displayProfile();

        std::vector<std::string> options = 
        {
            "Изменить имя пользователя",
            "Изменить email",
            "Изменить телефон",
            "Изменить пароль"
        };

        TableFormatter::displayMenu(options, "РЕДАКТИРОВАНИЕ ПРОФИЛЯ АДМИНИСТРАТОРА");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 4);

        switch (choice) 
        {
        case 1:
            changeAdminUsername();
            break;
        case 2:
            changeAdminEmail();
            break;
        case 3:
            changeAdminPhone();
            break;
        case 4:
            changeAdminPassword();
            break;
        }
    }
    TableFormatter::pause();
}

void AdminMenu::changeAdminUsername() 
{
    std::string newUsername = InputValidation::getValidatedString("Введите новое имя пользователя: ", 3, 20);

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (const auto& user : users) 
    {
        if (user->getUsername() == newUsername && user->getId() != currentUser->getId())
        {
            TableFormatter::displayErrorMessage("Имя пользователя уже занято!");
            TableFormatter::pause();
            return;
        }
    }

    for (auto& user : users) 
    {
        if (user->getId() == currentUser->getId()) 
        {
            std::string oldUsername = user->getUsername();
            user->setUsername(newUsername);
            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Имя пользователя успешно изменено!");
                currentUser->setUsername(newUsername);
                logger->log(LogLevel::INFO, "Admin username changed: " + oldUsername + " -> " + newUsername);
            }
            return;
        }
    }
}

void AdminMenu::changeAdminEmail() 
{
    std::string newEmail = InputValidation::getValidatedEmail("Введите новый email: ");

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users) 
    {
        if (user->getId() == currentUser->getId()) 
        {
            std::string oldEmail = user->getEmail();
            user->setEmail(newEmail);
            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Email успешно изменен!");
                currentUser->setEmail(newEmail);
                logger->log(LogLevel::INFO, "Admin email changed: " + oldEmail + " -> " + newEmail);
            }
            return;
        }
    }
}

void AdminMenu::changeAdminPhone()
{
    std::string newPhone = InputValidation::getValidatedPhone("Введите новый телефон: ");

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users) 
    {
        if (user->getId() == currentUser->getId()) 
        {
            std::string oldPhone = user->getPhone();
            user->setPhone(newPhone);
            if (fileManager.saveToFile(Config::USERS_FILE, users))
            {
                TableFormatter::displaySuccessMessage("Телефон успешно изменен!");
                currentUser->setPhone(newPhone);
                logger->log(LogLevel::INFO, "Admin phone changed: " + oldPhone + " -> " + newPhone);
            }
            return;
        }
    }
}

void AdminMenu::changeAdminPassword() 
{
    std::cout << "Текущий пароль: ";
    std::string currentPassword = InputValidation::maskPassword();

    std::string storedData = currentUser->getPasswordHash();
    size_t separatorPos = storedData.find(':');
    if (separatorPos == std::string::npos) 
    {
        TableFormatter::displayErrorMessage("Ошибка формата пароля!");
        return;
    }

    std::string storedSalt = storedData.substr(0, separatorPos);
    std::string storedHash = storedData.substr(separatorPos + 1);
    std::string inputHash = InputValidation::hashPassword(currentPassword, storedSalt);

    if (inputHash != storedHash) 
    {
        TableFormatter::displayErrorMessage("Неверный текущий пароль!");
        TableFormatter::pause();
        return;
    }

    std::cout << "Новый пароль: ";
    std::string newPassword = InputValidation::maskPassword();
    std::cout << "Подтвердите новый пароль: ";
    std::string confirmPassword = InputValidation::maskPassword();

    if (newPassword != confirmPassword) 
    {
        TableFormatter::displayErrorMessage("Пароли не совпадают!");
        TableFormatter::pause();
        return;
    }

    std::string newSalt = InputValidation::generateSalt();
    std::string newHashedPassword = InputValidation::hashPassword(newPassword, newSalt);
    std::string newStoredPassword = newSalt + ":" + newHashedPassword;

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users)
    {
        if (user->getId() == currentUser->getId()) 
        {
            user->setPasswordHash(newStoredPassword);
            if (fileManager.saveToFile(Config::USERS_FILE, users))
            {
                TableFormatter::displaySuccessMessage("Пароль успешно изменен!");
                currentUser->setPasswordHash(newStoredPassword);
                logger->log(LogLevel::INFO, "Admin password changed for user: " + currentUser->getUsername());
            }
            return;
        }
    }
}

void AdminMenu::viewNotifications() 
{
    displayHeader("УВЕДОМЛЕНИЯ АДМИНИСТРАТОРА");

    notificationSystem.displayAdminNotifications();

    auto notifications = notificationSystem.getAdminNotifications();

    if (!notifications.empty()) 
    {
        std::vector<std::string> options =
        {
            "Пометить все уведомления как прочитанные",
            "Очистить все уведомления"
        };

        TableFormatter::displayMenu(options, "ДЕЙСТВИЯ С УВЕДОМЛЕНИЯМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 2);

        switch (choice) 
        {
        case 1:
            notificationSystem.markAdminNotificationsAsRead();
            TableFormatter::displaySuccessMessage("Уведомления помечены как прочитанные!");
            break;
        case 2:
            if (confirmAction("Вы уверены, что хотите очистить все уведомления? Это действие необратимо!")) 
            {
                notificationSystem.clearAdminNotifications();
                TableFormatter::displaySuccessMessage("Уведомления очищены!");
            }
            break;
        case 0:
            return;
        }
    }

    TableFormatter::pause();
}

CustomerMenu::CustomerMenu(Store& s, Wishlist& w, RecommendationSystem& rs, Report& r, NotificationSystem& ns, std::shared_ptr<User> user)
    : Menu(s, w, rs, r, ns) 
{
    currentUser = user;
}

bool CustomerMenu::confirmAction(const std::string& message) 
{
    Config::setColor(Config::COLOR_WARNING);
    std::cout << message << std::endl;
    Config::resetColor();

    int choice = InputValidation::getValidatedNumber("Введите 1 для подтверждения или 0 для отмены: ", 0, 1);
    return (choice == 1);
}

void CustomerMenu::show() 
{
    if (!currentUser->getIsApproved()) 
    {
        showLimitedMenu();
        return;
    }
    while (true) 
    {
        displayHeader("ЛИЧНЫЙ КАБИНЕТ");

        std::vector<std::string> options = 
        {
            "Просмотр каталога",
            "Поиск игр",
            "Мои покупки",
            "Список желаемого",
            "Рекомендации",
            "Мой профиль",
            "Уведомления"
        };

        TableFormatter::displayMenu(options, "ПОКУПАТЕЛЬ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 7);

        switch (choice) 
        {
        case 1:
            viewCatalog();
            break;
        case 2:
            searchGames();
            break;
        case 3:
            viewPurchaseHistory();
            break;
        case 4:
            manageWishlist();
            break;
        case 5:
            viewRecommendations();
            break;
        case 6:
            viewProfile();
            break;
        case 7:
            viewNotifications();
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::showLimitedMenu() 
{
    while (true) 
    {
        displayHeader("ЛИЧНЫЙ КАБИНЕТ (ОЖИДАЕТ ПОДТВЕРЖДЕНИЯ)");

        std::vector<std::string> options = 
        {
            "Просмотр каталога",
            "Поиск игр",
            "Мой профиль",
        };

        TableFormatter::displayMenu(options, "ОГРАНИЧЕННЫЙ ДОСТУП");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 3);

        switch (choice) 
        {
        case 1:
            viewCatalogLimited();
            break;
        case 2:
            searchGamesLimited();
            break;
        case 3:
            viewProfile();
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewCatalogLimited() 
{
    int currentPage = 1;
    const int productsPerPage = 5;

    while (true) 
    {
        displayHeader("КАТАЛОГ ИГР (ТОЛЬКО ПРОСМОТР)");

        auto allProducts = store.getAllProducts();
        int totalPages = (allProducts.size() + productsPerPage - 1) / productsPerPage;
        if (totalPages == 0) totalPages = 1;

        TableFormatter::displayProductsTableWithPagination(allProducts, currentPage, totalPages, allProducts.size());

        std::vector<std::string> options = 
        {
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Просмотреть детали игры",
        };

        TableFormatter::displayMenu(options, "ПРОСМОТР КАТАЛОГА");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 4);

        switch (choice) 
        {
        case 1:
            if (currentPage < totalPages) currentPage++;
            break;
        case 2:
            if (currentPage > 1) currentPage--;
            break;
        case 3:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            break;
        case 4:
            viewProductDetailsLimited();
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewProductDetailsLimited() 
{
    int productId = InputValidation::getValidatedNumber("Введите ID товара для просмотра: ", 1, 10000);

    auto product = store.getProductById(productId);
    if (product)
    {
        TableFormatter::displayProductDetails(product);
        TableFormatter::displayWarningMessage("Для покупки и добавления в список желаемого необходимо дождаться одобрения аккаунта администратором.");
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
    }
    TableFormatter::pause();
}

void CustomerMenu::searchGamesLimited()
{
    int currentPage = 1;
    const int productsPerPage = 5;
    std::vector<std::shared_ptr<Product>> searchResults;
    std::string searchType = "all";
    std::string searchValue = "";

    while (true)
    {
        displayHeader("ПОИСК И ФИЛЬТРАЦИЯ ИГР (ТОЛЬКО ПРОСМОТР)");

        if (!searchResults.empty()) 
        {
            int totalPages = (searchResults.size() + productsPerPage - 1) / productsPerPage;
            if (totalPages == 0) totalPages = 1;

            if (currentPage > totalPages) currentPage = totalPages;
            if (currentPage < 1) currentPage = 1;

            std::cout << "Текущий фильтр: ";
            if (searchValue.empty())
            {
                std::cout << "не установлен" << std::endl;
            }
            else 
            {
                std::cout << searchType << " = '" << searchValue << "'" << std::endl;
            }
            std::cout << "Найдено игр: " << searchResults.size() << std::endl << std::endl;

            TableFormatter::displayProductsTableWithPagination(searchResults, currentPage, totalPages, searchResults.size());
        }
        else
        {
            TableFormatter::displayWarningMessage("Нет результатов поиска. Используйте опции поиска для настройки фильтров.");
        }

        std::vector<std::string> options = 
        {
            "Поиск по названию",
            "Фильтр по жанру",
            "Фильтр по разработчику",
            "Фильтр по тегу",
            "Только со скидками",
            "Только в наличии",
            "Сбросить фильтры",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Просмотреть детали игры"
        };

        TableFormatter::displayMenu(options, "ПОИСК И ФИЛЬТРАЦИЯ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 11);

        switch (choice) 
        {
        case 1: 
        {
            std::string query = InputValidation::getValidatedString("Введите название для поиска: ", 1, 100);
            searchResults = store.searchProducts(query);
            searchType = "title";
            searchValue = query;
            currentPage = 1;
            break;
        }
        case 2: 
        {
            auto genres = store.getUniqueGenres();
            if (genres.empty())
            {
                TableFormatter::displayWarningMessage("Нет доступных жанров.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные жанры:" << std::endl;
            for (size_t i = 0; i < genres.size(); ++i) 
            {
                std::cout << i + 1 << ". " << genres[i] << std::endl;
            }

            int genreChoice = InputValidation::getValidatedNumber("Выберите жанр: ", 1, genres.size());
            searchResults = store.filterProducts("genre", genres[genreChoice - 1]);
            searchType = "genre";
            searchValue = genres[genreChoice - 1];
            currentPage = 1;
            break;
        }
        case 3: 
        {
            auto developers = store.getUniqueDevelopers();
            if (developers.empty()) 
            {
                TableFormatter::displayWarningMessage("Нет доступных разработчиков.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные разработчики:" << std::endl;
            for (size_t i = 0; i < developers.size(); ++i)
            {
                std::cout << i + 1 << ". " << developers[i] << std::endl;
            }

            int devChoice = InputValidation::getValidatedNumber("Выберите разработчика: ", 1, developers.size());
            searchResults = store.filterProducts("developer", developers[devChoice - 1]);
            searchType = "developer";
            searchValue = developers[devChoice - 1];
            currentPage = 1;
            break;
        }
        case 4: 
        {
            FileManager fileManager;
            auto tags = fileManager.loadStringList(Config::TAGS_FILE);
            if (tags.empty())
            {
                TableFormatter::displayWarningMessage("Нет доступных тегов.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные теги:" << std::endl;
            for (size_t i = 0; i < tags.size(); ++i)
            {
                std::cout << i + 1 << ". " << tags[i] << std::endl;
            }

            int tagChoice = InputValidation::getValidatedNumber("Выберите тег: ", 1, tags.size());
            searchResults = store.filterProducts("tag", tags[tagChoice - 1]);
            searchType = "tag";
            searchValue = tags[tagChoice - 1];
            currentPage = 1;
            break;
        }
        case 5:
            searchResults = store.filterProducts("has_discount", "");
            searchType = "has_discount";
            searchValue = "";
            currentPage = 1;
            break;
        case 6:
            searchResults = store.filterProducts("in_stock", "");
            searchType = "in_stock";
            searchValue = "";
            currentPage = 1;
            break;
        case 7:
            searchResults.clear();
            searchType = "all";
            searchValue = "";
            currentPage = 1;
            break;
        case 8:
            if (currentPage < (searchResults.size() + productsPerPage - 1) / productsPerPage)
                currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 9:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 10:
            if (!searchResults.empty())
            {
                int totalPages = (searchResults.size() + productsPerPage - 1) / productsPerPage;
                if (totalPages > 1) 
                {
                    int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                    currentPage = newPage;
                }
                else 
                {
                    TableFormatter::displayWarningMessage("Доступна только одна страница!");
                    TableFormatter::pause();
                }
            }
            break;
        case 11:
            if (!searchResults.empty()) 
            {
                viewProductDetailsLimited();
            }
            else 
            {
                TableFormatter::displayWarningMessage("Нет результатов для просмотра.");
                TableFormatter::pause();
            }
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewCatalog() 
{
    int currentPage = 1;
    const int productsPerPage = 5;

    while (true) 
    {
        displayHeader("КАТАЛОГ ИГР");

        auto allProducts = store.getAllProducts();
        int totalPages = (allProducts.size() + productsPerPage - 1) / productsPerPage;
        if (totalPages == 0) totalPages = 1;

        TableFormatter::displayProductsTableWithPagination(allProducts, currentPage, totalPages, allProducts.size());

        std::vector<std::string> options = 
        {
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Поиск игр",
            "Просмотреть детали игры",
            "Добавить в список желаемого",
            "Купить игру"
        };

        TableFormatter::displayMenu(options, "ПРОСМОТР КАТАЛОГА");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 7);

        switch (choice) 
        {
        case 1:
            if (currentPage < totalPages) currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 2:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 3:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
                TableFormatter::pause();
            }
            break;
        case 4:
            searchGames();
            break;
        case 5:
            viewProductDetails();
            break;
        case 6:
            addToWishlistFromCatalog();
            break;
        case 7:
            purchaseFromCatalog();
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewProductDetails() 
{
    TableFormatter::clearScreen();
    displayHeader("ПРОСМОТР ДЕТАЛЕЙ ТОВАРА");

    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5;
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для просмотра (0 для отмены): ", 0, 10000);
    if (productId == 0) 
    {
        TableFormatter::displayWarningMessage("Просмотр отменен.");
        TableFormatter::pause();
        return;
    }

    auto product = store.getProductById(productId);
    if (product) 
    {
        TableFormatter::displayProductDetails(product);

        std::vector<std::string> options = 
        {
            "Добавить в список желаемого",
            "Купить эту игру"
        };

        TableFormatter::displayMenu(options, "ДОПОЛНИТЕЛЬНЫЕ ОПЦИИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 2);

        switch (choice) 
        {
        case 1:
            if (wishlist.addToWishlist(currentUser->getId(), productId)) 
            {
                TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Товар уже в списке желаемого!");
            }
            break;
        case 2:
            purchaseProduct(product);
            break;
        case 0:
            return;
        }
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
    }
    TableFormatter::pause();
}

void CustomerMenu::addToWishlistFromCatalog() 
{
    TableFormatter::clearScreen();
    displayHeader("ДОБАВЛЕНИЕ В СПИСОК ЖЕЛАЕМОГО");

    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5;
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для добавления в список желаемого (0 для отмены): ", 1, 10000);
    if (productId == 0) {
        TableFormatter::displayWarningMessage("Добавление отменено.");
        TableFormatter::pause();
        return;
    }

    if (wishlist.addToWishlist(currentUser->getId(), productId)) 
    {
        TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар уже в списке желаемого или не найден!");
    }
    TableFormatter::pause();
}

void CustomerMenu::purchaseFromCatalog() 
{
    TableFormatter::clearScreen();
    displayHeader("ПРОСМОТР ТОВАРОВ");
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5;
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для покупки (0 для отмены): ", 1, 10000);
    if (productId == 0) 
    {
        TableFormatter::displayWarningMessage("Просмотр отменен.");
        TableFormatter::pause();
        return;
    }

    auto product = store.getProductById(productId);
    if (product) 
    {
        purchaseProduct(product);
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден!");
        TableFormatter::pause();
    }
}

void CustomerMenu::purchaseProduct(std::shared_ptr<Product> product) 
{
    if (!product->hasKeys()) 
    {
        TableFormatter::displayErrorMessage("Этот товар временно отсутствует в наличии!");
        TableFormatter::displayWarningMessage("Вы можете добавить его в список желаемого и получить уведомление, когда он появится.");

        if (InputValidation::getValidatedNumber("Добавить в список желаемого? (1 - да, 0 - нет): ", 0, 1) == 1) 
        {
            wishlist.addToWishlist(currentUser->getId(), product->getId());
            TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
        }
        TableFormatter::pause();
        return;
    }

    TableFormatter::displayProductDetails(product);

    double discountedPrice = store.getDiscountedPrice(product);
    if (discountedPrice < product->getBasePrice())
    {
        double discountPercent = ((product->getBasePrice() - discountedPrice) / product->getBasePrice()) * 100;
        Config::setColor(Config::COLOR_SUCCESS);
        std::cout << "Цена со скидкой: " << discountedPrice << " руб. (скидка "
            << std::fixed << std::setprecision(1) << discountPercent << "%)" << std::endl;
        Config::resetColor();
    }

    int quantity = InputValidation::getValidatedNumber("Введите количество: ", 1, product->getActivationKeys());

    if (InputValidation::getValidatedNumber("Подтвердить покупку? (1 - да, 0 - нет): ", 0, 1) == 1)
    {
        try 
        {
            std::vector<std::pair<int, int>> items = { {product->getId(), quantity} };
            if (store.processSale(currentUser->getId(), items)) 
            {
                TableFormatter::displaySuccessMessage("Покупка завершена успешно!");
                TableFormatter::displaySuccessMessage("Ключ активации отправлен на ваш email: " + currentUser->getEmail());

                if (auto customer = std::dynamic_pointer_cast<Customer>(currentUser)) 
                {
                    customer->addPurchase(discountedPrice * quantity);
                    customer->setLastPurchasedProductId(product->getId());
                }

                notificationSystem.notifyPurchaseComplete(currentUser->getId(), product->getTitle());
            }
        }
        catch (const BusinessLogicError& e) 
        {
            TableFormatter::displayErrorMessage(e.what());
        }
    }
    else 
    {
        TableFormatter::displayWarningMessage("Покупка отменена.");
    }
    TableFormatter::pause();
}

void CustomerMenu::searchGames() 
{
    int currentPage = 1;
    const int productsPerPage = 5;
    std::vector<std::shared_ptr<Product>> searchResults;
    std::string searchType = "all";
    std::string searchValue = "";

    while (true) 
    {
        displayHeader("ПОИСК И ФИЛЬТРАЦИЯ ИГР");

        if (!searchResults.empty()) 
        {
            int totalPages = (searchResults.size() + productsPerPage - 1) / productsPerPage;
            if (totalPages == 0) totalPages = 1;

            if (currentPage > totalPages) currentPage = totalPages;
            if (currentPage < 1) currentPage = 1;

            std::cout << "Текущий фильтр: ";
            if (searchValue.empty()) 
            {
                std::cout << "не установлен" << std::endl;
            }
            else 
            {
                std::cout << searchType << " = '" << searchValue << "'" << std::endl;
            }
            std::cout << "Найдено игр: " << searchResults.size() << std::endl << std::endl;

            TableFormatter::displayProductsTableWithPagination(searchResults, currentPage, totalPages, searchResults.size());
        }
        else 
        {
            TableFormatter::displayWarningMessage("Нет результатов поиска. Используйте опции поиска для настройки фильтров.");
        }

        std::vector<std::string> options = 
        {
            "Поиск по названию",
            "Фильтр по жанру",
            "Фильтр по разработчику",
            "Фильтр по тегу",
            "Только со скидками",
            "Только в наличии",
            "Сбросить фильтры",
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Просмотреть детали игры",
            "Добавить в список желаемого",
            "Купить игру"
        };

        TableFormatter::displayMenu(options, "ПОИСК И ФИЛЬТРАЦИЯ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 13);

        switch (choice) 
        {
        case 1: {
            std::string query = InputValidation::getValidatedString("Введите название для поиска: ", 1, 100);
            searchResults = store.searchProducts(query);
            searchType = "title";
            searchValue = query;
            currentPage = 1;
            break;
        }
        case 2: 
        {
            auto genres = store.getUniqueGenres();
            if (genres.empty()) 
            {
                TableFormatter::displayWarningMessage("Нет доступных жанров.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные жанры:" << std::endl;
            for (size_t i = 0; i < genres.size(); ++i)
            {
                std::cout << i + 1 << ". " << genres[i] << std::endl;
            }

            int genreChoice = InputValidation::getValidatedNumber("Выберите жанр: ", 1, genres.size());
            searchResults = store.filterProducts("genre", genres[genreChoice - 1]);
            searchType = "genre";
            searchValue = genres[genreChoice - 1];
            currentPage = 1;
            break;
        }
        case 3: 
        {
            auto developers = store.getUniqueDevelopers();
            if (developers.empty()) 
            {
                TableFormatter::displayWarningMessage("Нет доступных разработчиков.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные разработчики:" << std::endl;
            for (size_t i = 0; i < developers.size(); ++i) 
            {
                std::cout << i + 1 << ". " << developers[i] << std::endl;
            }

            int devChoice = InputValidation::getValidatedNumber("Выберите разработчика: ", 1, developers.size());
            searchResults = store.filterProducts("developer", developers[devChoice - 1]);
            searchType = "developer";
            searchValue = developers[devChoice - 1];
            currentPage = 1;
            break;
        }
        case 4: 
        {
            FileManager fileManager;
            auto tags = fileManager.loadStringList(Config::TAGS_FILE);
            if (tags.empty()) 
            {
                TableFormatter::displayWarningMessage("Нет доступных тегов.");
                TableFormatter::pause();
                break;
            }

            std::cout << "Доступные теги:" << std::endl;
            for (size_t i = 0; i < tags.size(); ++i) 
            {
                std::cout << i + 1 << ". " << tags[i] << std::endl;
            }

            int tagChoice = InputValidation::getValidatedNumber("Выберите тег: ", 1, tags.size());
            searchResults = store.filterProducts("tag", tags[tagChoice - 1]);
            searchType = "tag";
            searchValue = tags[tagChoice - 1];
            currentPage = 1;
            break;
        }
        case 5:
            searchResults = store.filterProducts("has_discount", "");
            searchType = "has_discount";
            searchValue = "";
            currentPage = 1;
            break;
        case 6:
            searchResults = store.filterProducts("in_stock", "");
            searchType = "in_stock";
            searchValue = "";
            currentPage = 1;
            break;
        case 7:
            searchResults.clear();
            searchType = "all";
            searchValue = "";
            currentPage = 1;
            break;
        case 8:
            if (currentPage < (searchResults.size() + productsPerPage - 1) / productsPerPage)
                currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            TableFormatter::pause();
            break;
        case 9:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            TableFormatter::pause();
            break;
        case 10:
            if (!searchResults.empty()) 
            {
                int totalPages = (searchResults.size() + productsPerPage - 1) / productsPerPage;
                if (totalPages > 1) 
                {
                    int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                    currentPage = newPage;
                }
                else 
                {
                    TableFormatter::displayWarningMessage("Доступна только одна страница!");
                    TableFormatter::pause();
                }
            }
            break;
        case 11:
            if (!searchResults.empty())
            {
                viewProductDetailsFromSearch(searchResults);
            }
            else 
            {
                TableFormatter::displayWarningMessage("Нет результатов для просмотра.");
                TableFormatter::pause();
            }
            break;
        case 12:
            if (!searchResults.empty()) 
            {
                addToWishlistFromSearch();
            }
            else 
            {
                TableFormatter::displayWarningMessage("Нет результатов для добавления.");
                TableFormatter::pause();
            }
            break;
        case 13:
            if (!searchResults.empty()) 
            {
                purchaseFromSearch(searchResults);
            }
            else 
            {
                TableFormatter::displayWarningMessage("Нет результатов для покупки.");
                TableFormatter::pause();
            }
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewProductDetailsFromSearch(const std::vector<std::shared_ptr<Product>>& searchResults) {
    int productId = InputValidation::getValidatedNumber("Введите ID товара для просмотра: ", 1, 10000);

    bool found = false;
    std::shared_ptr<Product> product;
    for (const auto& p : searchResults) 
    {
        if (p->getId() == productId) 
        {
            product = p;
            found = true;
            break;
        }
    }

    if (found && product) 
    {
        TableFormatter::displayProductDetails(product);
        TableFormatter::pause();
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден в результатах поиска!");
        TableFormatter::pause();
    }
}

void CustomerMenu::addToWishlistFromSearch() 
{
    int productId = InputValidation::getValidatedNumber("Введите ID товара для добавления в список желаемого: ", 1, 10000);

    if (wishlist.addToWishlist(currentUser->getId(), productId)) 
    {
        TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар уже в списке желаемого или не найден!");
    }
    TableFormatter::pause();
}

void CustomerMenu::purchaseFromSearch(const std::vector<std::shared_ptr<Product>>& searchResults) 
{
    int productId = InputValidation::getValidatedNumber("Введите ID товара для покупки: ", 1, 10000);

    bool found = false;
    std::shared_ptr<Product> product;
    for (const auto& p : searchResults)
    {
        if (p->getId() == productId) 
        {
            product = p;
            found = true;
            break;
        }
    }

    if (found && product) 
    {
        purchaseProduct(product);
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден в результатах поиска!");
        TableFormatter::pause();
    }
}

void CustomerMenu::viewPurchaseHistory()
{
    int currentPage = 1;
    const int salesPerPage = 5;

    auto allSales = store.getAllSales();
    std::vector<Sale> userSales;

    for (const auto& sale : allSales) 
    {
        if (sale.getCustomerId() == currentUser->getId()) 
        {
            userSales.push_back(sale);
        }
    }

    if (userSales.empty())
    {
        displayHeader("ИСТОРИЯ ПОКУПОК");
        TableFormatter::displayWarningMessage("У вас пока нет покупок.");
        TableFormatter::pause();
        return;
    }

    int totalPages = (userSales.size() + salesPerPage - 1) / salesPerPage;
    if (totalPages == 0) totalPages = 1;

    while (true) 
    {
        displayHeader("ИСТОРИЯ ПОКУПОК");

        TableFormatter::displaySalesTableWithPagination(userSales, currentPage, totalPages, userSales.size());

        std::vector<std::string> options = 
        {
            "Следующая страница",
            "Предыдущая страница",
            "Перейти к странице",
            "Просмотреть детали продажи"
        };

        TableFormatter::displayMenu(options, "ИСТОРИЯ ПОКУПОК");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 4);

        switch (choice) 
        {
        case 1:
            if (currentPage < totalPages) currentPage++;
            else TableFormatter::displayWarningMessage("Вы уже на последней странице!");
            break;
        case 2:
            if (currentPage > 1) currentPage--;
            else TableFormatter::displayWarningMessage("Вы уже на первой странице!");
            break;
        case 3:
            if (totalPages > 1) 
            {
                int newPage = InputValidation::getValidatedNumber("Введите номер страницы: ", 1, totalPages);
                currentPage = newPage;
            }
            else 
            {
                TableFormatter::displayWarningMessage("Доступна только одна страница!");
            }
            break;
        case 4:
            viewSaleDetails(userSales);
            break;
        case 0:
            return;
        }
    }
}

void CustomerMenu::viewSaleDetails(const std::vector<Sale>& userSales) 
{
    int saleId = InputValidation::getValidatedNumber("Введите ID продажи для просмотра: ", 1, 10000);

    bool found = false;
    std::vector<Sale> targetSale;
    for (const auto& sale : userSales)
    {
        if (sale.getSaleId() == saleId)
        {
            found = true;
            targetSale.push_back(sale);
            break;
        }
    }

    if (found) 
    {
        displayHeader("ДЕТАЛИ ПРОДАЖИ #" + std::to_string(saleId));
        TableFormatter::displaySalesTableDetailed(targetSale, store, currentUser->getId());
    }
    else 
    {
        TableFormatter::displayErrorMessage("Продажа с указанным ID не найдена в вашей истории!");
    }

    TableFormatter::pause();
}

void CustomerMenu::manageWishlist() 
{
    auto customer = std::dynamic_pointer_cast<Customer>(currentUser);
    if (!customer) return;

    while (true) 
    {
        displayHeader("СПИСОК ЖЕЛАЕМОГО");

        auto products = store.getAllProducts();
        wishlist.displayWishlist(currentUser->getId(), products);

        std::vector<std::string> options = 
        {
            "Добавить в список желаемого",
            "Удалить из списка желаемого",
            "Очистить список желаемого"
        };

        TableFormatter::displayMenu(options, "УПРАВЛЕНИЕ СПИСКОМ ЖЕЛАЕМОГО");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 3);

        switch (choice) 
        {
        case 1: 
        {
            int productId = InputValidation::getValidatedNumber("Введите ID товара: ", 1, 1000);
            if (wishlist.addToWishlist(currentUser->getId(), productId)) 
            {
                TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Товар уже в списке желаемого или не найден!");
            }
            break;
        }
        case 2: 
        {
            int productId = InputValidation::getValidatedNumber("Введите ID товара: ", 1, 1000);
            if (wishlist.removeFromWishlist(currentUser->getId(), productId)) 
            {
                TableFormatter::displaySuccessMessage("Товар удален из списка желаемого!");
            }
            else 
            {
                TableFormatter::displayErrorMessage("Товар не найден в списке желаемого!");
            }
            break;
        }
        case 3:
            wishlist.clearWishlist(currentUser->getId());
            TableFormatter::displaySuccessMessage("Список желаемого очищен!");
            break;
        case 0:
            return;
        }
        TableFormatter::pause();
    }
}

void CustomerMenu::viewRecommendations() 
{
    auto customer = std::dynamic_pointer_cast<Customer>(currentUser);
    if (!customer) 
    {
        TableFormatter::displayErrorMessage("Ошибка: неверный тип пользователя!");
        TableFormatter::pause();
        return;
    }

    displayHeader("РЕКОМЕНДАЦИИ ДЛЯ ВАС");

    auto recommendations = recommendationSystem.getSimilarGames(customer);

    if (recommendations.empty()) 
    {
        if (customer->getLastPurchasedProductId() == -1) 
        {
            TableFormatter::displayWarningMessage("Рекомендации появятся после вашей первой покупки!");
            TableFormatter::displaySuccessMessage("А пока посмотрите наши популярные товары:");

            auto popularProducts = recommendationSystem.getPopularProducts();
            if (!popularProducts.empty()) 
            {
                TableFormatter::displayProductsTable(popularProducts);
            }
        }
        else 
        {
            TableFormatter::displayWarningMessage("К сожалению, сейчас нет подходящих рекомендаций.");
            TableFormatter::displaySuccessMessage("Посмотрите товары со скидками:");

            auto discountedProducts = recommendationSystem.getDiscountedProducts();
            if (!discountedProducts.empty()) 
            {
                TableFormatter::displayProductsTable(discountedProducts);
            }
        }
    }
    else
    {
        TableFormatter::displaySuccessMessage("Основано на ваших предыдущих покупках:");
        TableFormatter::displayProductsTable(recommendations);

        std::vector<std::string> options = 
        {
            "Добавить в список желаемого",
            "Купить рекомендованный товар"
        };

        TableFormatter::displayMenu(options, "ДОПОЛНИТЕЛЬНЫЕ ОПЦИИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 2);

        switch (choice)
        {
        case 1:
            addToWishlistFromRecommendations(recommendations);
            break;
        case 2:
            purchaseFromRecommendations(recommendations);
            break;
        case 0:
            return;
        }
    }

    TableFormatter::pause();
}

void CustomerMenu::addToWishlistFromRecommendations(const std::vector<std::shared_ptr<Product>>& recommendations) 
{
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5;
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для добавления в список желаемого: ", 1, 10000);

    bool found = false;
    for (const auto& product : recommendations) 
    {
        if (product->getId() == productId) 
        {
            found = true;
            break;
        }
    }

    if (found) 
    {
        if (wishlist.addToWishlist(currentUser->getId(), productId)) 
        {
            TableFormatter::displaySuccessMessage("Товар добавлен в список желаемого!");
        }
        else 
        {
            TableFormatter::displayErrorMessage("Товар уже в списке желаемого!");
        }
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден в рекомендациях!");
    }
}

void CustomerMenu::purchaseFromRecommendations(const std::vector<std::shared_ptr<Product>>& recommendations)
{
    auto allProducts = store.getAllProducts();
    int totalPages = (allProducts.size() + 5 - 1) / 5;
    TableFormatter::displayProductsTableWithPagination(allProducts, 1, totalPages, allProducts.size());

    int productId = InputValidation::getValidatedNumber("Введите ID товара для покупки: ", 1, 10000);

    bool found = false;
    std::shared_ptr<Product> productToPurchase;
    for (const auto& product : recommendations) 
    {
        if (product->getId() == productId) 
        {
            found = true;
            productToPurchase = product;
            break;
        }
    }

    if (found && productToPurchase) 
    {
        purchaseProduct(productToPurchase);
    }
    else 
    {
        TableFormatter::displayErrorMessage("Товар с указанным ID не найден в рекомендациях!");
        TableFormatter::pause();
    }
}

void CustomerMenu::viewProfile()
{
    TableFormatter::clearScreen();
    if (auto customer = std::dynamic_pointer_cast<Customer>(currentUser)) 
    {
        customer->displayProfile();

        std::vector<std::string> options = 
        {
            "Изменить email",
            "Изменить телефон",
            "Изменить пароль"
        };

        TableFormatter::displayMenu(options, "РЕДАКТИРОВАНИЕ ПРОФИЛЯ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 3);

        switch (choice) 
        {
        case 1:
            changeEmail();
            break;
        case 2:
            changePhone();
            break;
        case 3:
            changePassword();
            break;
        case 0:
            return;
        }
    }
    TableFormatter::pause();
}

void CustomerMenu::changeEmail() 
{
    std::string newEmail = InputValidation::getValidatedEmail("Введите новый email: ");

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users) 
    {
        if (user->getId() == currentUser->getId()) 
        {
            user->setEmail(newEmail);
            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Email успешно изменен!");
                currentUser->setEmail(newEmail);
            }
            return;
        }
    }
}

void CustomerMenu::changePhone() 
{
    std::string newPhone = InputValidation::getValidatedPhone("Введите новый телефон: ");

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users) 
    {
        if (user->getId() == currentUser->getId()) 
        {
            user->setPhone(newPhone);
            if (fileManager.saveToFile(Config::USERS_FILE, users)) 
            {
                TableFormatter::displaySuccessMessage("Телефон успешно изменен!");
                currentUser->setPhone(newPhone);
            }
            return;
        }
    }
}

void CustomerMenu::changePassword() 
{
    std::cout << "Текущий пароль: ";
    std::string currentPassword = InputValidation::maskPassword();
    std::cout << "Новый пароль: ";
    std::string newPassword = InputValidation::maskPassword();
    std::cout << "Подтвердите новый пароль: ";
    std::string confirmPassword = InputValidation::maskPassword();

    if (newPassword != confirmPassword)
    {
        TableFormatter::displayErrorMessage("Пароли не совпадают!");
        return;
    }

    std::string newSalt = InputValidation::generateSalt();
    std::string newHashedPassword = InputValidation::hashPassword(newPassword, newSalt);
    std::string newStoredPassword = newSalt + ":" + newHashedPassword;

    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    for (auto& user : users)
    {
        if (user->getId() == currentUser->getId()) 
        {
            user->setPasswordHash(newStoredPassword);
            if (fileManager.saveToFile(Config::USERS_FILE, users))
            {
                TableFormatter::displaySuccessMessage("Пароль успешно изменен!");
                currentUser->setPasswordHash(newHashedPassword);
            }
            return;
        }
    }
}

void CustomerMenu::viewNotifications() 
{
    displayHeader("ВАШИ УВЕДОМЛЕНИЯ");

    int customerId = currentUser->getId();
    notificationSystem.displayCustomerNotifications(customerId);

    auto notifications = notificationSystem.getCustomerNotifications(customerId);

    if (!notifications.empty()) 
    {
        std::vector<std::string> options = 
        {
            "Пометить все уведомления как прочитанные",
            "Очистить все уведомления"
        };

        TableFormatter::displayMenu(options, "ДЕЙСТВИЯ С УВЕДОМЛЕНИЯМИ");

        int choice = InputValidation::getValidatedNumber("Выберите действие: ", 0, 2);

        switch (choice) 
        {
        case 1:
            notificationSystem.markCustomerNotificationsAsRead(customerId);
            TableFormatter::displaySuccessMessage("Уведомления помечены как прочитанные!");
            break;
        case 2:
            if (InputValidation::getValidatedNumber("Вы уверены, что хотите очистить все уведомления? (1 - да, 0 - нет): ", 0, 1) == 1) {
                notificationSystem.clearCustomerNotifications(customerId);
                TableFormatter::displaySuccessMessage("Уведомления очищены!");
            }
            break;
        case 0:
            return;
        }
    }

    TableFormatter::pause();
}