#include "spectator_manager.hpp"
#include <ctime>
#include <sstream>

// SPECTATOR CLASS IMPLEMENTATION

Spectator::Spectator() : name(""), email(""), spectatorType("General"), 
                        priority(3), arrivalTime(0), seatSection(""), isSeated(false) {}

Spectator::Spectator(string n, string e, string type, int arrival) 
    : name(n), email(e), arrivalTime(arrival), seatSection(""), isSeated(false) {
    setSpectatorType(type);
}


void Spectator::setSpectatorType(string type) {
    spectatorType = type;
    // Set priority based on type (lower number = higher priority)
    if (type == "VIP") {
        priority = 1;
    } else if (type == "Influencer") {
        priority = 2;
    } else {
        priority = 3;
        spectatorType = "General";
    }
}

void Spectator::displaySpectator() const {
    cout << left << setw(15) << name 
         << setw(25) << email 
         << setw(12) << spectatorType
         << setw(10) << priority
         << setw(15) << seatSection
         << setw(8) << (isSeated ? "Yes" : "No") << endl;
}

string Spectator::toString() const {
    return name + "," + email + "," + spectatorType + "," + 
           to_string(arrivalTime) + "," + seatSection + "," + 
           (isSeated ? "1" : "0");
}

bool Spectator::operator<(const Spectator& other) const {
    // For max heap: higher priority (lower number) comes first
    if (priority != other.priority) {
        return priority > other.priority;
    }
    // If same priority, earlier arrival time has higher priority
    return arrivalTime > other.arrivalTime;
}

bool Spectator::operator>(const Spectator& other) const {
    if (priority != other.priority) {
        return priority < other.priority;
    }
    return arrivalTime < other.arrivalTime;
}

// ===== PRIORITY QUEUE IMPLEMENTATION =====

SpectatorPriorityQueue::SpectatorPriorityQueue(int initialCapacity) 
    : capacity(initialCapacity), size(0) {
    heap = new Spectator[capacity];
}

SpectatorPriorityQueue::~SpectatorPriorityQueue() {
    delete[] heap;
}

void SpectatorPriorityQueue::insert(const Spectator& spectator) {
    if (size >= capacity) {
        resizeHeap();
    }
    
    heap[size] = spectator;
    heapifyUp(size);
    size++;
}

Spectator SpectatorPriorityQueue::extractMax() {
    if (isEmpty()) {
        throw runtime_error("Queue is empty!");
    }
    
    Spectator maxSpectator = heap[0];
    heap[0] = heap[size - 1];
    size--;
    
    if (size > 0) {
        heapifyDown(0);
    }
    
    return maxSpectator;
}

Spectator SpectatorPriorityQueue::peek() const {
    if (isEmpty()) {
        throw runtime_error("Queue is empty!");
    }
    return heap[0];
}

void SpectatorPriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = getParentIndex(index);
        if (heap[index] > heap[parentIndex]) {
            // Swap with parent
            Spectator temp = heap[index];
            heap[index] = heap[parentIndex];
            heap[parentIndex] = temp;
            index = parentIndex;
        } else {
            break;
        }
    }
}

void SpectatorPriorityQueue::heapifyDown(int index) {
    while (getLeftChildIndex(index) < size) {
        int maxChildIndex = getLeftChildIndex(index);
        
        // Find the child with higher priority
        if (getRightChildIndex(index) < size && 
            heap[getRightChildIndex(index)] > heap[maxChildIndex]) {
            maxChildIndex = getRightChildIndex(index);
        }
        
        if (heap[index] > heap[maxChildIndex]) {
            break;
        } else {
            // Swap with max child
            Spectator temp = heap[index];
            heap[index] = heap[maxChildIndex];
            heap[maxChildIndex] = temp;
            index = maxChildIndex;
        }
    }
}

void SpectatorPriorityQueue::resizeHeap() {
    int newCapacity = capacity * 2;
    Spectator* newHeap = new Spectator[newCapacity];
    
    for (int i = 0; i < size; i++) {
        newHeap[i] = heap[i];
    }
    
    delete[] heap;
    heap = newHeap;
    capacity = newCapacity;
}

