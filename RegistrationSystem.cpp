#include "RegistrationSystem.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Constructor
RegistrationSystem::RegistrationSystem() {
    earlyFront = 0;
    earlyRear = -1;
    earlyCount = 0;

    normalFront = 0;
    normalRear = -1;
    normalCount = 0;

    repFront = 0;
    repRear = -1;
    repCount = 0;

    wildcardTop = -1;

    
    loadFromCSV("players.csv");
}

// Load players from CSV file
bool RegistrationSystem::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "CSV file not found: " << filename << "\n";
        
        return false;
    }



    
    earlyFront = 0;
    earlyRear = -1;
    earlyCount = 0;

    normalFront = 0;
    normalRear = -1;
    normalCount = 0;

    repFront = 0;
    repRear = -1;
    repCount = 0;

    wildcardTop = -1;

    string line;
    bool headerSkipped = false;
    while (getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        stringstream ss(line);
        string name, playerID, isEarlyStr, isWildcardStr, isCheckedInStr;

        getline(ss, name, ',');
        getline(ss, playerID, ',');
        getline(ss, isEarlyStr, ',');
        getline(ss, isWildcardStr, ',');
        getline(ss, isCheckedInStr, ',');

        Player player;
        player.name = name;
        player.playerID = playerID;
        player.isEarlyBird = (isEarlyStr == "1");
        player.isWildcard = (isWildcardStr == "1");
        player.isCheckedIn = (isCheckedInStr == "1");

        if (player.isWildcard) {
            pushWildcard(player);
        } else if (player.isEarlyBird) {
            enqueue(earlyBirdQueue, earlyFront, earlyRear, earlyCount, player);
        } else {
            enqueue(normalQueue, normalFront, normalRear, normalCount, player);
        }
    }

    file.close();
    cout << "Loaded players from CSV: " << filename << "\n";
    return true;
}

// Save all players to CSV file
bool RegistrationSystem::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open CSV file for writing: " << filename << "\n";
        return false;
    }

    // Write header
    file << "Name,PlayerID,IsEarlyBird,IsWildcard,IsCheckedIn\n";

    // Early-bird queue
    for (int i = 0, idx = earlyFront; i < earlyCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = earlyBirdQueue[idx];
        file << p.name << "," << p.playerID << ",1,0," << (p.isCheckedIn ? "1" : "0") << "\n";
    }

    // Normal queue
    for (int i = 0, idx = normalFront; i < normalCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = normalQueue[idx];
        file << p.name << "," << p.playerID << ",0,0," << (p.isCheckedIn ? "1" : "0") << "\n";
    }

    // Wildcard stack
    for (int i = 0; i <= wildcardTop; ++i) {
        const Player& p = wildcardStack[i];
        file << p.name << "," << p.playerID << ",0,1,0\n"; 
    }

    

    file.close();
    cout << "Saved players to CSV: " << filename << "\n";
    return true;
}

// Register a new player
bool RegistrationSystem::registerPlayer(const string& name, const string& playerID, bool isEarlyBird) {
    if (name.empty() || playerID.empty()) {
        cout << "Error: Name and ID cannot be empty.\n";
        return false;
    }

    Player newPlayer;
    newPlayer.name = name;
    newPlayer.playerID = playerID;
    newPlayer.isEarlyBird = isEarlyBird;
    newPlayer.isWildcard = false;
    newPlayer.isCheckedIn = false;

    if (isEarlyBird) {
        if (earlyCount >= MAX_PLAYERS) {
            cout << "Early-bird registration full.\n";
            return false;
        }
        enqueue(earlyBirdQueue, earlyFront, earlyRear, earlyCount, newPlayer);
    } else {
        if (normalCount >= MAX_PLAYERS) {
            cout << "Normal registration full.\n";
            return false;
        }
        enqueue(normalQueue, normalFront, normalRear, normalCount, newPlayer);
    }

    cout << "Registered: " << name << " (ID: " << playerID << ") as " 
              << (isEarlyBird ? "Early-bird" : "Normal") << "\n";

    saveToCSV("players.csv");

    return true;
}

// Add a wildcard entry
bool RegistrationSystem::addWildcardEntry(const string& name, const string& playerID) {
    if (wildcardTop + 1 >= MAX_PLAYERS) {
        cout << "Wildcard stack full.\n";
        return false;
    }

    Player newPlayer;
    newPlayer.name = name;
    newPlayer.playerID = playerID;
    newPlayer.isWildcard = true;
    newPlayer.isEarlyBird = false;
    newPlayer.isCheckedIn = false;

    pushWildcard(newPlayer);

    cout << "Wildcard added: " << name << " (ID: " << playerID << ")\n";

    saveToCSV("players.csv");

    return true;
}

