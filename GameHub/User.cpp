#include "User.h"

User::User() : id(0), isApproved(false) {}

User::User(int id, const std::string& username, const std::string& passwordHash,
    const std::string& email, const std::string& phone, const std::string& role)
    : id(id), username(username), passwordHash(passwordHash),
    email(email), phone(phone), role(role), isApproved(role == "admin"),
    registrationDate(Config::getCurrentDateTime()) {
}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPasswordHash() const { return passwordHash; }
std::string User::getEmail() const { return email; }
std::string User::getPhone() const { return phone; }
std::string User::getRole() const { return role; }
bool User::getIsApproved() const { return isApproved; }
std::string User::getRegistrationDate() const { return registrationDate; }
std::string User::getUnlockDate() const { return unlockDate; }

void User::setUsername(const std::string& newUsername) { username = newUsername; }
void User::setPasswordHash(const std::string& newHash) { passwordHash = newHash; }
void User::setEmail(const std::string& newEmail) { email = newEmail; }
void User::setPhone(const std::string& newPhone) { phone = newPhone; }
void User::setRole(const std::string& newRole) { role = newRole; }
void User::setIsApproved(bool approved) { isApproved = approved; }
void User::setUnlockDate(const std::string& date) { unlockDate = date; }

void User::serialize(std::ostream& os) const 
{
    os.write(reinterpret_cast<const char*>(&id), sizeof(id));

    size_t size = username.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(username.c_str(), size);

    size = passwordHash.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(passwordHash.c_str(), size);

    size = email.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(email.c_str(), size);

    size = phone.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(phone.c_str(), size);

    size = role.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(role.c_str(), size);

    os.write(reinterpret_cast<const char*>(&isApproved), sizeof(isApproved));

    size = registrationDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(registrationDate.c_str(), size);

    size = unlockDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(unlockDate.c_str(), size);
}

void User::deserialize(std::istream& is) 
{
    is.read(reinterpret_cast<char*>(&id), sizeof(id));

    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    username.resize(size);
    is.read(&username[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    passwordHash.resize(size);
    is.read(&passwordHash[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    email.resize(size);
    is.read(&email[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    phone.resize(size);
    is.read(&phone[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    role.resize(size);
    is.read(&role[0], size);

    is.read(reinterpret_cast<char*>(&isApproved), sizeof(isApproved));

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    registrationDate.resize(size);
    is.read(&registrationDate[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    unlockDate.resize(size);
    is.read(&unlockDate[0], size);
}

bool User::isBlocked() const 
{
    if (unlockDate.empty()) return false;
    return Config::getCurrentDateTime() < unlockDate;
}


Admin::Admin() : User() {}

Admin::Admin(int id, const std::string& username, const std::string& passwordHash,
    const std::string& email, const std::string& phone)
    : User(id, username, passwordHash, email, phone, "admin") {
}

void Admin::displayProfile() const 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ПРОФИЛЬ АДМИНИСТРАТОРА ===" << std::endl;
    Config::resetColor();

    std::cout << "ID: " << id << std::endl;
    std::cout << "Имя пользователя: " << username << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Телефон: " << phone << std::endl;
    std::cout << "Дата регистрации: " << registrationDate << std::endl;

    if (!unlockDate.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Аккаунт заблокирован до: " << unlockDate << std::endl;
        Config::resetColor();
    }
}

std::string Admin::getUserType() const 
{
    return "Administrator";
}

Customer::Customer() : User(), totalSpent(0.0), lastPurchasedProductId(-1) {}

Customer::Customer(int id, const std::string& username, const std::string& passwordHash,
    const std::string& email, const std::string& phone, const std::string& birthday)
    : User(id, username, passwordHash, email, phone, "customer"),
    totalSpent(0.0), birthday(birthday), lastPurchasedProductId(-1) {
}

double Customer::getTotalSpent() const { return totalSpent; }
std::string Customer::getBirthday() const { return birthday; }
int Customer::getLastPurchasedProductId() const { return lastPurchasedProductId; }

void Customer::setTotalSpent(double spent) { totalSpent = spent; }
void Customer::setLastPurchasedProductId(int productId) { lastPurchasedProductId = productId; }

void Customer::displayProfile() const 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ПРОФИЛЬ ПОКУПАТЕЛЯ ===" << std::endl;
    Config::resetColor();

    std::cout << "ID: " << id << std::endl;
    std::cout << "Имя пользователя: " << username << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Телефон: " << phone << std::endl;
    std::cout << "Дата рождения: " << birthday << std::endl;
    std::cout << "Всего потрачено: " << totalSpent << " руб." << std::endl;
    std::cout << "Дата регистрации: " << registrationDate << std::endl;
    std::cout << "Статус: " << (isApproved ? "Одобрен" : "Ожидает одобрения") << std::endl;

    if (!unlockDate.empty()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Аккаунт заблокирован до: " << unlockDate << std::endl;
        Config::resetColor();
    }
}

std::string Customer::getUserType() const 
{
    return "Customer";
}

void Customer::addPurchase(double amount) 
{
    totalSpent += amount;
}

void Customer::serialize(std::ostream& os) const 
{
    User::serialize(os);
    os.write(reinterpret_cast<const char*>(&totalSpent), sizeof(totalSpent));

    size_t size = birthday.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(birthday.c_str(), size);

    os.write(reinterpret_cast<const char*>(&lastPurchasedProductId), sizeof(lastPurchasedProductId));
}

void Customer::deserialize(std::istream& is) 
{
    User::deserialize(is);
    is.read(reinterpret_cast<char*>(&totalSpent), sizeof(totalSpent));

    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    birthday.resize(size);
    is.read(&birthday[0], size);

    is.read(reinterpret_cast<char*>(&lastPurchasedProductId), sizeof(lastPurchasedProductId));
}