#include "TaskManager.h"

#include <sstream>
#include <algorithm>


TaskManager::TaskManager(const std::string& filename)
{
	Initialize();
	loadFromFile(filename);
}

TaskManager::~TaskManager()
{
    saveToFile("Tasks.txt");
}


void TaskManager::Initialize()
{
	Taskset.clear();
	currentTaskIterator = Taskset.begin();
}

void TaskManager::sortTasks(SortKey key = SortKey::PRIORITY)
{
    auto sortLambda = [&key](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) -> bool {
            switch (key)
            {
            case SortKey::PRIORITY:
                return a->getPriority() > b->getPriority();
            case SortKey::DUEDATE:
                return a->getDueDate() > b->getDueDate();
            case SortKey::NAME:
                return a->getName() > b->getName();
            }
        };
    std::sort(Taskset.begin(), Taskset.end(), sortLambda);
}

void TaskManager::completeTask()
{
    if (currentTaskIterator != Taskset.end())
    {
        currentTaskIterator->get()->finishTask();
        ++currentTaskIterator;
    }
}

void TaskManager::addTask(std::string title, int priority, std::string due_date)
{
    std::unique_ptr<Task> newTask = std::make_unique<Task>(std::move(title), priority, std::move(due_date));
    Taskset.push_back(std::move(newTask));
}

void TaskManager::removeTask(const std::string& title)
{
    auto ip = std::find_if(Taskset.begin(), Taskset.end(), [&title](const std::unique_ptr<Task>& task) {return task->getName() == title; });
    if (ip != Taskset.end()) {
        ip->get()->finishTask();
    }
}

void TaskManager::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "TaskName,Priority,DueDate" << std::endl;
    for (const auto& task : Taskset) {
        file << task->getName() << ","
            << task->getPriority() << ","
            << task->getDueDate() << std::endl;
    }
}

void TaskManager::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filename);

    std::string line;
	std::getline(file, line); // Skip header line

    int task_number = -1;
    while (std::getline(file, line))
    {
        task_number++;
        std::stringstream ss(line);
        std::string name, priority_str, due_date_str;
        ChronoDate due_date;
        int priority;

        if (!(std::getline(ss, name, ',') &&
              std::getline(ss, priority_str,',') &&
              std::getline(ss,due_date_str,',')))
            throw std::runtime_error("Error reading the task number " + std::to_string(task_number) + "from the file : " + filename + ". Please check the file format");
		priority = std::stoi(priority_str);
		std::stringstream ss2(due_date_str);
		ss2 >> due_date;
        std::unique_ptr<Task> newTask = std::make_unique<Task>(std::move(name), priority, std::move(due_date));
        Taskset.push_back(std::move(newTask));
    }
}

