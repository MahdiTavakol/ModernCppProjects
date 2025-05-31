#include "TaskManager.h"

#include <sstream>
#include <algorithm>
#include <array>


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
    saveToFile("Tasks-desructor.txt");
}


void TaskManager::Initialize()
{
    Tasks.clear();
}

void TaskManager::sortTasks()
{
	SortKey sort_key_ = this->sort_key;
    auto sortLambda = [&sort_key_](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) -> bool {
            // Completed tasks always come after incomplete ones
            if (a->getCompleted() != b->getCompleted()) {
                return !a->getCompleted(); // incomplete < complete
            }
            
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
        (*currentTaskIterator)->finishTask();
        sortTasks();
    }
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
        (*ip)->finishTask();
    }
    else
    {
        std::cout << "Task with title " << title << " not found." << std::endl;
    }
    sortTasks();
}

void TaskManager::listTasks(const std::vector<std::unique_ptr<Task>>& inputTasks, const std::string& description, const Category& cat) const
{
    if (inputTasks.empty()) {
        std::cout << description << " is empty." << std::endl;
		std::cout << "No tasks available." << std::endl;
		return;
    }

	std::array<std::string, 4> headers = {
		"Task Name", "Priority", "Due Date", "Progress"
	};
    std::cout << description << ":" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << std::format("{:<20}\t{:^10}\t{:^15}\t{:^8}(%)\n",headers[0],headers[1],headers[2],headers[3]) << std::endl;

    auto print_line = [](const std::unique_ptr<Task>& task) {
        std::cout << std::format("{:<20}\t{:^10}\t{:^15}\t{:^8.1f}\n",
            task->getName(),
            task->getPriority(),
            task->getDueDate().ymd(),
            task->getProgress())
            << std::endl;
        };

    if (cat == Category::ALL) {
        for (auto& task : inputTasks)
        {
            print_line(task);
        }
        return;
    }

    std::for_each(inputTasks.begin(), inputTasks.end(), [&cat,&print_line](const std::unique_ptr<Task>& task) {
        if (task->getCategory() == cat)
            print_line(task);
        });
}

std::vector<std::unique_ptr<Task>> TaskManager::filterTasks(const Category& cat) const
{
    std::vector<std::unique_ptr<Task>> filteredTasks;
    std::for_each(Tasks.begin(), Tasks.end(), [&cat, &filteredTasks](const std::unique_ptr<Task>& task) {
            if (task->getCategory() == cat) {
			    filteredTasks.push_back(std::make_unique<Task>(*task));
            }
        });
    return std::move(filteredTasks);
}

void TaskManager::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    std::array<std::string, 4> headers = {
    "Task Name", "Priority", "Due Date", "Progress"
    };
	file << std::format("{:<20}, {:^10}, {:^15}, {:^8}\n", headers[0], headers[1], headers[2], headers[3]) << std::endl;
    for (const auto& task : Tasks) {
        std::stringstream ss;
		std::string due_date_str;
		ss << task->getDueDate();
		ss >> due_date_str;
        file << std::format("{:<20} {:^10} {:^15} {:^8}\n", task->getName(),task->getPriority(),due_date_str,task->getProgress());
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
    std::cout << std::endl << std::endl << std::endl;
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
        const ChronoDate due_date_from_task = Tasks[Tasks.size() - 1]->getDueDate();
    }
}

void TaskManager::printHighPriorityTasks(const int& threshold) const
{
    std::vector<std::unique_ptr<Task>> highPriorityTasks;
	highPriorityTasks.reserve(Tasks.size()); // Reserve space to avoid multiple allocations
    std::for_each(Tasks.begin(), Tasks.end(), [&threshold, &highPriorityTasks](const std::unique_ptr<Task>& task)
        {if (task->getPriority() >= threshold)  highPriorityTasks.push_back(std::make_unique<Task>(*task)); });

    // a lighter implementation might look like this;
	std::vector<TaskStruct> highPriorityTasksStructs;
    highPriorityTasksStructs.reserve(highPriorityTasks.size());
    
    auto conditional_append = [&threshold, &highPriorityTasksStructs](const std::unique_ptr<Task>& task)
		{
			if (task->getPriority() >= threshold) {
                TaskStruct taskStruct{ task->getName(), task->getPriority(), task->getDueDate(), task->getProgress() };
				highPriorityTasksStructs.push_back(taskStruct);
			}
		};

    //std::for_each(Tasks.begin(), Tasks.end(), conditional_append);
    // End of the lighter version. 

	this->listTasks(highPriorityTasks, "High Priority Tasks (Priority >= " + std::to_string(threshold) + ")");
}

