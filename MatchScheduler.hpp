#ifndef MATCHSCHEDULER_HPP
#define MATCHSCHEDULER_HPP

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MAX_TEAMS = 128;

struct MatchTeam {
    char name[50];
    char status[20];
    int points;
};

// ===================== Queue =====================
class QueueMatch {
private:
    MatchTeam data[MAX_TEAMS];
    int front, rear;

public:
    QueueMatch() : front(0), rear(0) {}

    bool isEmpty() {
        return front == rear;
    }

    bool isFull() {
        return (rear + 1) % MAX_TEAMS == front;
    }

    void enqueue(MatchTeam team) {
        if (!isFull()) {
            data[rear] = team;
            rear = (rear + 1) % MAX_TEAMS;
        }
    }

    MatchTeam dequeue() {
        MatchTeam t = data[front];
        front = (front + 1) % MAX_TEAMS;
        return t;
    }
};

// =================== Stack ===================
class StackMatch {
private:
    MatchTeam data[MAX_TEAMS];
    int top;

public:
    StackMatch() : top(-1) {}

    bool isEmpty() { return top == -1; }

    void push(MatchTeam t) {
        if (top < MAX_TEAMS - 1) {
            data[++top] = t;
        }
    }

    MatchTeam pop() {
        return data[top--];
    }

    MatchTeam peek() {
        return data[top];
    }
};

// ================= PriorityQueue =================
class PriorityQueueMatch {
private:
    MatchTeam data[MAX_TEAMS];
    int count;

    int getPriority(const char* status) {
        if (strcmp(status, "early bird") == 0) return 3;
        if (strcmp(status, "normal") == 0) return 2;
        if (strcmp(status, "wild card") == 0) return 1;
        return 0;
    }

public:
    PriorityQueueMatch() : count(0) {}

    bool isEmpty() { return count == 0; }

    void insert(MatchTeam team) {
        int i = count - 1;
        while (i >= 0 && getPriority(team.status) > getPriority(data[i].status)) {
            data[i + 1] = data[i];
            i--;
        }
        data[i + 1] = team;
        count++;
    }

    MatchTeam remove() {
        return data[--count];
    }
};

class CircularQueueMatch {
    private:
        static const int MAX = 128;
        MatchTeam data[MAX];
        int front, rear, count;
    
    public:
        CircularQueueMatch() : front(0), rear(0), count(0) {}
    
        bool isEmpty() {
            return count == 0;
        }
    
        bool isFull() {
            return count == MAX;
        }
    
        void enqueue(const MatchTeam& team) {
            if (isFull()) return;
            data[rear] = team;
            rear = (rear + 1) % MAX;
            count++;
        }
    
        MatchTeam dequeue() {
            if (isEmpty()) return MatchTeam();  // return empty team
            MatchTeam t = data[front];
            front = (front + 1) % MAX;
            count--;
            return t;
        }
    
        int size() {
            return count;
        }
    
        // Access by index (for random shuffle or transfer)
        MatchTeam& operator[](int index) {
            return data[(front + index) % MAX];
        }
    
        void swap(int i, int j) {
            MatchTeam temp = (*this)[i];
            (*this)[i] = (*this)[j];
            (*this)[j] = temp;
        }
    };

// ================ MatchScheduler =================
class MatchScheduler {
private:
    MatchTeam teams[MAX_TEAMS];
    int teamCount;
    ofstream resultFile;

    int randomWinner(int a, int b);
    void printMatch(const MatchTeam& t1, const MatchTeam& t2, const MatchTeam& winner);
    void knockoutRound(int &numTeams);
    void groupStage(MatchTeam allTeams[12], MatchTeam finalists[6]);
    void knockoutStage(MatchTeam finalists[6], int size);
    void logMatchResult(const char* teamA, const char* teamB, const char* winner);
    void readTeams(const char* filename);
    int findTeamIndex(MatchTeam arr[], int size, const char* name);

public:
    MatchScheduler();
    void startTournament(const char* filename);
};

#endif
