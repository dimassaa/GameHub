#include "Config.h"

namespace Config 
{
    const int COLOR_DEFAULT = 7;
    const int COLOR_ERROR = 12;
    const int COLOR_WARNING = 14;
    const int COLOR_SUCCESS = 10;
    const int COLOR_HEADER = 9;
    const int COLOR_MENUU = 11;

    const std::string USERS_FILE = "users.dat";
    const std::string PRODUCTS_FILE = "products.dat";
    const std::string SALES_FILE = "sales.dat";
    const std::string DISCOUNTS_FILE = "discounts.dat";
    const std::string WISHLISTS_FILE = "wishlists.dat";
    const std::string GENRES_FILE = "genres.dat";
    const std::string TAGS_FILE = "tags.dat";
    const std::string LOG_FILE = "system.log";

    const int PAGE_SIZE = 10;
    const int MAX_DISPLAY_WIDTH = 80;
    const std::string DATE_FORMAT = "%d.%m.%Y %H:%M:%S";
}

void Config::setColor(int color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Config::resetColor() 
{
    setColor(Config::COLOR_DEFAULT);
}

std::string Config::getCurrentDateTime() 
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), Config::DATE_FORMAT.c_str(), &tm);
    return std::string(buffer);
}

std::string Config::centerText(const std::string& text, int width) 
{
    if (text.length() >= width) return text;
    int padding = width - text.length();
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;
    return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
}