void SpectatorPriorityQueue::displayQueue() const {
    if (isEmpty()) {
        cout << "No spectators in waiting queue.\n";
        return;
    }
    
    cout << "\n=== WAITING QUEUE ===\n";
    cout << left << setw(15) << "Name" 
         << setw(25) << "Email" 
         << setw(12) << "Type"
         << setw(10) << "Priority"
         << setw(15) << "Seat Section"
         << setw(8) << "Seated" << endl;
    cout << string(85, '-') << endl;
    
    for (int i = 0; i < size; i++) {
        heap[i].displaySpectator();
    }
}

void SpectatorPriorityQueue::displayByPriority() const {
    cout << "\n=== QUEUE BY PRIORITY ORDER ===\n";
    
    // Create temporary copy to show priority order without modifying original
    SpectatorPriorityQueue tempQueue(*this);
    
    int count = 1;
    while (!tempQueue.isEmpty()) {
        cout << count++ << ". ";
        Spectator spec = tempQueue.extractMax();
        spec.displaySpectator();
    }
}

// ===== SPECTATOR MANAGER IMPLEMENTATION =====

SpectatorManager::SpectatorManager(int vip, int influencer, int general) 
    : totalSeats(vip + influencer + general), occupiedSeats(0),
      vipSeats(vip), influencerSeats(influencer), generalSeats(general) {
    
    waitingQueue = new SpectatorPriorityQueue();
    seatedSpectators = new Spectator[totalSeats];
    
    // Initialize seat availability
    seatStatus.vipAvailable = vip;
    seatStatus.influencerAvailable = influencer;
    seatStatus.generalAvailable = general;
}

SpectatorManager::~SpectatorManager() {
    delete waitingQueue;
    delete[] seatedSpectators;
}

void SpectatorManager::registerSpectator() {
    string name, email, type;
    
    cout << "\n=== SPECTATOR REGISTRATION ===\n";
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    cout << "Select spectator type:\n";
    cout << "1. VIP\n2. Influencer\n3. General\n";
    cout << "Enter choice (1-3): ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1: type = "VIP"; break;
        case 2: type = "Influencer"; break;
        case 3: type = "General"; break;
        default: 
            cout << "Invalid choice. Defaulting to General.\n";
            type = "General";
    }
    
    // Create spectator with current timestamp
    int currentTime = static_cast<int>(time(nullptr));
    Spectator newSpectator(name, email, type, currentTime);
    
    // Add to waiting queue
    waitingQueue->insert(newSpectator);
    
    cout << "\nSpectator registered successfully!\n";
    cout << "Name: " << name << "\n";
    cout << "Type: " << type << "\n";
    cout << "Position in queue: " << waitingQueue->getSize() << "\n";
}

void SpectatorManager::allocateSeating() {
    if (waitingQueue->isEmpty()) {
        cout << "No spectators in waiting queue.\n";
        return;
    }
    
    cout << "\n=== SEAT ALLOCATION PROCESS ===\n";
    int allocated = 0;
    
    while (!waitingQueue->isEmpty()) {
        try {
            Spectator nextSpectator = waitingQueue->peek();
            
            if (hasAvailableSeats(nextSpectator.getSpectatorType())) {
                // Remove from queue and assign seat
                nextSpectator = waitingQueue->extractMax();
                string seatSection = assignSeatSection(nextSpectator.getSpectatorType());
                nextSpectator.setSeatSection(seatSection);
                nextSpectator.setIsSeated(true);
                
                // Add to seated spectators array
                seatedSpectators[occupiedSeats] = nextSpectator;
                occupiedSeats++;
                allocated++;
                
                cout << "✓ Allocated seat to: " << nextSpectator.getName() 
                     << " (Type: " << nextSpectator.getSpectatorType() 
                     << ", Section: " << seatSection << ")\n";
            } else {
                cout << "✗ No available seats for " << nextSpectator.getSpectatorType() 
                     << " spectator: " << nextSpectator.getName() << "\n";
                break;
            }
        } catch (const exception& e) {
            cout << "Error during allocation: " << e.what() << "\n";
            break;
        }
    }
    
    cout << "\nAllocation Summary:\n";
    cout << "- Total allocated: " << allocated << " spectators\n";
    cout << "- Remaining in queue: " << waitingQueue->getSize() << " spectators\n";
    cout << "- Total seated: " << occupiedSeats << "/" << totalSeats << " seats\n";
}

