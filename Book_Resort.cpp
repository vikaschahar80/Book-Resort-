#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <fstream>
#include <queue>
#include <climits>

using namespace std;

// ------------------------- CLASS DEFINITIONS -----------------------------

class User {
public:
    string name;
    int age;
    string phone;

    void registerUser() {
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Phone Number: ";
        cin >> phone;
        cin.ignore();  // To handle getline newline issue
    }
};

class Resort {
private:
    map<int, bool> rooms;  // room number -> booked or not
    map<string, int> activities;  // activity -> cost
    vector<string> selectedActivities;
    int selectedRoom = -1;
    int totalBill = 0;

public:
    Resort() {
        // Initialize 10 rooms as available
        for (int i = 101; i <= 110; i++) {
            rooms[i] = false;
        }

        // Sample activities
        activities["Spa"] = 1500;
        activities["Swimming"] = 800;
        activities["Horse Riding"] = 1200;
        activities["Sightseeing"] = 1000;
    }

    void showAvailableRooms() {
        cout << "\nAvailable Rooms:\n";
        for (auto &room : rooms) {
            if (!room.second)
                cout << "Room " << room.first << endl;
        }
    }

    void bookRoom() {
        int choice;
        showAvailableRooms();
        cout << "Enter room number to book: ";
        cin >> choice;

        if (rooms.find(choice) != rooms.end() && !rooms[choice]) {
            rooms[choice] = true;
            selectedRoom = choice;
            totalBill += 3000; // base room cost
            cout << "Room " << choice << " booked successfully.\n";
        } else {
            cout << "Room not available or invalid.\n";
        }
    }

    void chooseActivities() {
        int ch;
        do {
            cout << "\nAvailable Activities:\n";
            int i = 1;
            map<int, string> options;
            for (auto &act : activities) {
                cout << i << ". " << act.first << " (Rs. " << act.second << ")\n";
                options[i] = act.first;
                i++;
            }
            cout << "0. Done selecting\n";
            cout << "Enter your choice (number): ";
            cin >> ch;
    
            if (options.count(ch)) {
                string act = options[ch];
                selectedActivities.push_back(act);
                totalBill += activities[act];
                cout << act << " added to your plan.\n";
            } else if (ch != 0) {
                cout << "Invalid option. Try again.\n";
            }
    
        } while (ch != 0);
    }

    void generateBill() {
        cout << "\n===== BILL SUMMARY =====\n";
        cout << "Room Number: " << selectedRoom << " (Rs. 3000)\n";
        cout << "Activities:\n";
        for (auto &act : selectedActivities) {
            cout << "- " << act << ": Rs. " << activities[act] << endl;
        }
        cout << "Total Amount: Rs. " << totalBill << "\n";
    }

    void takeFeedback() {
        cin.ignore();  // Flush input buffer
        string feedback;
        int rating;
    
        cout << "\nPlease rate our service (1-5): ";
        while (!(cin >> rating) || rating < 1 || rating > 5) {
            cout << "Invalid rating. Please enter a number from 1 to 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    
        cin.ignore(); // Flush newline before getline
        cout << "Any suggestions or feedback? ";
        getline(cin, feedback);
    
        ofstream file("feedback.txt", ios::app);
        file << "Rating: " << rating << "/5\n";
        file << "Feedback: " << feedback << "\n\n";
        file.close();
        cout << "Thanks for your feedback!\n";
    }
    
};

// ------------------------- GRAPH + DIJKSTRA -----------------------------

class Graph {
    int V;
    vector<pair<int, int>> adj[100];  // node -> list of (neighbor, weight)

public:
    Graph(int v) : V(v) {}

    void addEdge(int u, int v, int wt) {
        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt}); // undirected
    }

    void shortestPath(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto &nbr : adj[u]) {
                int v = nbr.first;
                int weight = nbr.second;

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "\nOptimal distances from Location " << src << ":\n";
        for (int i = 0; i < V; i++) {
            cout << "To Location " << i << " = " << dist[i] << " mins\n";
        }
    }
};

// ------------------------- MAIN -----------------------------

int main() {
    User user;
    Resort resort;

    cout << "==== Welcome to the Resort Booking System ====\n";
    user.registerUser();
    resort.bookRoom();
    resort.chooseActivities();
    resort.generateBill();
    resort.takeFeedback();

    // Graph-based route suggestion
    Graph g(5); // 5 places in resort
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(3, 4, 3);

    cout << "\nFinding best internal route...\n";
    g.shortestPath(0);  // Assuming entrance is 0

    cout << "\nThank you for using our service!\n";
    return 0;
}
