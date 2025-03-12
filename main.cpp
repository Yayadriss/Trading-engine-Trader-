#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QPair>
#include <QVector>
#include <QDateTime>
#include <cstdlib>  // For rand()
#include <ctime>    // For srand()
#include <algorithm> // For std::min

/**
 * Generates a list of random bid or ask orders
 * Each order consists of a price and quantity
 *
 * @param isBid - true for bid orders, false for ask orders
 * @param basePrice - the base price around which orders fluctuate
 * @param count - the number of orders to generate
 * @return A QVector containing pairs of price and quantity
 */
QVector<QPair<double, int>> generateRandomOrders(bool isBid, double basePrice, int count) {
    QVector<QPair<double, int>> orders;
    for (int i = 0; i < count; ++i) {
        double priceVariation = (rand() % 50) / 100.0; // Variation between 0.00 and 0.50
        double price = isBid ? basePrice - priceVariation : basePrice + priceVariation;
        int quantity = (rand() % 10 + 1) * 10; // Quantity between 10 and 100
        orders.append(qMakePair(price, quantity));
    }
    return orders;
}

/**
 * Populates a QTableWidget with order data (bids or asks)
 *
 * @param table - pointer to the QTableWidget to fill
 * @param data - QVector of price and quantity pairs
 * @param isBid - true for bid orders, false for ask orders
 */
void populateOrderTable(QTableWidget* table, const QVector<QPair<double, int>>& data, bool isBid) {
    table->clearContents();
    table->setRowCount(data.size());

    for (int i = 0; i < data.size(); ++i) {
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(data[i].first, 'f', 2));
        QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(data[i].second));

        // Use color coding: green for bids, red for asks
        if (isBid) {
            priceItem->setBackground(Qt::green);
            quantityItem->setBackground(Qt::green);
        } else {
            priceItem->setBackground(Qt::red);
            quantityItem->setBackground(Qt::red);
        }

        table->setItem(i, 0, priceItem);
        table->setItem(i, 1, quantityItem);
    }
}

/**
 * Adds a new order entry to the history table
 *
 * @param historyTable - pointer to the QTableWidget storing order history
 * @param type - string indicating order type ("Buy" or "Sell")
 * @param price - price of the order
 * @param quantity - quantity of the order
 */
void addHistoryEntry(QTableWidget* historyTable, const QString& type, double price, int quantity) {
    int row = historyTable->rowCount(); // Get the next available row
    historyTable->insertRow(row);      // Add a new row

    // Insert order details into the table
    historyTable->setItem(row, 0, new QTableWidgetItem(type));
    historyTable->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 2)));
    historyTable->setItem(row, 2, new QTableWidgetItem(QString::number(quantity)));
    historyTable->setItem(row, 3, new QTableWidgetItem(QDateTime::currentDateTime().toString("hh:mm:ss"))); // Timestamp
}

/**
 * Moves an order from the ongoing table to the completed table
 *
 * @param ongoingTable - pointer to the table containing active orders
 * @param completedTable - pointer to the table storing completed orders
 * @param row - the row index of the order to move
 */
