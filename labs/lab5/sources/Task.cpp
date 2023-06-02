#include "Task.hpp"

Task::Task() {
    
}

Task::Task(int parentProcessId)
    : parentProcessId(parentProcessId) {}

Task::Task(int parentProcessId, int taskType) 
    : parentProcessId(parentProcessId), taskType(taskType) {}

Task::Task(int parentProcessId, int taskType, int weight) 
    : parentProcessId(parentProcessId), taskType(taskType), weight(weight) {}

const int & Task::getTaskType() const {
    return this->taskType;
}

const int &Task::getWeight() const
{
    return this->weight;
}

const int &Task::getParentProcessId() const
{
    return this->parentProcessId;
}

std::ostream& operator<<(std::ostream& os, const Task &task) {
    os << "(task)<ParentProcessId : " << task.getParentProcessId() << " | Weight : " << task.getWeight() << ">";
    return os;
}
