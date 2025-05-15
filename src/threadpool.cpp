#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include "threadpool.h"

ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        workers.emplace_back([this] { //creates new worker which waites for tasks
            for(;;) {
                std::unique_lock<std::mutex> lock(queue_mutex);

                //locks tasks to single thread. Puts threads to sleep if no tasks or shutingdow
                condition.wait(lock, [this] {return !tasks.empty() || shutdown; });

                if (tasks.empty() && shutdown) {
                    return;
                }

                auto task = std::move(tasks.front()); //takes task from front of the queue
                tasks.pop();

                lock.unlock();
                task();
            }
        });
    }
}
    
ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    shutdown = true;
    lock.unlock();
    condition.notify_all(); //Wakes up all threads

    for (auto& worker: workers) {
        worker.join(); //Waits for workers to finish task
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex); //Locks tasks_queue so only single thread can modify it
        tasks.emplace(std::move(task)); //task is added to tasks queue
    } 

    condition.notify_one(); //notifies single worker thread of new task if any
}



