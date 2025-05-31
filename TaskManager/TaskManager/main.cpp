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

	std::cout << "Starting Task Manager..." << std::endl;
	std::unique_ptr<TaskManager> taskManagerPtr = std::make_unique<TaskManager>("Tasks.txt", SortKey::PRIORITY);
	std::cout << "Listing Tasks from the Task Manager..." << std::endl;
	taskManagerPtr->listTasks();
	std::cout << "Adding a new task..." << std::endl;
	taskManagerPtr->addTask("Test Task", 1, "2023-10-31");
	std::cout << "Finishing a task..." << std::endl;
	taskManagerPtr->finishTaskByTitle("Test Task");
	std::cout << "Listing Tasks after adding and finishing a task..." << std::endl;
	taskManagerPtr->listTasks();
	std::cout << "Finishing a task with a title that does not exist..." << std::endl;
	taskManagerPtr->finishTaskByTitle("Nonexistent Task");
	std::cout << "Listing tasks..." << std::endl;
	taskManagerPtr->listTasks();
	std::cout << "Completing two tasks (a) ..." << std::endl;
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();
	std::cout << "Listing tasks after completing two tasks..." << std::endl;
	taskManagerPtr->listTasks();

	std::cout << "Adding 20 random tasks..." << std::endl;
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

	std::cout << "Listing tasks after adding 20 random tasks..." << std::endl;
	taskManagerPtr->listTasks();
	std::cout << "Completing two tasks (b) ..." << std::endl;
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();

	std::cout << "Adding 20 more random tasks..." << std::endl;
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
	std::cout << "Listing tasks after adding 20 more random tasks..." << std::endl;
	taskManagerPtr->listTasks();
	std::cout << "Completing two tasks (c) ..." << std::endl;
	taskManagerPtr->completeTask();
	taskManagerPtr->completeTask();


	std::cout << "Saving tasks to file..." << std::endl;
	taskManagerPtr->saveToFile("Tasks-saved.txt");
}