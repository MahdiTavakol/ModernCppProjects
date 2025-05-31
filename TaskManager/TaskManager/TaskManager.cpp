#include "TaskManager.h"

#include <sstream>
#include <algorithm>
#include <iostream>


TaskManager::TaskManager(const std::string& filename)
{
	Initialize();
	loadFromFile(filename);
	sort_key = SortKey::PRIORITY;
	sortTasks();
}

TaskManager::TaskManager(const std::string& filename, SortKey _sort_key) : sort_key{ _sort_key }
{
	Initialize();
	loadFromFile(filename);
	sortTasks();
}

TaskManager::~TaskManager()
{
    saveToFile("Tasks.txt");
}


void TaskManager::Initialize()
{
    Tasks.clear();
}

void TaskManager::sortTasks()
{
	SortKey sort_key_ = this->sort_key;
    auto sortLambda = [&sort_key_](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) -> bool {
            if (b->getCompleted()) return true;
            switch (sort_key_)
            {
                case SortKey::PRIORITY:
                    return a->getPriority() > b->getPriority();
                case SortKey::DUEDATE:
                    return a->getDueDate() > b->getDueDate();
                case SortKey::NAME:
                    return a->getName() > b->getName();
            }
        };

    std::sort(Tasks.begin(), Tasks.end(), sortLambda);
	currentTaskIterator = Tasks.begin();
}

void TaskManager::completeTask()
{
    if (currentTaskIterator != Tasks.end())
    {
        currentTaskIterator->get()->finishTask();
    }
    sortTasks();
}

void TaskManager::addTask(std::unique_ptr<Task>&& newTask)
{
    Tasks.push_back(std::move(newTask));
    sortTasks();
}

void TaskManager::addTask(std::string title, int priority, const std::string& due_date_str)
{
	ChronoDate due_date;
	std::stringstream ss(due_date_str);
	ss >> due_date;
    std::unique_ptr<Task> newTask = std::make_unique<Task>(std::move(title), priority, std::move(due_date));
	Tasks.push_back(std::move(newTask));
    sortTasks();
}

void TaskManager::finishTaskByTitle(const std::string& title)
{
    auto ip = std::find_if(Tasks.begin(), Tasks.end(), [&title](const std::unique_ptr<Task>& task)
        {return task->getName() == title; }
    );
    if (ip != Tasks.end()) {
        ip->get()->finishTask();
    }
    else
    {
        std::cout << "Task with title " << title << " not found." << std::endl;
    }
    sortTasks();
}

void TaskManager::listTasks() const
{
    if (Tasks.empty()) {
		std::cout << "No tasks available." << std::endl;
		return;
    }

    std::cout << "Tasks List:" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Task Name\tPriority\tDue Date\tProgress" << std::endl;

    for (auto& task : Tasks)
    {
		std::cout << std::format("{}\t{}\t{}\t{}%\n",
			task->getName(),
			task->getPriority(),
			task->getDueDate().ymd(),
			task->getProgress())
            << std::endl;
    }
}

void TaskManager::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "TaskName,Priority,DueDate" << std::endl;
    for (const auto& task : Tasks) {
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
    }

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
        Tasks.push_back(std::move(newTask));
    }
}

template<DaysBehindAhead mode>
void TaskManager::printBehindAheadTasks(const ChronoDate& today) const
{
    switch (mode)
    {
    case DaysBehindAhead::BEHIND:
		std::cout << "Tasks behind schedule:" << std::endl;
		break;
	case DaysBehindAhead::AHEAD:
		std::cout << "Tasks ahead of schedule:" << std::endl;
		break;
	case DaysBehindAhead::ON_TIME:
		std::cout << "Tasks on time:" << std::endl;
		break;
    }

    
	for (const auto& task : Tasks)
	{
		int days_behind_ahead;
		task->daysBehindAhead(days_behind_ahead, today);
		if (days_behind_ahead < 0 && mode == DaysBehindAhead::BEHIND) {
			std::cout << task->getName() << " is " << -days_behind_ahead << " days behind." << std::endl;
		}
        else if (days_behind_ahead > 0 && mode == DaysBehindAhead::AHEAD) {
            std::cout << task->getName() << " is " << days_behind_ahead << " days ahead." << std::endl;
        }
		else if (days_behind_ahead == 0 && mode == DaysBehindAhead::ON_TIME) {
			std::cout << task->getName() << " is on time." << std::endl;
		}
	}
}

