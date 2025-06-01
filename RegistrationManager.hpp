#ifndef REGISTRATION_MANAGER_HPP
#define REGISTRATION_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <cstring>

#define MAX_SIZE 200

using namespace std;

// Simple struct for Team
struct Team {
    char name[100];
    char status[20];
};

// Queue implementation for Team
class TeamQueue {
private:
    Team data[MAX_SIZE];
    int front, rear;

public:
    TeamQueue() {
        front = 0;
        rear = -1;
    }

    bool isEmpty() {
        return rear < front;
    }

    bool isFull() {
        return rear == MAX_SIZE - 1;
    }

    void enqueue(Team team) {
        if (!isFull()) {
            data[++rear] = team;
        }
    }

    Team dequeue() {
        if (!isEmpty()) {
            return data[front++];
        } else {
            Team t = {"", ""};
            return t;
        }
    }

    int size() const {
        return rear - front + 1;
    }

    Team peek(int index) const {
        if (index >= 0 && (front + index) <= rear)
            return data[front + index];
        else {
            Team t = {"", ""};
            return t;
        }
    }
};

// Stack for Team
class TeamStack {
private:
    Team data[MAX_SIZE];
    int topIndex;

public:
    TeamStack() {
        topIndex = -1;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == MAX_SIZE - 1;
    }

    void push(Team team) {
        if (!isFull()) {
            data[++topIndex] = team;
        }
    }

    Team pop() {
        if (!isEmpty()) {
            return data[topIndex--];
        } else {
            Team t = {"", ""};
            return t;
        }
    }

    Team peek() {
        if (!isEmpty()) {
            return data[topIndex];
        } else {
            Team t = {"", ""};
            return t;
        }
    }
};

// Circular Queue for Team
class TeamCircularQueue {
private:
    Team data[MAX_SIZE];
    int front, rear, count;

public:
    TeamCircularQueue() {
        front = 0;
        rear = -1;
        count = 0;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == MAX_SIZE;
    }

    void enqueue(Team team) {
        if (!isFull()) {
            rear = (rear + 1) % MAX_SIZE;
            data[rear] = team;
            count++;
        }
    }

    Team dequeue() {
        if (!isEmpty()) {
            Team temp = data[front];
            front = (front + 1) % MAX_SIZE;
            count--;
            return temp;
        } else {
            Team t = {"", ""};
            return t;
        }
    }

    int size() const {
        return count;
    }
};

// Priority Queue for Team
class TeamPriorityQueue {
private:
    Team data[MAX_SIZE];
    int sizeVal;

    int getPriority(const Team& t) const {
        if (strcmp(t.status, "early bird") == 0) return 1;
        if (strcmp(t.status, "normal") == 0) return 2;
        return 3; // wild card
    }

public:
    TeamPriorityQueue() {
        sizeVal = 0;
    }

    bool isEmpty() {
        return sizeVal == 0;
    }

    bool isFull() {
        return sizeVal == MAX_SIZE;
    }

    void enqueue(Team t) {
        if (!isFull()) {
            int i = sizeVal - 1;
            while (i >= 0 && getPriority(data[i]) > getPriority(t)) {
                data[i + 1] = data[i];
                i--;
            }
            data[i + 1] = t;
            sizeVal++;
        }
    }

    Team dequeue() {
        if (!isEmpty()) {
            return data[--sizeVal];
        } else {
            Team t = {"", ""};
            return t;
        }
    }

    int size() const {
        return sizeVal;
    }

    Team peek(int index) const {
        if (index >= 0 && index < sizeVal)
            return data[index];
        else {
            Team t = {"", ""};
            return t;
        }
    }
};

class RegistrationManager {
private:
    TeamQueue normalQueue;
    TeamQueue wildCardQueue;

public:
    void registerTeam();
    void saveToCSV(const char* filename);
    void endRegistration(const char* outputFilename);
    void withdrawTeam(const char* teamName);
    void replaceTeam(const char* oldName, const char* newName);

};

#endif
