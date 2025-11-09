#include "Store.h"

void Discount::serialize(std::ostream& os) const 
{
    size_t size = type.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(type.c_str(), size);

    size_t targetsSize = targets.size();
    os.write(reinterpret_cast<const char*>(&targetsSize), sizeof(targetsSize));
    for (const auto& target : targets) 
    {
        size_t targetSize = target.size();
        os.write(reinterpret_cast<const char*>(&targetSize), sizeof(targetSize));
        os.write(target.c_str(), targetSize);
    }

    os.write(reinterpret_cast<const char*>(&percentage), sizeof(percentage));

    size = startDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(startDate.c_str(), size);

    size = endDate.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(endDate.c_str(), size);
}

void Discount::deserialize(std::istream& is) 
{
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    type.resize(size);
    is.read(&type[0], size);

    size_t targetsSize;
    is.read(reinterpret_cast<char*>(&targetsSize), sizeof(targetsSize));
    targets.resize(targetsSize);
    for (size_t i = 0; i < targetsSize; ++i) 
    {
        size_t targetSize;
        is.read(reinterpret_cast<char*>(&targetSize), sizeof(targetSize));
        std::string target(targetSize, ' ');
        is.read(&target[0], targetSize);
        targets[i] = target;
    }

    is.read(reinterpret_cast<char*>(&percentage), sizeof(percentage));

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    startDate.resize(size);
    is.read(&startDate[0], size);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    endDate.resize(size);
    is.read(&endDate[0], size);
}

bool Discount::isValid() const 
{
    std::string currentDate = Config::getCurrentDateTime();
    return currentDate >= startDate && currentDate <= endDate;
}

Store::Store() {
    logger = Logger::getInstance();
    loadData();
}

int Store::generateProductId() 
{
    if (products.empty()) return 1;
    int maxId = 0;
    for (const auto& product : products) 
    {
        if (product->getId() > maxId) 
        {
            maxId = product->getId();
        }
    }
    return maxId + 1;
}

int Store::generateSaleId() 
{
    if (sales.empty()) return 1;
    int maxId = 0;
    for (const auto& sale : sales) 
    {
        if (sale.getSaleId() > maxId) 
        {
            maxId = sale.getSaleId();
        }
    }
    return maxId + 1;
}

void Store::addProduct(std::shared_ptr<Product> product) 
{
    auto newProduct = std::make_shared<Game>(
        generateProductId(),
        product->getTitle(),
        product->getBasePrice(),
        product->getDeveloper(),
        product->getPublisher(),
        product->getGenre(),
        product->getAgeRating(),
        product->getDescription(),
        product->getReleaseDate()
    );
    newProduct->setTags(product->getTags());
    newProduct->setActivationKeys(product->getActivationKeys());

    products.push_back(newProduct);
    saveProducts();
    logger->log(LogLevel::INFO, "Product added: " + product->getTitle());
}

bool Store::updateProduct(int productId, std::shared_ptr<Product> updatedProduct) 
{
    for (auto& product : products) 
    {
        if (product->getId() == productId) 
        {
            product->setTitle(updatedProduct->getTitle());
            product->setBasePrice(updatedProduct->getBasePrice());
            product->setDeveloper(updatedProduct->getDeveloper());
            product->setPublisher(updatedProduct->getPublisher());
            product->setGenre(updatedProduct->getGenre());
            product->setAgeRating(updatedProduct->getAgeRating());
            product->setDescription(updatedProduct->getDescription());
            product->setTags(updatedProduct->getTags());
            product->setActivationKeys(updatedProduct->getActivationKeys());

            saveProducts();
            logger->log(LogLevel::INFO, "Product updated: ID " + std::to_string(productId));
            return true;
        }
    }
    return false;
}

bool Store::deleteProduct(int productId) 
{
    auto it = std::remove_if(products.begin(), products.end(),
        [productId](const std::shared_ptr<Product>& product) 
        {
            return product->getId() == productId;
        });

    if (it != products.end()) 
    {
        products.erase(it, products.end());
        saveProducts();
        logger->log(LogLevel::INFO, "Product deleted: ID " + std::to_string(productId));
        return true;
    }
    return false;
}

