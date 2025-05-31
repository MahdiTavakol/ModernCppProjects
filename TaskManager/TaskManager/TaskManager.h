#pragma once

#include "Task.h"

#include <string>
#include <set>
#include <memory>
#include <map>
#include <iterator>
#include <fstream>

enum class SortKey {
    PRIORITY = 0,
	DUEDATE = 1<<1,
	NAME = 1 << 2,
};

enum class DaysBehindAhead {
	BEHIND = -1,
	AHEAD = 1,
	ON_TIME = 0
};

class TaskManager {
public:
    TaskManager() = default;
    TaskManager(const std::string& filename);
    TaskManager(const std::string& filename, SortKey _sort_key);


	TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    TaskManager(TaskManager&&) = default;
    TaskManager& operator=(TaskManager&&) = default;
	~TaskManager();


    
    void completeTask();
    void addTask(std::unique_ptr<Task>&& newTask);
    void addTask(std::string title, int priority, const std::string& due_date_str);
    void finishTaskByTitle(const std::string& title);

    void listTasks() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    template<DaysBehindAhead mode>
	void printBehindAheadTasks(const ChronoDate& today) const;

    

private:
    std::vector <std::unique_ptr<Task>> Tasks;
    std::vector <std::unique_ptr<Task>>::iterator currentTaskIterator;
    SortKey sort_key;


    void Initialize();
    void sortTasks();
};