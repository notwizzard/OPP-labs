#include <mpi.h>
#include <pthread.h>
#include <queue>
#include "Task.hpp"

enum ThreadServiceState {
    WORKING = 1,
    TERMINATING = -1,
};

enum CrossProcessActionType {
    REQUEST,
    RESPONSE,
};

extern pthread_mutex_t mutex;
extern pthread_cond_t workerCondition;
extern pthread_cond_t receiverCondition;
extern std::queue<Task> tasks;
extern int processId;
extern int processesNumber;
extern int state;

class ThreadService {
private:
    pthread_t workerThreadId;
    pthread_t receiverThreadId;
    pthread_t senderThreadId;

    static void* workerExecute();
    static void* receiverExecute();
    static void* senderExecute();

public:
    void init();
    void generateTasks();
    void run();
    void execute();

    ThreadService(int processId, int processesNumber);
    
};
