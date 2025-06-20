#include "../../include/ThreadPool/ThreadPool.hpp"
#include "../../include/Logger/Logger.hpp"  // Adjust the path if needed

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    Logger::getInstance().log("Initializing ThreadPool with " + std::to_string(threads) + " threads.", LoggerLevel::INFO);

    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this, i] {
            Logger::getInstance().log("Worker thread " + std::to_string(i) + " started.", LoggerLevel::DEBUG);

            while (!stop) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });
                    if (this->stop && this->tasks.empty()) {
                        Logger::getInstance().log("Worker thread " + std::to_string(i) + " is stopping (no more tasks).", LoggerLevel::DEBUG);
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                Logger::getInstance().log("Worker thread " + std::to_string(i) + " is executing a task.", LoggerLevel::DEBUG);
                task();
            }

            Logger::getInstance().log("Worker thread " + std::to_string(i) + " exited loop.", LoggerLevel::DEBUG);
        });
    }

    Logger::getInstance().log("ThreadPool initialization complete.", LoggerLevel::INFO);
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::move(task));
        Logger::getInstance().log("New task enqueued. Total pending tasks: " + std::to_string(tasks.size()), LoggerLevel::DEBUG);
    }
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    Logger::getInstance().log("ThreadPool is shutting down...", LoggerLevel::INFO);

    stop = true;
    condition.notify_all();

    for (size_t i = 0; i < workers.size(); ++i) {
        if (workers[i].joinable()) {
            workers[i].join();
            Logger::getInstance().log("Worker thread " + std::to_string(i) + " has been joined.", LoggerLevel::DEBUG);
        }
    }

    Logger::getInstance().log("ThreadPool shut down completed.", LoggerLevel::INFO);
}
