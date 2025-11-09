#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Config.h"
#include "Store.h"
#include "Wishlist.h"
#include "RecommendationSystem.h"
#include "Report.h"
#include "Notification.h"
#include "Menu.h"
#include "Logger.h"
#include "FileManager.h"
#include "InputValidation.h"
#include "User.h"

// Функция для создания первого администратора при первом запуске
void createFirstAdmin() 
{
    FileManager fileManager;
    auto users = fileManager.loadFromFile<std::shared_ptr<User>>(Config::USERS_FILE);

    bool hasAdmin = false;
    for (const auto& user : users) 
    {
        if (user->getRole() == "admin") 
        {
            hasAdmin = true;
            break;
        }
    }

    if (!hasAdmin)
    {
        std::cout << "==========================================" << std::endl;
        std::cout << "    ПЕРВЫЙ ЗАПУСК СИСТЕМЫ GAMEHUB" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "Создание учетной записи администратора..." << std::endl;

        std::string username = InputValidation::getValidatedString("Введите имя пользователя: ", 3, 20);
        std::string email = InputValidation::getValidatedEmail();
        std::string phone = InputValidation::getValidatedPhone();

        std::cout << "Пароль: ";
        std::string password = InputValidation::maskPassword();
        std::cout << "Подтвердите пароль: ";
        std::string confirmPassword = InputValidation::maskPassword();

        if (password != confirmPassword) 
        {
            std::cout << "Пароли не совпадают! Повторите попытку." << std::endl;
            TableFormatter::pause();
            TableFormatter::clearScreen();
            createFirstAdmin(); 
            return;
        }

        std::string salt = InputValidation::generateSalt();
        std::string hashedPassword = InputValidation::hashPassword(password, salt);
        std::string storedPassword = salt + ":" + hashedPassword; 

        int adminId = 1;
        auto admin = std::make_shared<Admin>(adminId, username, storedPassword, email, phone);

        users.push_back(admin);
        fileManager.saveToFile(Config::USERS_FILE, users);

        Config::setColor(Config::COLOR_SUCCESS);
        std::cout << "Администратор успешно создан!" << std::endl;
        Config::resetColor();
        std::cout << "Нажмите любую клавишу для продолжения...";
        _getch();
    }
}


int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try 
    {
        Logger* logger = Logger::getInstance();
        logger->log(LogLevel::INFO, "Запуск системы GameHub");

        createFirstAdmin();

        FileManager fileManager;

        Store store;
        Wishlist wishlist(fileManager);  
        RecommendationSystem recommendationSystem(store);
        Report report(store);
        NotificationSystem notificationSystem;

        MainMenu mainMenu(store, wishlist, recommendationSystem, report, notificationSystem);
        mainMenu.show();

        logger->log(LogLevel::INFO, "Завершение работы системы GameHub");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        Config::setColor(Config::COLOR_ERROR);
        std::cerr << "Система будет завершена." << std::endl;
        Config::resetColor();
        return 1;
    }
    catch (...) 
    {
        std::cerr << "Неизвестная критическая ошибка!" << std::endl;
        Config::setColor(Config::COLOR_ERROR);
        std::cerr << "Система будет завершена." << std::endl;
        Config::resetColor();
        return 1;
    }

    return 0;
}