// Check in a player
void RegistrationSystem::checkInPlayer(const string& playerID) {
    int index;

    // Early-bird
    index = findPlayerInQueue(earlyBirdQueue, earlyFront, earlyRear, earlyCount, playerID);
    if (index != -1) {
        earlyBirdQueue[index].isCheckedIn = true;
        cout << "Checked in (Early-bird): " << earlyBirdQueue[index].name << "\n";

        saveToCSV("players.csv");
        return;
    }

    // Normal
    index = findPlayerInQueue(normalQueue, normalFront, normalRear, normalCount, playerID);
    if (index != -1) {
        normalQueue[index].isCheckedIn = true;
        cout << "Checked in (Normal): " << normalQueue[index].name << "\n";

        saveToCSV("players.csv");
        return;
    }

    cout << "Player not found for check-in.\n";
}

// Withdraw a player
bool RegistrationSystem::withdrawPlayer(const string& playerID) {
    bool removed = false;

    removed |= removeFromQueue(earlyBirdQueue, earlyFront, earlyRear, earlyCount, playerID);
    removed |= removeFromQueue(normalQueue, normalFront, normalRear, normalCount, playerID);

    if (removed) {
        cout << "Player withdrawn: " << playerID << "\n";

        replacePlayer();

        saveToCSV("players.csv");
    } else {
        cout << "Player not found: " << playerID << "\n";
    }

    return removed;
}

// Replace with a player from wildcard 
bool RegistrationSystem::replacePlayer() {
    Player replacement;

    if (popWildcard(replacement)) {
        cout << "Replacement from wildcard: " << replacement.name << "\n";
        registerPlayer(replacement.name, replacement.playerID, false);
        return true;
    }

    cout << "No replacement available from wildcard.\n";
    return false;
}

// Display all players in all queues
void RegistrationSystem::displayAllQueues() const {
    cout << "\n--- Early-bird Players ---\n";
    for (int i = 0, idx = earlyFront; i < earlyCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = earlyBirdQueue[idx];
        cout << p.name << " (ID: " << p.playerID << ") " << (p.isCheckedIn ? "[Checked-in]" : "") << "\n";
    }

    cout << "\n--- Normal Players ---\n";
    for (int i = 0, idx = normalFront; i < normalCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = normalQueue[idx];
        cout << p.name << " (ID: " << p.playerID << ") " << (p.isCheckedIn ? "[Checked-in]" : "") << "\n";
    }

    cout << "\n--- Wildcard Stack ---\n";
    for (int i = wildcardTop; i >= 0; --i) {
        const Player& p = wildcardStack[i];
        cout << p.name << " (ID: " << p.playerID << ")\n";
    }

    
}

// Display only checked-in players
void RegistrationSystem::displayCheckedInPlayers() const {
    cout << "\n--- Checked-in Early-bird Players ---\n";
    for (int i = 0, idx = earlyFront; i < earlyCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = earlyBirdQueue[idx];
        if (p.isCheckedIn)
            cout << p.name << " (ID: " << p.playerID << ")\n";
    }

    cout << "\n--- Checked-in Normal Players ---\n";
    for (int i = 0, idx = normalFront; i < normalCount; ++i, idx = (idx + 1) % MAX_PLAYERS) {
        const Player& p = normalQueue[idx];
        if (p.isCheckedIn)
            cout << p.name << " (ID: " << p.playerID << ")\n";
    }
}

// Helper queue operations
void RegistrationSystem::enqueue(Player queue[], int& front, int& rear, int& count, const Player& player) {
    if (count == MAX_PLAYERS) return; // Queue full
    rear = (rear + 1) % MAX_PLAYERS;
    queue[rear] = player;
    ++count;
}

bool RegistrationSystem::dequeue(Player queue[], int& front, int& rear, int& count, Player& player) {
    if (count == 0) return false; // Queue empty
    player = queue[front];
    front = (front + 1) % MAX_PLAYERS;
    --count;
    return true;
}

int RegistrationSystem::findPlayerInQueue(const Player queue[], int front, int rear, int count, const string& playerID) const {
    int idx = front;
    for (int i = 0; i < count; ++i) {
        if (queue[idx].playerID == playerID) return idx;
        idx = (idx + 1) % MAX_PLAYERS;
    }
    return -1;
}

bool RegistrationSystem::removeFromQueue(Player queue[], int& front, int& rear, int& count, const string& playerID) {
    if (count == 0) return false;

    int idx = front;
    int foundIndex = -1;
    for (int i = 0; i < count; ++i) {
        if (queue[idx].playerID == playerID) {
            foundIndex = idx;
            break;
        }
        idx = (idx + 1) % MAX_PLAYERS;
    }
    if (foundIndex == -1) return false;

    // Shift elements to fill gap
    while (foundIndex != rear) {
        int nextIndex = (foundIndex + 1) % MAX_PLAYERS;
        queue[foundIndex] = queue[nextIndex];
        foundIndex = nextIndex;
    }

    // Update rear and count
    rear = (rear - 1 + MAX_PLAYERS) % MAX_PLAYERS;
    --count;
    return true;
}

// Wildcard stack operations
void RegistrationSystem::pushWildcard(const Player& player) {
    if (wildcardTop + 1 >= MAX_PLAYERS) return; // Stack full
    wildcardStack[++wildcardTop] = player;
}

bool RegistrationSystem::popWildcard(Player& player) {
    if (wildcardTop < 0) return false; // Stack empty
    player = wildcardStack[wildcardTop--];
    return true;
}
