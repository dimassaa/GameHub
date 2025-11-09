#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <conio.h>
#include "Config.h"
#include "Exceptions.h"

class InputValidation 
{
private:
    static bool isValidEmail(const std::string& email);
    static bool isValidPhone(const std::string& phone);
    static bool isValidDate(const std::string& date);
    static bool isNumber(const std::string& str);

public:
    static std::string maskPassword();
    static std::string generateSalt();
    static std::string hashPassword(const std::string& password, const std::string& salt);
    static std::string generateRandomPassword();

    template<typename T>
    static T getValidatedInput(const std::string& prompt, const std::string& errorMessage = "Неверный ввод. Попробуйте снова: ");

    static std::string getValidatedEmail(const std::string& prompt = "Введите email: ");
    static std::string getValidatedPhone(const std::string& prompt = "Введите телефон (+375...): ");
    static std::string getValidatedDate(const std::string& prompt = "Введите дату (ДД.ММ.ГГГГ ЧЧ:ММ:СС): ");
    static std::string getValidatedBirthday(const std::string& prompt = "Введите дату рождения (ДД.ММ.ГГГГ): ");

    static int getValidatedNumber(const std::string& prompt, int min = INT_MIN, int max = INT_MAX);
    static double getValidatedDouble(const std::string& prompt, double min = -1e9, double max = 1e9);
    static std::string getValidatedString(const std::string& prompt, size_t minLength = 1, size_t maxLength = 100);
};
