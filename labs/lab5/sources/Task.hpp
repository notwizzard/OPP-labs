#include <iostream>

enum TaskType {
    EMPTY_QUEUE = -1,
    TASK = 0,
};

class Task {
private:
    int taskType = TASK;
    int weight = 0;
    int parentProcessId;

public:
    Task();
    Task(int parentProcessId);
    Task(int parentProcessId, int taskType);
    Task(int parentProcessId, int taskType, int weight);

    const int & getTaskType() const;
    const int & getWeight() const;
    const int & getParentProcessId() const;
};

std::ostream& operator<<(std::ostream& os, const Task &task);
