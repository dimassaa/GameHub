#include "Sale.h"

Sale::Sale() : saleId(0), customerId(0), totalAmount(0.0), discountAmount(0.0) {}

Sale::Sale(int saleId, int customerId, const std::vector<std::pair<int, int>>& items,
    double totalAmount, double discountAmount, const std::string& status)
    : saleId(saleId), customerId(customerId), items(items),
    totalAmount(totalAmount), discountAmount(discountAmount), status(status) {
    saleDateTime = Config::getCurrentDateTime();
}

int Sale::getSaleId() const { return saleId; }
int Sale::getCustomerId() const { return customerId; }
std::string Sale::getSaleDateTime() const { return saleDateTime; }
std::vector<std::pair<int, int>> Sale::getItems() const { return items; }
double Sale::getTotalAmount() const { return totalAmount; }
double Sale::getDiscountAmount() const { return discountAmount; }
std::string Sale::getStatus() const { return status; }
double Sale::getFinalAmount() const { return totalAmount - discountAmount; }

void Sale::setSaleId(int id) { saleId = id; }
void Sale::setCustomerId(int id) { customerId = id; }
void Sale::setSaleDateTime(const std::string& datetime) { saleDateTime = datetime; }
void Sale::setItems(const std::vector<std::pair<int, int>>& newItems) { items = newItems; }
void Sale::setTotalAmount(double amount) { totalAmount = amount; }
void Sale::setDiscountAmount(double amount) { discountAmount = amount; }
void Sale::setStatus(const std::string& newStatus) { status = newStatus; }

void Sale::addItem(int productId, int quantity) 
{
    for (auto& item : items) 
    {
        if (item.first == productId) 
        {
            item.second += quantity;
            return;
        }
    }
    items.push_back(std::make_pair(productId, quantity));
}

bool Sale::removeItem(int productId) 
{
    auto it = std::remove_if(items.begin(), items.end(),
        [productId](const std::pair<int, int>& item)
        {
            return item.first == productId;
        });

    if (it != items.end()) 
    {
        items.erase(it, items.end());
        return true;
    }
    return false;
}

int Sale::getItemQuantity(int productId) const 
{
    for (const auto& item : items) 
    {
        if (item.first == productId) 
        {
            return item.second;
        }
    }
    return 0;
}

void Sale::displaySaleInfo() const 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ИНФОРМАЦИЯ О ПРОДАЖЕ ===" << std::endl;
    Config::resetColor();

    std::cout << "ID продажи: " << saleId << std::endl;
    std::cout << "ID покупателя: " << customerId << std::endl;
    std::cout << "Дата и время: " << saleDateTime << std::endl;
    std::cout << "Статус: " << status << std::endl;

    Config::setColor(Config::COLOR_MENUU);
    std::cout << "--- Товары ---" << std::endl;
    Config::resetColor();

    for (const auto& item : items) 
    {
        std::cout << "Товар ID: " << item.first << ", Количество: " << item.second << std::endl;
    }

    Config::setColor(Config::COLOR_MENUU);
    std::cout << "--- Финансы ---" << std::endl;
    Config::resetColor();

    std::cout << "Общая сумма: " << totalAmount << " руб." << std::endl;
    std::cout << "Скидка: " << discountAmount << " руб." << std::endl;
    std::cout << "Итоговая сумма: " << getFinalAmount() << " руб." << std::endl;
}

void Sale::serialize(std::ostream& os) const 
{
    os.write(reinterpret_cast<const char*>(&saleId), sizeof(saleId));
    os.write(reinterpret_cast<const char*>(&customerId), sizeof(customerId));

    size_t size = saleDateTime.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(saleDateTime.c_str(), size);

    size_t itemsSize = items.size();
    os.write(reinterpret_cast<const char*>(&itemsSize), sizeof(itemsSize));
    for (const auto& item : items) 
    {
        os.write(reinterpret_cast<const char*>(&item.first), sizeof(item.first));
        os.write(reinterpret_cast<const char*>(&item.second), sizeof(item.second));
    }

    os.write(reinterpret_cast<const char*>(&totalAmount), sizeof(totalAmount));
    os.write(reinterpret_cast<const char*>(&discountAmount), sizeof(discountAmount));

    size = status.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(status.c_str(), size);
}

void Sale::deserialize(std::istream& is) 
{
    is.read(reinterpret_cast<char*>(&saleId), sizeof(saleId));
    is.read(reinterpret_cast<char*>(&customerId), sizeof(customerId));

    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    saleDateTime.resize(size);
    is.read(&saleDateTime[0], size);

    size_t itemsSize;
    is.read(reinterpret_cast<char*>(&itemsSize), sizeof(itemsSize));
    items.resize(itemsSize);
    for (size_t i = 0; i < itemsSize; ++i) 
    {
        int productId, quantity;
        is.read(reinterpret_cast<char*>(&productId), sizeof(productId));
        is.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
        items[i] = std::make_pair(productId, quantity);
    }

    is.read(reinterpret_cast<char*>(&totalAmount), sizeof(totalAmount));
    is.read(reinterpret_cast<char*>(&discountAmount), sizeof(discountAmount));

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    status.resize(size);
    is.read(&status[0], size);
}

bool Sale::containsProduct(int productId) const 
{
    for (const auto& item : items) 
    {
        if (item.first == productId) 
        {
            return true;
        }
    }
    return false;
}

int Sale::getTotalItemsCount() const 
{
    int total = 0;
    for (const auto& item : items) 
    {
        total += item.second;
    }
    return total;
}