#include "DatabaseManager.h"
#include <QtSql/QSqlError>  // Required for handling SQL errors

// Constructor: initializes the database connection
DatabaseManager::DatabaseManager() {
    // Use SQLite as the database engine
    db = QSqlDatabase::addDatabase("QSQLITE");

    // Set the database file name
    db.setDatabaseName("trading.db");

    // Attempt to open the database
    if (!db.open()) {
        qDebug() << "Failed to open database.";  // Print an error message if the database cannot be opened
    } else {
        initializeDatabase();  // If successful, initialize the database (create tables if needed)
    }
}

// Function to initialize the database structure (create tables if they don’t exist)
void DatabaseManager::initializeDatabase() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS orders ("  // Create the 'orders' table
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "  // Unique ID for each order
               "type TEXT, "  // Order type ("Buy" or "Sell")
               "price REAL, "  // Order price
               "quantity INTEGER, "  // Order quantity
               "status TEXT, "  // Status: "Pending" (active) or "Completed" (executed)
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");  // Automatic timestamp
}

// Function to add a new order to the database
void DatabaseManager::addOrder(const QString& type, double price, int quantity) {
    QSqlQuery query;
    query.prepare("INSERT INTO orders (type, price, quantity, status) "
                  "VALUES (:type, :price, :quantity, 'Pending')");  // Default status is "Pending"

    // Bind values to prevent SQL injection
    query.bindValue(":type", type);
    query.bindValue(":price", price);
    query.bindValue(":quantity", quantity);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Failed to add order:" << query.lastError();
    }
}

// Function to get the top buy orders (bids) sorted by highest price
QVector<QPair<double, int>> DatabaseManager::getTopBids(int limit) {
    QVector<QPair<double, int>> bids;  // Store results as pairs of (price, quantity)
    QSqlQuery query;

    // Select buy orders with "Pending" status, ordered by highest price
    query.prepare("SELECT price, quantity FROM orders WHERE type='Buy' AND status='Pending' "
                  "ORDER BY price DESC LIMIT :limit");

    query.bindValue(":limit", limit);  // Bind the limit parameter

    // Execute the query
    if (query.exec()) {
        while (query.next()) {
            // Append each result as a (price, quantity) pair
            bids.append(qMakePair(query.value(0).toDouble(), query.value(1).toInt()));
        }
    }
    return bids;  // Return the list of top buy orders
}

// Function to get the top sell orders (asks) sorted by lowest price
QVector<QPair<double, int>> DatabaseManager::getTopAsks(int limit) {
    QVector<QPair<double, int>> asks;  // Store results as pairs of (price, quantity)
    QSqlQuery query;

    // Select sell orders with "Pending" status, ordered by lowest price
    query.prepare("SELECT price, quantity FROM orders WHERE type='Sell' AND status='Pending' "
                  "ORDER BY price ASC LIMIT :limit");

    query.bindValue(":limit", limit);  // Bind the limit parameter

    // Execute the query
    if (query.exec()) {
        while (query.next()) {
            // Append each result as a (price, quantity) pair
            asks.append(qMakePair(query.value(0).toDouble(), query.value(1).toInt()));
        }
    }
    return asks;  // Return the list of top sell orders
}

// Function to retrieve completed (executed) orders from the database
QVector<QPair<double, int>> DatabaseManager::getCompletedOrders() {
    QVector<QPair<double, int>> completedOrders;  // Store results as pairs of (price, quantity)
    
    // Select orders where the status is "Completed"
    QSqlQuery query("SELECT price, quantity FROM orders WHERE status='Completed'");

    // Process the query results
    while (query.next()) {
        completedOrders.append(qMakePair(query.value(0).toDouble(), query.value(1).toInt()));
    }
    
    return completedOrders;  // Return the list of completed orders
}
