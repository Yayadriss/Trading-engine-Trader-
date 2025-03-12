#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

// Include necessary Qt SQL and standard libraries
#include <QtSql/QSqlDatabase>  // Provides database connectivity
#include <QtSql/QSqlQuery>     // Used for executing SQL queries
#include <QString>             // Handles text strings
#include <QDebug>              // Used for debugging (prints messages to console)
#include <QVector>             // Dynamic array container similar to std::vector
#include <QPair>               // Stores pairs of values (useful for storing price and quantity)

// DatabaseManager class handles all database-related operations
class DatabaseManager {
public:
    // Constructor: Initializes the database connection
    DatabaseManager();

    // Adds a new order to the database
    // Parameters:
    // - type: "Buy" or "Sell"
    // - price: The price of the order
    // - quantity: The quantity of the asset being bought or sold
    void addOrder(const QString& type, double price, int quantity);

    // Retrieves the top buy orders (bids) from the database
    // - limit: The maximum number of orders to return
    // Returns a vector of pairs (price, quantity)
    QVector<QPair<double, int>> getTopBids(int limit);

    // Retrieves the top sell orders (asks) from the database
    // - limit: The maximum number of orders to return
    // Returns a vector of pairs (price, quantity)
    QVector<QPair<double, int>> getTopAsks(int limit);

    // Retrieves a list of completed (executed) orders from the database
    // Returns a vector of pairs (price, quantity)
    QVector<QPair<double, int>> getCompletedOrders();

private:
    QSqlDatabase db;  // The database connection object

    // Initializes the database (creates tables if they don't exist)
    void initializeDatabase();
};

#endif // DATABASE_MANAGER_H
