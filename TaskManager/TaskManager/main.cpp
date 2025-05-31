#include <memory>
#include <iostream>
#include <random>

#include "TaskManager.h"


void main()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> pr(1, 1000);
	std::uniform_int_distribution<> da(1, 27);
	std::uniform_int_distribution<> mn(1, 12);
	std::uniform_int_distribution<> yr(2020, 2025);

	std::unique_ptr<TaskManager> taskManagerPtr = std::make_unique<TaskManager>("Tasks.txt", SortKey::PRIORITY);
	taskManagerPtr->listTasks();
	taskManagerPtr->addTask("Test Task", 1, "2023-10-31");
	taskManagerPtr->finishTaskByTitle("Test Task");
	taskManagerPtr->listTasks();
	taskManagerPtr->finishTaskByTitle("Nonexistent Task");
	taskManagerPtr->listTasks();
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();
	taskManagerPtr->listTasks();

	for (int i = 0; i < 20; i++)
	{
		int priority = pr(gen);
		unsigned day = static_cast<unsigned> (da(gen));
		unsigned month = static_cast<unsigned> (mn(gen));
		int year = yr(gen);
		ChronoDate due_date(year, month, day);
		std::string title = "Task " + std::to_string(i + 1);
		std::string discription = "This is a description for " + title;
		std::unique_ptr<Task> newTask = std::make_unique<Task>(title, priority, discription, due_date);
		taskManagerPtr->addTask(std::move(newTask));
	}
	taskManagerPtr->listTasks();
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();

	for (int i = 0; i < 20; i++)
	{
		int priority = pr(gen);
		unsigned day = static_cast<unsigned> (da(gen));
		unsigned month = static_cast<unsigned> (mn(gen));
		int year = yr(gen);
		ChronoDate due_date(year, month, day);
		std::string title = "Task " + std::to_string(i + 1);
		std::string discription = "This is a description for " + title;
		std::stringstream due_date_ss;
		std::string due_date_str;
		due_date_ss << due_date;
		due_date_ss >> due_date_str;
		taskManagerPtr->addTask(std::move(title), priority, std::move(due_date_str));
	}
	taskManagerPtr->listTasks();
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();



	taskManagerPtr->saveToFile("Tasks-saved.txt");
}