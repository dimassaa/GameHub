#include "InputValidation.h"

bool InputValidation::isValidEmail(const std::string& email) 
{
    std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, pattern);
}

bool InputValidation::isValidPhone(const std::string& phone) 
{
    std::regex pattern(R"(^\+375(25|29|33|44)\d{7}$)");
    return std::regex_match(phone, pattern);
}

bool InputValidation::isValidDate(const std::string& date) 
{
    std::regex pattern(R"(^\d{2}\.\d{2}\.\d{4} \d{2}:\d{2}:\d{2}$)");
    if (!std::regex_match(date, pattern)) return false;

    try 
    {
        int day = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(3, 2));
        int year = std::stoi(date.substr(6, 4));

        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        if (year < 1900 || year > 2100) return false;

        return true;
    }
    catch (...) 
    {
        return false;
    }
}

bool InputValidation::isNumber(const std::string& str) 
{
    if (str.empty()) return false;
    for (char c : str) 
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

std::string InputValidation::maskPassword()
{
    std::string password = "";
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b') 
        {
            if (!password.empty()) 
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}

std::string InputValidation::generateSalt()
{
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string salt = "";
    for (int i = 0; i < 16; ++i) 
    {
        salt += chars[rand() % chars.size()];
    }
    return salt;
}

std::string InputValidation::hashPassword(const std::string& password, const std::string& salt) 
{
    std::string hashed;
    for (size_t i = 0; i < password.size(); ++i)
    {
        char combined = password[i] ^ salt[i % salt.size()];
        combined = (combined + 128) % 256;
        hashed += combined;
    }

    static const char hex_digits[] = "0123456789ABCDEF";
    std::string hex_hash;
    for (char c : hashed) 
    {
        hex_hash += hex_digits[(c >> 4) & 0xF];
        hex_hash += hex_digits[c & 0xF];
    }
    return hex_hash;
}

template<typename T>
T InputValidation::getValidatedInput(const std::string& prompt, const std::string& errorMessage) 
{
    T value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            Config::setColor(Config::COLOR_ERROR);
            std::cout << errorMessage << std::endl;
            Config::resetColor();
        }
        else 
        {
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return value;
        }
    }
}

// Явные инстанциации шаблона для часто используемых типов
template int InputValidation::getValidatedInput<int>(const std::string&, const std::string&);
template double InputValidation::getValidatedInput<double>(const std::string&, const std::string&);
template std::string InputValidation::getValidatedInput<std::string>(const std::string&, const std::string&);

std::string InputValidation::getValidatedEmail(const std::string& prompt) 
{
    std::string email;
    while (true) 
    {
        std::cout << prompt;
        std::getline(std::cin, email);

        if (isValidEmail(email)) 
        {
            return email;
        }
        else 
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Неверный формат email. Пример: user@example.com" << std::endl;
            Config::resetColor();
        }
    }
}

std::string InputValidation::getValidatedPhone(const std::string& prompt) 
{
    std::string phone;
    while (true) 
    {
        std::cout << prompt;
        std::getline(std::cin, phone);

        if (isValidPhone(phone))
        {
            return phone;
        }
        else
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Неверный формат телефона. Пример: +375291234567" << std::endl;
            Config::resetColor();
        }
    }
}

std::string InputValidation::getValidatedDate(const std::string& prompt) 
{
    std::string date;
    while (true) 
    {
        std::cout << prompt;
        std::getline(std::cin, date);

        if (isValidDate(date)) 
        {
            return date;
        }
        else
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Неверный формат даты. Пример: 25.12.2023 14:30:00" << std::endl;
            Config::resetColor();
        }
    }
}

std::string InputValidation::getValidatedBirthday(const std::string& prompt) 
{
    std::string birthday;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, birthday);

        std::string fullDate = birthday + " 00:00:00";
        if (isValidDate(fullDate)) 
        {
            return birthday;
        }
        else 
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Неверный формат даты. Пример: 25.12.1990" << std::endl;
            Config::resetColor();
        }
    }
}

int InputValidation::getValidatedNumber(const std::string& prompt, int min, int max) 
{
    int number;
    while (true)
    {
        number = getValidatedInput<int>(prompt, "Введите целое число: ");

        if (number >= min && number <= max) 
        {
            return number;
        }
        else 
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Число должно быть в диапазоне от " << min << " до " << max << std::endl;
            Config::resetColor();
        }
    }
}

double InputValidation::getValidatedDouble(const std::string& prompt, double min, double max) 
{
    double number;
    while (true)
    {
        std::cout << prompt;
        std::cin >> number;

        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Введите дробное число: " << std::endl;
            Config::resetColor();
        }
        else if (number >= min && number <= max) 
        {
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return number;
        }
        else 
        {
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Число должно быть в диапазоне от " << min << " до " << max << std::endl;
            Config::resetColor();
        }
    }
}

std::string InputValidation::getValidatedString(const std::string& prompt, size_t minLength, size_t maxLength) 
{
    std::string input;
    while (true) 
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input.length() >= minLength && input.length() <= maxLength) 
        {
            return input;
        }
        else
        {
            Config::setColor(Config::COLOR_ERROR);
            std::cout << "Длина строки должна быть от " << minLength << " до " << maxLength << " символов" << std::endl;
            Config::resetColor();
        }
    }
}

std::string InputValidation::generateRandomPassword()
{
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%&*";
    std::string password = "";
    for (int i = 0; i < 12; ++i) 
    {
        password += chars[rand() % chars.size()];
    }
    return password;
}