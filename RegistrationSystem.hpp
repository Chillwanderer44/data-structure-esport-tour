#ifndef REGISTRATIONSYSTEM_HPP
#define REGISTRATIONSYSTEM_HPP

#include <string>

using namespace std;

const int MAX_PLAYERS = 100;  

struct Player {
    string name;
    string playerID;
    bool isEarlyBird;
    bool isWildcard;
    bool isCheckedIn;
};

class RegistrationSystem {
private:
    Player earlyBirdQueue[MAX_PLAYERS];
    Player normalQueue[MAX_PLAYERS];
    Player wildcardStack[MAX_PLAYERS];

    int earlyFront, earlyRear, earlyCount;
    int normalFront, normalRear, normalCount;
    int repFront, repRear, repCount;
    int wildcardTop;  

    void enqueue(Player queue[], int& front, int& rear, int& count, const Player& player);
    bool dequeue(Player queue[], int& front, int& rear, int& count, Player& player);
    int findPlayerInQueue(const Player queue[], int front, int rear, int count, const string& playerID) const;
    bool removeFromQueue(Player queue[], int& front, int& rear, int& count, const string& playerID);

    void pushWildcard(const Player& player);
    bool popWildcard(Player& player);

public:
    //Constructor
    RegistrationSystem();

   
    bool loadFromCSV(const string& filename);

    bool saveToCSV(const string& filename) const;

    bool registerPlayer(const string& name, const string& playerID, bool isEarlyBird);

    bool addWildcardEntry(const string& name, const string& playerID);

    void checkInPlayer(const string& playerID);

    bool withdrawPlayer(const string& playerID);

    bool replacePlayer();

    void displayAllQueues() const;

    void displayCheckedInPlayers() const;
};

#endif 
