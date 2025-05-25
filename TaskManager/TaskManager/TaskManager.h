#pragma once

#include "Task.h"

#include <string>
#include <set>
#include <memory>
#include <iterator>
#include <fstream>

enum class SortKey {
    PRIORITY = 0,
	DUEDATE = 1<<1,
	NAME = 1 << 2,
};

class TaskManager {
public:
    TaskManager() = default;
    TaskManager(const std::string& filename);


	TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    TaskManager(const TaskManager&&) = default;
    TaskManager& operator=(TaskManager&&) = default;
	~TaskManager();


    void Initialize();
    void sortTasks(SortKey key = SortKey::PRIORITY);
    void completeTask();
    void addTask(std::string title, int priority, std::string due_date);
    void removeTask(const std::string& title);

    void listTasks() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    

private:

    std::vector<std::unique_ptr<Task>> Taskset;
    std::vector<std::unique_ptr<Task>>::iterator currentTaskIterator;
};