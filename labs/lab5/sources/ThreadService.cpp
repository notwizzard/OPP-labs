#include "ThreadService.hpp"
#include <iostream>
#include <unistd.h>
typedef void * (*THREADFUNCPTR)(void *);

pthread_mutex_t mutex;
pthread_cond_t workerCondition;
pthread_cond_t receiverCondition;
std::queue<Task> tasks;
int processId;
int processesNumber;
int state = WORKING;

void ThreadService::init()
{
    pthread_mutex_init(&mutex, NULL);

    int workerStatus = pthread_create(&workerThreadId, NULL, (THREADFUNCPTR) &ThreadService::workerExecute, NULL);
    int receiverStatus = pthread_create(&receiverThreadId, NULL, (THREADFUNCPTR) &ThreadService::receiverExecute, NULL);
    int senderStatus = pthread_create(&senderThreadId, NULL, (THREADFUNCPTR) &ThreadService::senderExecute, NULL);
    if (workerStatus | receiverStatus | senderStatus != 0) {
        std::cerr << "can't create thread" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ThreadService::generateTasks() {

    int taskWeight = (processId + 1) * 100000;
    for (int i = 0; i < 10 * (processId + 1); i++) {
        Task task = Task(processId, TASK, taskWeight);
        tasks.push(task);
    }
}

void ThreadService::run() {
    pthread_cond_init(&workerCondition, NULL);
    pthread_cond_init(&receiverCondition, NULL);

    state = WORKING;

    pthread_join(workerThreadId, NULL);
    pthread_join(receiverThreadId, NULL);
    pthread_join(senderThreadId, NULL);
}

void ThreadService::execute() {
    this->init();
    this->generateTasks();
    MPI_Barrier(MPI_COMM_WORLD);
    this->run();
}

ThreadService::ThreadService(int processId_, int processesNumber_) {
    processId = processId_; 
    processesNumber = processesNumber_;
}

inline void *ThreadService::workerExecute() {

    while (true) {
        while (true) {

            pthread_mutex_lock(&mutex);
            if (tasks.empty()) {
                pthread_mutex_unlock(&mutex);
                break;
            }
            Task task = tasks.front();
            tasks.pop();
            pthread_mutex_unlock(&mutex);

            std::cout << "(process)<Id : " << processId << "> executing " << task << std::endl;
            std::flush(std::cout);
            usleep(task.getWeight());
        }

        pthread_mutex_lock(&mutex);
        while (tasks.empty() && state == WORKING) {
            pthread_cond_signal(&receiverCondition);
            pthread_cond_wait(&workerCondition, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        if (state == TERMINATING) {
            break;
        }
    }
    pthread_exit(NULL);
}

inline void *ThreadService::receiverExecute() {
    std::cout << "State: " << state << std::endl;
    std::flush(std::cout);
    while (state == WORKING) {
        pthread_mutex_unlock(&mutex);
        int receivedTasks = 0;

        pthread_mutex_lock(&mutex);
        while (!tasks.empty()) {
            pthread_cond_wait(&receiverCondition, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        for (int i = 0; i < processesNumber; i++) {
            if (i == processId) {
                continue;
            }

            Task task;
            MPI_Send(&processId, 1, MPI_INT, i, REQUEST, MPI_COMM_WORLD);
            MPI_Recv(&task, sizeof(Task), MPI_BYTE, i, RESPONSE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (task.getTaskType() == TASK) {
                std::cout << "(process)<Id : " << processId << "> received " << task << " from (process)<Id : " << i << ">" << std::endl;
                std::flush(std::cout);
                pthread_mutex_lock(&mutex);
                tasks.push(task);
                pthread_mutex_unlock(&mutex);
                receivedTasks++;
            } else {
                std::cout << "(process)<Id : " << processId << "> received empty from (process)<Id : " << i << ">" << std::endl;
                std::flush(std::cout);
            }
        }

        if (receivedTasks == 0) {
            pthread_mutex_lock(&mutex);
            state = TERMINATING;
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&workerCondition);
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);
    MPI_Barrier(MPI_COMM_WORLD);

    int terminatingRequestProcessId = -1;
    MPI_Send(&terminatingRequestProcessId, 1, MPI_INT, processId, REQUEST, MPI_COMM_WORLD);
    pthread_exit(NULL);
}

inline void *ThreadService::senderExecute() {

    while (true) {
        
        int requestedProcessId;
        MPI_Recv(&requestedProcessId, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (requestedProcessId < 0) {
            break;
        }

        Task task;
        pthread_mutex_lock(&mutex);
        if (!tasks.empty()) {
            task = tasks.front();
            tasks.pop();
        } else {
            task = Task(processId, EMPTY_QUEUE, 0);
        }
        pthread_mutex_unlock(&mutex);
        MPI_Send(&task, sizeof(Task), MPI_BYTE, requestedProcessId, RESPONSE, MPI_COMM_WORLD);
    }
    pthread_exit(NULL);
}