bool SpectatorManager::hasAvailableSeats(const string& spectatorType) {
    if (spectatorType == "VIP") {
        return seatStatus.vipAvailable > 0;
    } else if (spectatorType == "Influencer") {
        return seatStatus.influencerAvailable > 0 || seatStatus.generalAvailable > 0;
    } else { // General
        return seatStatus.generalAvailable > 0;
    }
}

string SpectatorManager::assignSeatSection(const string& spectatorType) {
    if (spectatorType == "VIP" && seatStatus.vipAvailable > 0) {
        seatStatus.vipAvailable--;
        return "VIP-" + to_string(vipSeats - seatStatus.vipAvailable);
    } else if (spectatorType == "Influencer") {
        if (seatStatus.influencerAvailable > 0) {
            seatStatus.influencerAvailable--;
            return "INF-" + to_string(influencerSeats - seatStatus.influencerAvailable);
        } else if (seatStatus.generalAvailable > 0) {
            seatStatus.generalAvailable--;
            return "GEN-" + to_string(generalSeats - seatStatus.generalAvailable);
        }
    } else if (spectatorType == "General" && seatStatus.generalAvailable > 0) {
        seatStatus.generalAvailable--;
        return "GEN-" + to_string(generalSeats - seatStatus.generalAvailable);
    }
    return "ERROR";
}

void SpectatorManager::displayWaitingQueue() {
    cout << "\n=== CURRENT WAITING QUEUE ===\n";
    waitingQueue->displayQueue();
    cout << "\nQueue size: " << waitingQueue->getSize() << " spectators\n";
}

void SpectatorManager::displaySeatedSpectators() {
    if (occupiedSeats == 0) {
        cout << "No spectators currently seated.\n";
        return;
    }
    
    cout << "\n=== SEATED SPECTATORS ===\n";
    cout << left << setw(15) << "Name" 
         << setw(25) << "Email" 
         << setw(12) << "Type"
         << setw(15) << "Seat Section" << endl;
    cout << string(67, '-') << endl;
    
    for (int i = 0; i < occupiedSeats; i++) {
        seatedSpectators[i].displaySpectator();
    }
    cout << "\nTotal seated: " << occupiedSeats << " spectators\n";
}

void SpectatorManager::displayVenueStatus() {
    cout << "\n=== VENUE STATUS ===\n";
    cout << "VIP Seats: " << (vipSeats - seatStatus.vipAvailable) << "/" << vipSeats 
         << " occupied (" << seatStatus.vipAvailable << " available)\n";
    cout << "Influencer Seats: " << (influencerSeats - seatStatus.influencerAvailable) << "/" << influencerSeats 
         << " occupied (" << seatStatus.influencerAvailable << " available)\n";
    cout << "General Seats: " << (generalSeats - seatStatus.generalAvailable) << "/" << generalSeats 
         << " occupied (" << seatStatus.generalAvailable << " available)\n";
    cout << string(50, '-') << endl;
    cout << "TOTAL: " << occupiedSeats << "/" << totalSeats 
         << " occupied (" << (totalSeats - occupiedSeats) << " available)\n";
    cout << "Queue Length: " << waitingQueue->getSize() << " waiting\n";
}

void SpectatorManager::displayStatistics() {
    cout << "\n=== SYSTEM STATISTICS ===\n";
    
    // Count by type in seated spectators
    int vipSeated = 0, influencerSeated = 0, generalSeated = 0;
    for (int i = 0; i < occupiedSeats; i++) {
        string type = seatedSpectators[i].getSpectatorType();
        if (type == "VIP") vipSeated++;
        else if (type == "Influencer") influencerSeated++;
        else generalSeated++;
    }
    
    // Calculate utilization rates
    double vipUtilization = (double)(vipSeats - seatStatus.vipAvailable) / vipSeats * 100;
    double influencerUtilization = (double)(influencerSeats - seatStatus.influencerAvailable) / influencerSeats * 100;
    double generalUtilization = (double)(generalSeats - seatStatus.generalAvailable) / generalSeats * 100;
    double overallUtilization = (double)occupiedSeats / totalSeats * 100;
    
    cout << fixed << setprecision(1);
    cout << "Seat Utilization:\n";
    cout << "- VIP: " << vipUtilization << "% (" << vipSeated << " seated)\n";
    cout << "- Influencer: " << influencerUtilization << "% (" << influencerSeated << " seated)\n";
    cout << "- General: " << generalUtilization << "% (" << generalSeated << " seated)\n";
    cout << "- Overall: " << overallUtilization << "% (" << occupiedSeats << "/" << totalSeats << ")\n";
    cout << "\nQueue Status:\n";
    cout << "- Waiting spectators: " << waitingQueue->getSize() << "\n";
    cout << "- Queue capacity utilization: " << (waitingQueue->getSize() > 0 ? "Active" : "Empty") << "\n";
}

void SpectatorManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to create file " << filename << endl;
        return;
    }
    
    // Write header
    file << "Name,Email,Type,ArrivalTime,SeatSection,IsSeated\n";
    
    // Write seated spectators
    for (int i = 0; i < occupiedSeats; i++) {
        file << seatedSpectators[i].toString() << "\n";
    }
    
    file.close();
    cout << "Data saved to " << filename << " successfully!\n";
}

void SpectatorManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    int loadedCount = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, email, type, arrivalTimeStr, seatSection, isSeatedStr;
        
        // Parse CSV line
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, type, ',');
        getline(ss, arrivalTimeStr, ',');
        getline(ss, seatSection, ',');
        getline(ss, isSeatedStr, ',');
        
        // Convert string to int
        int arrivalTime = stoi(arrivalTimeStr);
        bool isSeated = (isSeatedStr == "1");
        
        // Create spectator object
        Spectator spectator(name, email, type, arrivalTime);
        
        if (isSeated && !seatSection.empty()) {
            // Add to seated spectators
            spectator.setSeatSection(seatSection);
            spectator.setIsSeated(true);
            
            // Check if we have space
            if (occupiedSeats < totalSeats) {
                seatedSpectators[occupiedSeats] = spectator;
                occupiedSeats++;
                
                // Update seat availability
                if (type == "VIP") {
                    seatStatus.vipAvailable--;
                } else if (type == "Influencer") {
                    seatStatus.influencerAvailable--;
                } else {
                    seatStatus.generalAvailable--;
                }
            }
        } else {
            // Add to waiting queue
            waitingQueue->insert(spectator);
        }
        
        loadedCount++;
    }
    
    file.close();
    cout << "Loaded " << loadedCount << " spectators from " << filename << endl;
    cout << "- Seated: " << occupiedSeats << endl;
    cout << "- In queue: " << waitingQueue->getSize() << endl;
}

void SpectatorManager::displayMenu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "   APUEC SPECTATOR MANAGEMENT SYSTEM\n";
    cout << string(50, '=') << "\n";
    cout << "1. Register New Spectator\n";
    cout << "2. Allocate Seating (Process Queue)\n";
    cout << "3. Display Waiting Queue\n";
    cout << "4. Display Seated Spectators\n";
    cout << "5. Display Venue Status\n";
    cout << "6. Display System Statistics\n";
    cout << "7. Save Data to File\n";
    cout << "8. Load Data from File\n";
    cout << "9. Exit System\n";
    cout << string(50, '-') << "\n";
    cout << "Enter your choice (1-9): ";
}

void SpectatorManager::runSystem() {
    int choice;
    string filename;
    
    cout << "Welcome to APUEC Spectator Management System!\n";
    cout << "Venue Capacity: " << totalSeats << " seats (VIP: " << vipSeats 
         << ", Influencer: " << influencerSeats << ", General: " << generalSeats << ")\n";
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                registerSpectator();
                break;
            case 2:
                allocateSeating();
                break;
            case 3:
                displayWaitingQueue();
                break;
            case 4:
                displaySeatedSpectators();
                break;
            case 5:
                displayVenueStatus();
                break;
            case 6:
                displayStatistics();
                break;
            case 7:
                cout << "Enter filename to save (e.g., spectators.csv): ";
                cin >> filename;
                saveToFile(filename);
                break;
            case 8:
                cout << "Enter filename to load (e.g., spectators.csv): ";
                cin >> filename;
                loadFromFile(filename);
                break;
            case 9:
                cout << "Thank you for using APUEC Spectator Management System!\n";
                break;
            default:
                cout << "Invalid choice! Please enter 1-9.\n";
        }
        
        if (choice != 9) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 9);
}
