#include "MatchScheduler.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstring>

MatchScheduler::MatchScheduler() {
    teamCount = 0;
    srand((unsigned int)time(NULL));
    resultFile.open("result.csv");
    if (!resultFile.is_open()) {
        cout << "Error opening result.csv for logging." << endl;
    } else {
        resultFile << "Team A,Team B,Winner\n";
    }
}

void MatchScheduler::readTeams(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        teamCount = 0;
        return;
    }

    PriorityQueueMatch pq;
    string line;
    teamCount = 0;
    while (getline(file, line) && teamCount < MAX_TEAMS) {
        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string tname = line.substr(0, comma);
        string tstatus = line.substr(comma + 1);

        MatchTeam t;
        strncpy(t.name, tname.c_str(), 49);
        t.name[49] = '\0';
        strncpy(t.status, tstatus.c_str(), 19);
        t.status[19] = '\0';
        t.points = 0;

        pq.insert(t);
        teamCount++;
    }
    file.close();

    int i = 0;
    while (!pq.isEmpty() && i < 96) {
        teams[i++] = pq.remove();
    }
    teamCount = i;

    if (teamCount < 96) {
        cout << "Not enough teams in " << filename << " (need at least 96)" << endl;
        teamCount = 0;
    }
}

int MatchScheduler::randomWinner(int a, int b) {
    return (rand() % 2 == 0) ? a : b;
}

void MatchScheduler::printMatch(const MatchTeam& t1, const MatchTeam& t2, const MatchTeam& winner) {
    cout << "Match: [" << t1.name << "] VS [" << t2.name << "] --> Winner: [" << winner.name << "]\n";
    logMatchResult(t1.name, t2.name, winner.name);
}

void MatchScheduler::logMatchResult(const char* teamA, const char* teamB, const char* winner) {
    if (resultFile.is_open()) {
        resultFile << teamA << "," << teamB << "," << winner << "\n";
    }
}

void MatchScheduler::knockoutRound(int& numTeams) {
    cout << "\n=== Knockout Round: " << numTeams << " Teams ===\n";

    QueueMatch queue;
    for (int i = 0; i < numTeams; i++) {
        queue.enqueue(teams[i]);
    }

    int nextRoundCount = 0;
    while (!queue.isEmpty()) {
        if (queue.isEmpty()) break;
        MatchTeam t1 = queue.dequeue();
        if (queue.isEmpty()) {
            teams[nextRoundCount++] = t1;
            break;
        }
        MatchTeam t2 = queue.dequeue();
        int winnerIndex = randomWinner(0, 1);
        MatchTeam winner = (winnerIndex == 0) ? t1 : t2;
        printMatch(t1, t2, winner);
        teams[nextRoundCount++] = winner;
    }
    numTeams = nextRoundCount;
}

