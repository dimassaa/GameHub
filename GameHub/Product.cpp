#include "Product.h"

Product::Product() : id(0), basePrice(0.0), ageRating(0), averageRating(0.0), activationKeys(0) {}

Product::Product(int id, const std::string& title, double basePrice, const std::string& developer,
    const std::string& publisher, const std::string& genre, int ageRating,
    const std::string& description, const std::string& releaseDate)
    : id(id), title(title), basePrice(basePrice), developer(developer),
    publisher(publisher), genre(genre), ageRating(ageRating),
    description(description), releaseDate(releaseDate),
    averageRating(0.0), activationKeys(0) {
}

int Product::getId() const { return id; }
std::string Product::getTitle() const { return title; }
double Product::getBasePrice() const { return basePrice; }
std::string Product::getDeveloper() const { return developer; }
std::string Product::getPublisher() const { return publisher; }
std::string Product::getGenre() const { return genre; }
int Product::getAgeRating() const { return ageRating; }
std::string Product::getDescription() const { return description; }
std::string Product::getReleaseDate() const { return releaseDate; }
double Product::getAverageRating() const { return averageRating; }
std::vector<std::string> Product::getTags() const { return tags; }
int Product::getActivationKeys() const { return activationKeys; }

void Product::setTitle(const std::string& newTitle) { title = newTitle; }
void Product::setBasePrice(double price) { basePrice = price; }
void Product::setDeveloper(const std::string& dev) { developer = dev; }
void Product::setPublisher(const std::string& pub) { publisher = pub; }
void Product::setGenre(const std::string& newGenre) { genre = newGenre; }
void Product::setAgeRating(int rating) { ageRating = rating; }
void Product::setDescription(const std::string& desc) { description = desc; }
void Product::setAverageRating(double rating) { averageRating = rating; }
void Product::setTags(const std::vector<std::string>& newTags) { tags = newTags; }
void Product::setActivationKeys(int keys) { activationKeys = keys; }

void Product::addTag(const std::string& tag) 
{
    tags.push_back(tag);
}

void Product::removeTag(const std::string& tag) 
{
    tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
}

bool Product::hasTag(const std::string& tag) const
{
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

bool Product::hasKeys() const 
{
    return activationKeys > 0;
}

bool Product::reserveKeys(int count) 
{
    if (activationKeys >= count)
    {
        activationKeys -= count;
        return true;
    }
    return false;
}

void Product::addKeys(int count) 
{
    activationKeys += count;
}

void Product::serialize(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&id), sizeof(id));

    size_t size = title.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(title.c_str(), size);

    os.write(reinterpret_cast<const char*>(&basePrice), sizeof(basePrice));

    size = developer.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(developer.c_str(), size);

    size = publisher.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(publisher.c_str(), size);

    size = genre.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(genre.c_str(), size);

    os.write(reinterpret_cast<const char*>(&ageRating), sizeof(ageRating));

    size = description.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(description.c_str(), size);

    size = releaseDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(releaseDate.c_str(), size);

    os.write(reinterpret_cast<const char*>(&averageRating), sizeof(averageRating));
    os.write(reinterpret_cast<const char*>(&activationKeys), sizeof(activationKeys));

    size_t tagsSize = tags.size();
    os.write(reinterpret_cast<const char*>(&tagsSize), sizeof(tagsSize));
    for (const auto& tag : tags) 
    {
        size_t tagSize = tag.size();
        os.write(reinterpret_cast<const char*>(&tagSize), sizeof(tagSize));
        os.write(tag.c_str(), tagSize);
    }
}

void Product::deserialize(std::istream& is) 
{
    is.read(reinterpret_cast<char*>(&id), sizeof(id));

    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    title.resize(size);
    is.read(&title[0], size);

    is.read(reinterpret_cast<char*>(&basePrice), sizeof(basePrice));

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    developer.resize(size);
    is.read(&developer[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    publisher.resize(size);
    is.read(&publisher[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    genre.resize(size);
    is.read(&genre[0], size);

    is.read(reinterpret_cast<char*>(&ageRating), sizeof(ageRating));

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    description.resize(size);
    is.read(&description[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    releaseDate.resize(size);
    is.read(&releaseDate[0], size);

    is.read(reinterpret_cast<char*>(&averageRating), sizeof(averageRating));
    is.read(reinterpret_cast<char*>(&activationKeys), sizeof(activationKeys));

    size_t tagsSize;
    is.read(reinterpret_cast<char*>(&tagsSize), sizeof(tagsSize));
    tags.resize(tagsSize);
    for (size_t i = 0; i < tagsSize; ++i) 
    {
        size_t tagSize;
        is.read(reinterpret_cast<char*>(&tagSize), sizeof(tagSize));
        std::string tag(tagSize, ' ');
        is.read(&tag[0], tagSize);
        tags[i] = tag;
    }
}

std::string Product::getShortTitle() const 
{
    if (title.length() > 30) 
    {
        return title.substr(0, 27) + "...";
    }
    return title;
}

std::string Product::getShortDescription() const 
{
    if (description.length() > 50) 
    {
        return description.substr(0, 47) + "...";
    }
    return description;
}

Game::Game() : Product() {}

Game::Game(int id, const std::string& title, double basePrice, const std::string& developer,
    const std::string& publisher, const std::string& genre, int ageRating,
    const std::string& description, const std::string& releaseDate)
    : Product(id, title, basePrice, developer, publisher, genre,
        ageRating, description, releaseDate) {
}

void Game::displayInfo() const 
{
    Config::setColor(Config::COLOR_HEADER);
    std::cout << "=== ИНФОРМАЦИЯ ОБ ИГРЕ ===" << std::endl;
    Config::resetColor();

    std::cout << "ID: " << id << std::endl;
    std::cout << "Название: " << title << std::endl;
    std::cout << "Цена: " << basePrice << " руб." << std::endl;
    std::cout << "Разработчик: " << developer << std::endl;
    std::cout << "Издатель: " << publisher << std::endl;
    std::cout << "Жанр: " << genre << std::endl;
    std::cout << "Возрастной рейтинг: " << ageRating << "+" << std::endl;
    std::cout << "Описание: " << description << std::endl;
    std::cout << "Дата выхода: " << releaseDate << std::endl;
    std::cout << "Рейтинг: " << averageRating << "/5" << std::endl;
    std::cout << "Ключи в наличии: " << activationKeys << std::endl;

    if (!tags.empty()) 
    {
        std::cout << "Теги: ";
        for (size_t i = 0; i < tags.size(); ++i) 
        {
            std::cout << tags[i];
            if (i < tags.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    if (!hasKeys()) 
    {
        Config::setColor(Config::COLOR_WARNING);
        std::cout << "Нет в наличии" << std::endl;
        Config::resetColor();
    }
}

std::string Game::getProductType() const 
{
    return "Game";
}