void moveOrderToCompleted(QTableWidget* ongoingTable, QTableWidget* completedTable, int row) {
    // Retrieve order details
    QString type = ongoingTable->item(row, 0)->text();
    double price = ongoingTable->item(row, 1)->text().toDouble();
    int quantity = ongoingTable->item(row, 2)->text().toInt();
    QString time = ongoingTable->item(row, 3)->text();

    // Move order to completed orders table
    int completedRow = completedTable->rowCount();
    completedTable->insertRow(completedRow);
    completedTable->setItem(completedRow, 0, new QTableWidgetItem(type));
    completedTable->setItem(completedRow, 1, new QTableWidgetItem(QString::number(price, 'f', 2)));
    completedTable->setItem(completedRow, 2, new QTableWidgetItem(QString::number(quantity)));
    completedTable->setItem(completedRow, 3, new QTableWidgetItem(time));

    // Remove the order from the ongoing table
    ongoingTable->removeRow(row);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    srand(time(0)); // Initialize random seed

    // Create main application window
    QWidget window;
    window.setWindowTitle("Trading Engine");
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);

    QLabel* titleLabel = new QLabel("Trading Simulation - Index XYZ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Full UI setup (tables, buttons, order book, execution,...)
  
    // Bids section
    QLabel* bidLabel = new QLabel("Top 5 Bids");
    QTableWidget* bidTable = new QTableWidget(0, 2);
    bidTable->setHorizontalHeaderLabels({"Price", "Quantity"});
    bidTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* bidLayout = new QVBoxLayout();
    bidLayout->addWidget(bidLabel);
    bidLayout->addWidget(bidTable);

    // Asks section
    QLabel* askLabel = new QLabel("Top 5 Asks");
    QTableWidget* askTable = new QTableWidget(0, 2);
    askTable->setHorizontalHeaderLabels({"Price", "Quantity"});
    askTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* askLayout = new QVBoxLayout();
    askLayout->addWidget(askLabel);
    askLayout->addWidget(askTable);

    // Buy/Sell buttons
    QPushButton* buyButton = new QPushButton("Buy");
    QPushButton* sellButton = new QPushButton("Sell");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(buyButton);
    buttonLayout->addWidget(sellButton);

    // Create a horizontal layout to display Bids and Asks side by side
    QHBoxLayout* orderBookLayout = new QHBoxLayout();
    orderBookLayout->addLayout(bidLayout);  // Add the bid layout (buy orders)
    orderBookLayout->addLayout(askLayout);  // Add the ask layout (sell orders)

    // Create a vertical layout for the entire order book section
    QVBoxLayout* orderBookSection = new QVBoxLayout();
    orderBookSection->addLayout(orderBookLayout);  // Add the Bids/Asks layout
    orderBookSection->addLayout(buttonLayout);  // Add the buttons below

    // Add the order book section to the main layout
    mainLayout->addLayout(orderBookSection);

    // Create a table for ongoing (active) orders
    QLabel* ongoingLabel = new QLabel("Ongoing Orders");
    QTableWidget* ongoingTable = new QTableWidget(0, 4); // 4 columns including "Time"
    ongoingTable->setHorizontalHeaderLabels({"Type", "Price", "Quantity", "Time"});
    ongoingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add the label and table to the main layout
    mainLayout->addWidget(ongoingLabel);
    mainLayout->addWidget(ongoingTable);

    // Create a button to close a selected order
    QPushButton* closeOrderButton = new QPushButton("Close Selected Order");
    mainLayout->addWidget(closeOrderButton);

    // Create a table for completed (closed) orders
    QLabel* completedLabel = new QLabel("Completed Orders");
    QTableWidget* completedTable = new QTableWidget(0, 4); // 4 columns including "Time"
    completedTable->setHorizontalHeaderLabels({"Type", "Price", "Quantity", "Time"});
    completedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add the label and table to the main layout
    mainLayout->addWidget(completedLabel);
    mainLayout->addWidget(completedTable);

    // Create input fields for placing limit orders
    QLineEdit* priceInput = new QLineEdit();
    priceInput->setPlaceholderText("Enter Price");  // User enters the price
    QLineEdit* quantityInput = new QLineEdit();
    quantityInput->setPlaceholderText("Enter Quantity");  // User enters the quantity

    // Create buttons for limit buy and limit sell orders
    QPushButton* buyLimitButton = new QPushButton("Buy Limit");
    QPushButton* sellLimitButton = new QPushButton("Sell Limit");

    // Create a horizontal layout for the limit order inputs and buttons
    QHBoxLayout* limitOrderLayout = new QHBoxLayout();
    limitOrderLayout->addWidget(priceInput);
    limitOrderLayout->addWidget(quantityInput);
    limitOrderLayout->addWidget(buyLimitButton);
    limitOrderLayout->addWidget(sellLimitButton);

    // Add the limit order section to the main layout
    mainLayout->addLayout(limitOrderLayout);

    // Connect the "Buy" button to a function that executes a market buy order
    QObject::connect(buyButton, &QPushButton::clicked, [&]() {
        if (askTable->rowCount() > 0) {  // Check if there are sell orders available
            double price = askTable->item(0, 0)->text().toDouble();  // Get the price from the first row
            int quantity = askTable->item(0, 1)->text().toInt();  // Get the quantity
            addHistoryEntry(ongoingTable, "Market Buy", price, quantity);  // Add to the ongoing orders
            askTable->removeRow(0);  // Remove the top sell order from the ask table
        }
    });

    // Connect the "Sell" button to a function that executes a market sell order
    QObject::connect(sellButton, &QPushButton::clicked, [&]() {
        if (bidTable->rowCount() > 0) {  // Check if there are buy orders available
            double price = bidTable->item(0, 0)->text().toDouble();  // Get the price from the first row
            int quantity = bidTable->item(0, 1)->text().toInt();  // Get the quantity
            addHistoryEntry(ongoingTable, "Market Sell", price, quantity);  // Add to the ongoing orders
            bidTable->removeRow(0);  // Remove the top buy order from the bid table
        }
    });

    // Connect the "Close Selected Order" button to move an order from ongoing to completed
    QObject::connect(closeOrderButton, &QPushButton::clicked, [&]() {
        if (ongoingTable->currentRow() >= 0) {  // Check if a row is selected
            moveOrderToCompleted(ongoingTable, completedTable, ongoingTable->currentRow());  // Move order
        }
    });

    // Set up a timer to refresh bid and ask data in real-time
    double basePrice = 100.0;  // Set a base price for order generation
    QTimer* timer = new QTimer(&window);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        auto bids = generateRandomOrders(true, basePrice, 5);  // Generate 5 random buy orders
        auto asks = generateRandomOrders(false, basePrice, 5); // Generate 5 random sell orders
        populateOrderTable(bidTable, bids, true);  // Populate the bid table
        populateOrderTable(askTable, asks, false); // Populate the ask table
    });
    timer->start(2000); // Refresh orders every 2 seconds

    // Set the main layout for the window and display it
    window.setLayout(mainLayout);
    window.show();

    // Start the application event loop
    return app.exec();

}