static void shuffleTeams(MatchTeam arr[], int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        MatchTeam temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void MatchScheduler::groupStage(MatchTeam allTeams[12], MatchTeam finalists[6]) {
    cout << "\n=== Group Stage ===\n";

    // Step 1: Load and shuffle using CircularQueue
    CircularQueueMatch teamPool;
    for (int i = 0; i < 12; ++i) {
        teamPool.enqueue(allTeams[i]);
    }

    // Shuffle by rotating
    for (int i = 0; i < 24; ++i) {
        int rotateTimes = rand() % 12;
        for (int j = 0; j < rotateTimes; ++j) {
            MatchTeam temp = teamPool.dequeue();
            teamPool.enqueue(temp);
        }
    }

    // Step 2: Distribute into 3 groups
    MatchTeam groupTeams[3][4];
    for (int g = 0; g < 3; ++g) {
        for (int i = 0; i < 4; ++i) {
            MatchTeam t = teamPool.dequeue();
            t.points = 0;
            groupTeams[g][i] = t;
        }
    }

    // Step 3: Display teams in each group
    for (int g = 0; g < 3; ++g) {
        cout << "\nGroup " << (g + 1) << ":\n";
        for (int i = 0; i < 4; ++i) {
            cout << "  - " << groupTeams[g][i].name << "\n";
        }
    }

    int finalistIndex = 0;

    // Step 4: Simulate matches group by group
    for (int g = 0; g < 3; ++g) {
        cout << "\n-- Group " << (g + 1) << " Matches --\n";

        bool advanced[4] = {false, false, false, false};
        int qualifiedCount = 0;

        while (qualifiedCount < 2) {
            for (int i = 0; i < 3; ++i) {
                for (int j = i + 1; j < 4; ++j) {
                    if (advanced[i] || advanced[j]) continue;

                    MatchTeam& teamA = groupTeams[g][i];
                    MatchTeam& teamB = groupTeams[g][j];
                    int winnerIdx = randomWinner(0, 1);
                    MatchTeam& winner = (winnerIdx == 0) ? teamA : teamB;

                    printMatch(teamA, teamB, winner);
                    winner.points++;

                    if (winner.points == 3 && !advanced[&winner - groupTeams[g]]) {
                        advanced[&winner - groupTeams[g]] = true;
                        finalists[finalistIndex++] = winner;
                        cout << " >> " << winner.name << " advances with 3 points!\n";
                        qualifiedCount++;
                        if (qualifiedCount == 2) break;
                    }
                }
                if (qualifiedCount == 2) break;
            }
        }
    }

    // Step 5: Finalist summary
    cout << "\n=== Finalists advancing to Knockout Stage ===\n";
    for (int i = 0; i < 6; ++i) {
        cout << (i + 1) << ". " << finalists[i].name << " (Points: " << finalists[i].points << ")\n";
    }
}


int MatchScheduler::findTeamIndex(MatchTeam arr[], int size, const char* name) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i].name, name) == 0) return i;
    }
    return -1;
}

void MatchScheduler::knockoutStage(MatchTeam finalists[6], int size) {
    cout << "\n=== Knockout Stage ===\n";

    // Step 1: Load finalists into CircularQueue and shuffle
    CircularQueueMatch teamQueue;
    for (int i = 0; i < size; ++i) {
        teamQueue.enqueue(finalists[i]);
    }

    // Shuffle: dequeue-rotate-enqueue
    for (int i = 0; i < 12; ++i) {
        int r = rand() % size;
        for (int j = 0; j < r; ++j) {
            MatchTeam t = teamQueue.dequeue();
            teamQueue.enqueue(t);
        }
    }

    // Step 2: Quarterfinals - 3 matches
    QueueMatch quarterWinners;
    for (int i = 0; i < 3; ++i) {
        MatchTeam t1 = teamQueue.dequeue();
        MatchTeam t2 = teamQueue.dequeue();
        int winnerIndex = randomWinner(0, 1);
        MatchTeam winner = (winnerIndex == 0) ? t1 : t2;
        printMatch(t1, t2, winner);
        quarterWinners.enqueue(winner);
    }

    // Step 3: Semifinal - 1 match + 1 bye
    MatchTeam semi1 = quarterWinners.dequeue();
    MatchTeam semi2 = quarterWinners.dequeue();
    MatchTeam byeTeam = quarterWinners.dequeue();  // gets auto pass to final

    MatchTeam finalist1;
    int semifinalWinner = randomWinner(0, 1);
    finalist1 = (semifinalWinner == 0) ? semi1 : semi2;
    printMatch(semi1, semi2, finalist1);

    cout << ">> " << byeTeam.name << " gets a BYE to the Final!\n";

    // Step 4: Final
    MatchTeam champion;
    int finalWinner = randomWinner(0, 1);
    champion = (finalWinner == 0) ? finalist1 : byeTeam;
    printMatch(finalist1, byeTeam, champion);

    cout << "\n=== TOURNAMENT WINNER: " << champion.name << " ===\n";
}


void MatchScheduler::startTournament(const char* filename) {
    readTeams(filename);
    if (teamCount == 0) {
        cout << "No teams available to start tournament.\n";
        return;
    }

    int currentTeams = 96;
    knockoutRound(currentTeams);
    knockoutRound(currentTeams);
    knockoutRound(currentTeams);

    // Prepare teams for group stage
    MatchTeam groupTeams[12];
    for (int i = 0; i < 12; ++i) {
        groupTeams[i] = teams[i];
    }

    MatchTeam finalists[6];
    groupStage(groupTeams, finalists);
    knockoutStage(finalists, 6);

    if (resultFile.is_open()) {
        resultFile.close();
    }
}
