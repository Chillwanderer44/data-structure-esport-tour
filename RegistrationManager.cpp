#include "RegistrationManager.hpp"

void RegistrationManager::registerTeam() {
    Team newTeam;
    int choice;

    cout << "Enter Team Name: ";
    cin.ignore();
    cin.getline(newTeam.name, 100);

    cout << "Select Registration Type:\n";
    cout << "1. Normal\n";
    cout << "2. Wild Card\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        int currentNormal = normalQueue.size();
        if (currentNormal < 20) {
            strcpy(newTeam.status, "early bird");
        } else {
            strcpy(newTeam.status, "normal");
        }
        normalQueue.enqueue(newTeam);
    } else if (choice == 2) {
        strcpy(newTeam.status, "wild card");
        wildCardQueue.enqueue(newTeam);
    } else {
        cout << "Invalid option.\n";
    }

    // Save state after each registration
    saveToCSV("registration.csv");
}

void RegistrationManager::saveToCSV(const char* filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Failed to open file.\n";
        return;
    }

    for (int i = 0; i < normalQueue.size(); i++) {
        Team t = normalQueue.peek(i);
        file << t.name << "," << t.status << endl;
    }

    for (int i = 0; i < wildCardQueue.size(); i++) {
        Team t = wildCardQueue.peek(i);
        file << t.name << "," << t.status << endl;
    }

    file.close();
}

void RegistrationManager::endRegistration(const char* outputFilename) {
    ifstream file("registration.csv");
    if (!file.is_open()) {
        cout << "Cannot open registration.csv\n";
        return;
    }

    TeamQueue earlyBirdQueue;
    TeamQueue normalStatusQueue;
    TeamQueue wildCardStatusQueue;

    string line;
    while (getline(file, line)) {
        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string name = line.substr(0, comma);
        string status = line.substr(comma + 1);

        Team t;
        strcpy(t.name, name.c_str());
        strcpy(t.status, status.c_str());

        if (status == "early bird") {
            earlyBirdQueue.enqueue(t);
        } else if (status == "normal") {
            normalStatusQueue.enqueue(t);
        } else if (status == "wild card") {
            wildCardStatusQueue.enqueue(t);
        }
    }

    file.close();

    int total = earlyBirdQueue.size() + normalStatusQueue.size() + wildCardStatusQueue.size();
    if (total < 96) {
        cout << "Cannot end registration: only " << total << " teams registered (need 96)\n";
        return;
    }

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cout << "Failed to write to " << outputFilename << endl;
        return;
    }

    int count = 0;
    while (!earlyBirdQueue.isEmpty() && count < 96) {
        Team t = earlyBirdQueue.dequeue();
        outFile << t.name << "," << t.status << endl;
        count++;
    }
    while (!normalStatusQueue.isEmpty() && count < 96) {
        Team t = normalStatusQueue.dequeue();
        outFile << t.name << "," << t.status << endl;
        count++;
    }
    while (!wildCardStatusQueue.isEmpty() && count < 96) {
        Team t = wildCardStatusQueue.dequeue();
        outFile << t.name << "," << t.status << endl;
        count++;
    }

    outFile.close();
    cout << "Successfully selected top 96 teams into " << outputFilename << endl;
}

void RegistrationManager::withdrawTeam(const char* teamName) {
    ifstream file("registration.csv");
    if (!file.is_open()) {
        cout << "Cannot open registration.csv\n";
        return;
    }

    TeamQueue tempQueue;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string name = line.substr(0, comma);
        string status = line.substr(comma + 1);

        if (name == teamName) {
            found = true;
            continue; // Skip adding this team to the new queue
        }

        Team t;
        strcpy(t.name, name.c_str());
        strcpy(t.status, status.c_str());
        tempQueue.enqueue(t);
    }

    file.close();

    if (!found) {
        cout << "Team \"" << teamName << "\" not found.\n";
        return;
    }

    ofstream outFile("registration.csv");
    while (!tempQueue.isEmpty()) {
        Team t = tempQueue.dequeue();
        outFile << t.name << "," << t.status << endl;
    }
    outFile.close();

    cout << "Team \"" << teamName << "\" successfully withdrawn.\n";
}

void RegistrationManager::replaceTeam(const char* oldName, const char* newName) {
    ifstream file("registration.csv");
    if (!file.is_open()) {
        cout << "Cannot open registration.csv\n";
        return;
    }

    TeamQueue tempQueue;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string name = line.substr(0, comma);
        string status = line.substr(comma + 1);

        Team t;
        if (name == oldName) {
            found = true;
            strcpy(t.name, newName); // Replacing name
        } else {
            strcpy(t.name, name.c_str());
        }

        strcpy(t.status, status.c_str());
        tempQueue.enqueue(t);
    }

    file.close();

    if (!found) {
        cout << "Team \"" << oldName << "\" not found.\n";
        return;
    }

    ofstream outFile("registration.csv");
    while (!tempQueue.isEmpty()) {
        Team t = tempQueue.dequeue();
        outFile << t.name << "," << t.status << endl;
    }
    outFile.close();

    cout << "Team \"" << oldName << "\" successfully replaced by \"" << newName << "\".\n";
}
