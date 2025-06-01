#ifndef SPECTATOR_MANAGER_HPP
#define SPECTATOR_MANAGER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

// Spectator class to represent each viewer
class Spectator {
private:
    string name;
    string email;
    string spectatorType;  // "VIP", "Influencer", "General"
    int priority;          // 1=VIP, 2=Influencer, 3=General (lower number = higher priority)
    int arrivalTime;       // Registration timestamp
    string seatSection;    // Assigned seat section
    bool isSeated;         // Seating status

public:
    // Constructors
    Spectator();
    Spectator(string n, string e, string type, int arrival);
    
    // Getters
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getSpectatorType() const { return spectatorType; }
    int getPriority() const { return priority; }
    int getArrivalTime() const { return arrivalTime; }
    string getSeatSection() const { return seatSection; }
    bool getIsSeated() const { return isSeated; }
    
    // Setters
    void setName(string n) { name = n; }
    void setEmail(string e) { email = e; }
    void setSpectatorType(string type);
    void setSeatSection(string section) { seatSection = section; }
    void setIsSeated(bool seated) { isSeated = seated; }
    
    // Utility functions
    void displaySpectator() const;
    string toString() const;
    
    // Comparison operators for priority queue
    bool operator<(const Spectator& other) const;
    bool operator>(const Spectator& other) const;
};

// Priority Queue implementation using Max Heap
class SpectatorPriorityQueue {
private:
    Spectator* heap;        // Dynamic array to store spectators
    int capacity;           // Maximum capacity of heap
    int size;               // Current number of elements
    
    // Helper functions for heap operations
    int getParentIndex(int index) const { return (index - 1) / 2; }
    int getLeftChildIndex(int index) const { return 2 * index + 1; }
    int getRightChildIndex(int index) const { return 2 * index + 2; }
    
    void heapifyUp(int index);      // Maintain heap property upward
    void heapifyDown(int index);    // Maintain heap property downward
    void resizeHeap();              // Resize heap when capacity exceeded

public:
    // Constructor and Destructor
    SpectatorPriorityQueue(int initialCapacity = 10);
    ~SpectatorPriorityQueue();
    
    // Core queue operations
    void insert(const Spectator& spectator);    // Add spectator to queue
    Spectator extractMax();                     // Remove highest priority spectator
    Spectator peek() const;                     // View highest priority without removing
    
    // Utility functions
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    
    // Display functions
    void displayQueue() const;
    void displayByPriority() const;
};

// Main Spectator Management System
class SpectatorManager {
private:
    SpectatorPriorityQueue* waitingQueue;    // Queue for spectators waiting for seats
    Spectator* seatedSpectators;             // Array of seated spectators
    int totalSeats;                          // Total venue capacity
    int occupiedSeats;                       // Currently occupied seats
    int vipSeats;                           // Reserved VIP seats
    int influencerSeats;                    // Reserved Influencer seats
    int generalSeats;                       // General admission seats
    
    // Seat management
    struct SeatCapacity {
        int vipAvailable;
        int influencerAvailable;
        int generalAvailable;
    } seatStatus;

public:
    // Constructor and Destructor
    SpectatorManager(int vip = 20, int influencer = 30, int general = 100);
    ~SpectatorManager();
    
    // Main operations
    void registerSpectator();               // Register new spectator
    void allocateSeating();                 // Process queue and assign seats
    void removeSpectator();                 // Remove seated spectator
    void searchSpectator();                 // Find spectator by name/email
    
    // Display functions
    void displayWaitingQueue();             // Show all waiting spectators
    void displaySeatedSpectators();         // Show all seated spectators
    void displayVenueStatus();              // Show seat availability
    void displayStatistics();               // Show system statistics
    
    // File operations
    void saveToFile(const string& filename);        // Save spectator data to CSV
    void loadFromFile(const string& filename);      // Load spectator data from CSV
    
    // System management
    void displayMenu();                     // Main menu interface
    void runSystem();                       // Main system loop
    
    // Utility functions
    bool hasAvailableSeats(const string& spectatorType);
    string assignSeatSection(const string& spectatorType);
    void updateSeatStatus(const string& spectatorType, bool occupy);
};

#endif