std::shared_ptr<Product> Store::getProductById(int id) 
{
    for (const auto& product : products) 
    {
        if (product->getId() == id) 
        {
            return product;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Product>> Store::searchProducts(const std::string& query) 
{
    std::vector<std::shared_ptr<Product>> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (const auto& product : products) 
    {
        std::string lowerTitle = product->getTitle();
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

        if (lowerTitle.find(lowerQuery) != std::string::npos) 
        {
            results.push_back(product);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Product>> Store::filterProducts(const std::string& filterType, const std::string& filterValue) 
{
    std::vector<std::shared_ptr<Product>> results;

    for (const auto& product : products) 
    {
        if (filterType == "genre" && product->getGenre() == filterValue) 
        {
            results.push_back(product);
        }
        else if (filterType == "developer" && product->getDeveloper() == filterValue) 
        {
            results.push_back(product);
        }
        else if (filterType == "tag" && product->hasTag(filterValue)) 
        {
            results.push_back(product);
        }
        else if (filterType == "has_discount" && getDiscountedPrice(product) < product->getBasePrice()) 
        {
            results.push_back(product);
        }
        else if (filterType == "in_stock" && product->hasKeys()) 
        {
            results.push_back(product);
        }
    }
    return results;
}

bool Store::addKeys(int productId, int count) 
{
    auto product = getProductById(productId);
    if (product) 
    {
        product->addKeys(count);
        saveProducts();

        if (count < 5) 
        {
            logger->log(LogLevel::WARNING, "Low keys for product: " + product->getTitle() + ", added: " + std::to_string(count));
        }
        else 
        {
            logger->log(LogLevel::INFO, "Keys added for product: " + product->getTitle() + ", count: " + std::to_string(count));
        }
        return true;
    }
    return false;
}

void Store::addDiscount(const Discount& discount) 
{
    discounts.push_back(discount);
    saveDiscounts();
    logger->log(LogLevel::INFO, "Discount added: " + discount.type + " " + std::to_string(discount.percentage) + "%");
}

double Store::getDiscountedPrice(std::shared_ptr<Product> product) 
{
    double price = product->getBasePrice();
    double bestDiscount = 0.0;

    for (const auto& discount : discounts) 
    {
        if (!discount.isValid()) continue;

        if (discount.type == "product")
        {
            for (const auto& target : discount.targets)
            {
                if (std::to_string(product->getId()) == target)
                {
                    bestDiscount = (std::max)(bestDiscount, discount.percentage);
                    break;
                }
            }
        }
        else if (discount.type == "genre" && std::find(discount.targets.begin(), discount.targets.end(), product->getGenre()) != discount.targets.end()) 
        {
            bestDiscount = (std::max)(bestDiscount, discount.percentage);
        }
        else if (discount.type == "developer" && std::find(discount.targets.begin(), discount.targets.end(), product->getDeveloper()) != discount.targets.end()) 
        {
            bestDiscount = (std::max)(bestDiscount, discount.percentage);
        }
        else if (discount.type == "tag") 
        {
            for (const auto& tag : product->getTags()) 
            {
                if (std::find(discount.targets.begin(), discount.targets.end(), tag) != discount.targets.end()) 
                {
                    bestDiscount = (std::max)(bestDiscount, discount.percentage);
                    break;
                }
            }
        }
    }

    return price * (1.0 - bestDiscount / 100.0);
}

bool Store::processSale(int customerId, const std::vector<std::pair<int, int>>& items, double discountAmount) {
    double totalAmount = 0.0;

    for (const auto& item : items) 
    {
        auto product = getProductById(item.first);
        if (!product) 
        {
            throw BusinessLogicError("Product not found: ID " + std::to_string(item.first));
        }
        if (!product->hasKeys() || product->getActivationKeys() < item.second) 
        {
            throw BusinessLogicError("Not enough keys for product: " + product->getTitle());
        }
        totalAmount += getDiscountedPrice(product) * item.second;
    }

    int saleId = generateSaleId();
    Sale sale(saleId, customerId, items, totalAmount, discountAmount);

    for (const auto& item : items) 
    {
        auto product = getProductById(item.first);
        product->reserveKeys(item.second);
    }

    sales.push_back(sale);
    saveSales();
    saveProducts();

    logger->log(LogLevel::INFO, "Sale processed: ID " + std::to_string(saleId) + ", Customer: " + std::to_string(customerId));
    return true;
}

std::vector<std::shared_ptr<Product>> Store::getAllProducts() const 
{
    return products;
}

std::vector<Discount> Store::getAllDiscounts() const 
{
    return discounts;
}

std::vector<Sale> Store::getAllSales() const 
{
    return sales;
}

void Store::loadData() 
{
    products = fileManager.loadFromFile<std::shared_ptr<Product>>(Config::PRODUCTS_FILE);
    sales = fileManager.loadFromFile<Sale>(Config::SALES_FILE);
    discounts = fileManager.loadFromFile<Discount>(Config::DISCOUNTS_FILE);
}

void Store::saveProducts() 
{
    fileManager.saveToFile(Config::PRODUCTS_FILE, products);
}

void Store::saveSales() 
{
    fileManager.saveToFile(Config::SALES_FILE, sales);
}

void Store::saveDiscounts()
{
    fileManager.saveToFile(Config::DISCOUNTS_FILE, discounts);
}

std::vector<std::string> Store::getUniqueGenres() 
{
    std::vector<std::string> genres;
    for (const auto& product : products) 
    {
        if (std::find(genres.begin(), genres.end(), product->getGenre()) == genres.end()) 
        {
            genres.push_back(product->getGenre());
        }
    }
    return genres;
}

std::vector<std::string> Store::getUniqueDevelopers() 
{
    std::vector<std::string> developers;
    for (const auto& product : products) 
    {
        if (std::find(developers.begin(), developers.end(), product->getDeveloper()) == developers.end()) 
        {
            developers.push_back(product->getDeveloper());
        }
    }
    return developers;
}

