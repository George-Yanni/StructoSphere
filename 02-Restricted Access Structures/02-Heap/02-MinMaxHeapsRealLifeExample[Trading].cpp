#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <ctime>
#include <unordered_map>

using namespace std;

// Order structure to store order information
struct Order {
    int order_id;
    string stock_symbol;
    double price;
    int quantity;
    string type; // "BID" or "ASK"
    time_t timestamp;

    // Default constructor
    Order() : order_id(0), stock_symbol(""), price(0.0), quantity(0), type(""), timestamp(0) {}

    // Parameterized constructor
    Order(int id, string symbol, double p, int qty, string t) 
        : order_id(id), stock_symbol(symbol), price(p), quantity(qty), type(t) {
        timestamp = time(nullptr);
    }

    // For max-heap (bids), we want highest price first
    bool operator<(const Order& other) const {
        if (price != other.price)
            return price < other.price; // Lower priority for max-heap
        return timestamp > other.timestamp; // Earlier orders have priority
    }

    // For min-heap (asks), we want lowest price first
    bool operator>(const Order& other) const {
        if (price != other.price)
            return price > other.price; // Higher priority for min-heap
        return timestamp > other.timestamp; // Earlier orders have priority
    }
};

class OrderBook {
private:
    // Max-heap for bids (buy orders)
    priority_queue<Order> bids;

    // Min-heap for asks (sell orders)
    priority_queue<Order, vector<Order>, greater<Order>> asks;

    // Map to track active orders for cancellation
    unordered_map<int, Order> active_orders;

public:
    // Add a new order to the order book
    void addOrder(const Order& order) {
        active_orders[order.order_id] = order;
        
        if (order.type == "BID") {
            bids.push(order);
            cout << "Added BID order #" << order.order_id 
                 << " for " << order.stock_symbol 
                 << " at $" << fixed << setprecision(2) << order.price 
                 << " x " << order.quantity << endl;
        } else {
            asks.push(order);
            cout << "Added ASK order #" << order.order_id 
                 << " for " << order.stock_symbol 
                 << " at $" << fixed << setprecision(2) << order.price 
                 << " x " << order.quantity << endl;
        }

        matchOrders();
    }

    // Cancel an existing order
    void cancelOrder(int order_id) {
        auto it = active_orders.find(order_id);
        if (it != active_orders.end()) {
            cout << "Cancelled order #" << order_id << endl;
            active_orders.erase(it);
        } else {
            cout << "Order #" << order_id << " not found" << endl;
        }
    }

    // Match bids and asks
    void matchOrders() {
        while (!bids.empty() && !asks.empty()) {
            Order top_bid = bids.top();
            Order top_ask = asks.top();

            // Check if orders are still active
            if (active_orders.count(top_bid.order_id) == 0) {
                bids.pop();
                continue;
            }
            if (active_orders.count(top_ask.order_id) == 0) {
                asks.pop();
                continue;
            }

            // Check if the best bid meets or exceeds the best ask
            if (top_bid.price >= top_ask.price) {
                // Remove from active orders map
                active_orders.erase(top_bid.order_id);
                active_orders.erase(top_ask.order_id);

                // Remove from heaps
                bids.pop();
                asks.pop();

                // Determine matched quantity
                int matched_qty = min(top_bid.quantity, top_ask.quantity);

                cout << "\n=== TRADE EXECUTED ===" << endl;
                cout << "BID #" << top_bid.order_id << " (" << top_bid.stock_symbol << ")" 
                     << " matched with ASK #" << top_ask.order_id << endl;
                cout << "Price: $" << fixed << setprecision(2) << top_ask.price << endl;
                cout << "Quantity: " << matched_qty << endl;
                cout << "=====================\n" << endl;

                // Handle remaining quantities
                if (top_bid.quantity > matched_qty) {
                    Order remaining_bid = top_bid;
                    remaining_bid.quantity -= matched_qty;
                    bids.push(remaining_bid);
                    active_orders[remaining_bid.order_id] = remaining_bid;
                }

                if (top_ask.quantity > matched_qty) {
                    Order remaining_ask = top_ask;
                    remaining_ask.quantity -= matched_qty;
                    asks.push(remaining_ask);
                    active_orders[remaining_ask.order_id] = remaining_ask;
                }
            } else {
                break; // No more matches possible
            }
        }
    }

    // Print the current order book
    void printOrderBook() const {
        cout << "\n====== ORDER BOOK ======" << endl;
        
        // Print bids (highest first)
        cout << "--- BIDS (BUY) ---" << endl;
        priority_queue<Order> temp_bids = bids;
        while (!temp_bids.empty()) {
            Order o = temp_bids.top();
            if (active_orders.count(o.order_id)) {
                cout << "BID #" << o.order_id << ": $" << fixed << setprecision(2) << o.price 
                     << " x " << o.quantity << endl;
            }
            temp_bids.pop();
        }

        // Print asks (lowest first)
        cout << "\n--- ASKS (SELL) ---" << endl;
        priority_queue<Order, vector<Order>, greater<Order>> temp_asks = asks;
        while (!temp_asks.empty()) {
            Order o = temp_asks.top();
            if (active_orders.count(o.order_id)) {
                cout << "ASK #" << o.order_id << ": $" << fixed << setprecision(2) << o.price 
                     << " x " << o.quantity << endl;
            }
            temp_asks.pop();
        }
        cout << "=====================\n" << endl;
    }
};

int main() {
    OrderBook orderBook;
    int order_id = 1;

    cout << "=== STOCK MARKET ORDER MATCHING SYSTEM ===" << endl;
    cout << "Using Max-Heap for BIDS and Min-Heap for ASKS\n" << endl;

    // Add some initial orders
    orderBook.addOrder(Order(order_id++, "AAPL", 150.25, 100, "BID"));
    orderBook.addOrder(Order(order_id++, "AAPL", 149.50, 200, "BID"));
    orderBook.addOrder(Order(order_id++, "AAPL", 151.00, 150, "ASK"));
    orderBook.addOrder(Order(order_id++, "AAPL", 150.00, 300, "ASK"));
    orderBook.addOrder(Order(order_id++, "AAPL", 150.50, 250, "BID"));

    orderBook.printOrderBook();

    // This order should trigger matches
    orderBook.addOrder(Order(order_id++, "AAPL", 150.00, 400, "BID"));

    orderBook.printOrderBook();

    // Add more orders
    orderBook.addOrder(Order(order_id++, "AAPL", 152.00, 100, "ASK"));
    orderBook.addOrder(Order(order_id++, "AAPL", 148.75, 150, "BID"));

    orderBook.printOrderBook();

    // Try to cancel an order
    orderBook.cancelOrder(2);

    orderBook.printOrderBook();

    return 0;
}