#include "Statistic.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void displayAllMatches() {
    ifstream file("result.csv");
    if (!file.is_open()) {
        cerr << "Failed to open result.csv" << endl;
        return;
    }

    Stack<string> matchStack;
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        matchStack.push(line);
    }

    cout << "All Matches (latest first):\n";
    cout << "Team A vs Team B -> Winner\n";
    while (!matchStack.isEmpty()) {
        string record = matchStack.peek();
        matchStack.pop();

        stringstream ss(record);
        string teamA, teamB, winner;
        getline(ss, teamA, ',');
        getline(ss, teamB, ',');
        getline(ss, winner, ',');

        cout << teamA << " vs " << teamB << " -> " << winner << endl;
    }

    file.close();
}

void displayTeamMatches(const string& teamName) {
    ifstream file("result.csv");
    if (!file.is_open()) {
        cerr << "Failed to open result.csv" << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header
    cout << "Matches for " << teamName << ":\n";
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string teamA, teamB, winner;
        getline(ss, teamA, ',');
        getline(ss, teamB, ',');
        getline(ss, winner, ',');

        if (teamA == teamName || teamB == teamName) {
            cout << teamA << " vs " << teamB << " -> Winner: " << winner << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No matches found for team " << teamName << "." << endl;
    }

    file.close();
}

void displayTeamStats(const string& teamName) {
    ifstream file("result.csv");
    if (!file.is_open()) {
        cerr << "Failed to open result.csv" << endl;
        return;
    }

    int totalMatches = 0, wins = 0, losses = 0;
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string teamA, teamB, winner;
        getline(ss, teamA, ',');
        getline(ss, teamB, ',');
        getline(ss, winner, ',');

        if (teamA == teamName || teamB == teamName) {
            totalMatches++;
            if (winner == teamName)
                wins++;
            else
                losses++;
        }
    }

    cout << "Statistics for " << teamName << ":\n";
    cout << "Total Matches: " << totalMatches << endl;
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;

    if (totalMatches > 0) {
        double winRate = (static_cast<double>(wins) / totalMatches) * 100.0;
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "Win Rate: " << winRate << "%" << endl;
    } else {
        cout << "Win Rate: N/A (no matches played)" << endl;
    }

    file.close();
}
