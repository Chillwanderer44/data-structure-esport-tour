#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include <string>

template <typename T>
class Stack {
private:
    static const int MAX_SIZE_STAT = 1000;
    T data[MAX_SIZE_STAT];
    int top;

public:
    Stack() : top(-1) {}

    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == MAX_SIZE_STAT - 1; }

    void push(const T& item) {
        if (!isFull()) data[++top] = item;
    }

    void pop() {
        if (!isEmpty()) --top;
    }

    T peek() const {
        if (!isEmpty()) return data[top];
        return T();
    }

    int size() const { return top + 1; }
};

void displayAllMatches();
void displayTeamMatches(const std::string& teamName);
void displayTeamStats(const std::string& teamName);

#endif
