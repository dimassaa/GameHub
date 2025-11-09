#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>
#include <mutex>
#include <conio.h>
#include <windows.h>

namespace Config 
{
    // Цвета консоли
    extern const int COLOR_DEFAULT;
    extern const int COLOR_ERROR;
    extern const int COLOR_WARNING;
    extern const int COLOR_SUCCESS;
    extern const int COLOR_HEADER;
    extern const int COLOR_MENUU;

    // Пути к файлам
    extern const std::string USERS_FILE;
    extern const std::string PRODUCTS_FILE;
    extern const std::string SALES_FILE;
    extern const std::string DISCOUNTS_FILE;
    extern const std::string WISHLISTS_FILE;
    extern const std::string GENRES_FILE;
    extern const std::string TAGS_FILE;
    extern const std::string LOG_FILE;

    // Настройки
    extern const int PAGE_SIZE;
    extern const int MAX_DISPLAY_WIDTH;
    extern const std::string DATE_FORMAT;

    void setColor(int color);
    void resetColor();

    std::string getCurrentDateTime();

    std::string centerText(const std::string& text, int width);
}
