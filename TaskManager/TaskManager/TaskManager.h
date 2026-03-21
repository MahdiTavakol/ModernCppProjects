#pragma once

#include "Task.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <string>

enum class SortKey {
  PRIORITY,
  DUEDATE,
  NAME,
};

enum class DaysBehindAhead { BEHIND = -1, AHEAD = 1, ON_TIME = 0 };

class TaskManager {
public:
  TaskManager() = default;
  TaskManager(const std::string &filename);
  TaskManager(const std::string &filename, SortKey _sort_key);

  TaskManager(const TaskManager &) = delete;
  TaskManager &operator=(const TaskManager &) = delete;
  TaskManager(TaskManager &&) = default;
  TaskManager &operator=(TaskManager &&) = default;
  ~TaskManager();

  void completeTask();
  void addTask(std::unique_ptr<Task> &&newTask);
  void addTask(std::string title, int priority,
               const std::string &due_date_str);
  void finishTaskByTitle(const std::string &title);

  void listTasks(const std::vector<std::unique_ptr<Task>> &inputTasks,
                 const std::string &description,
                 const Category &cat = Category::ALL) const;
  void listTasks() const { listTasks(Tasks, "Tasks List"); }
  void listTasks(const std::string &description,
                 const Category &cat = Category::ALL) const {
    listTasks(Tasks, description, cat);
  }
  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  void printHighPriorityTasks(const int &threshold = 500) const;
  std::vector<std::unique_ptr<Task>> filterTasks(const Category &cat) const;

  template <DaysBehindAhead mode>
  void printBehindAheadTasks(const ChronoDate &today) const {
    switch (mode) {
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

    for (const auto &task : Tasks) {
      if (!(task->start_date_set))
        continue;
      int days_behind_ahead;
      task->daysBehindAhead(days_behind_ahead, today);
      if (days_behind_ahead < 0 && mode == DaysBehindAhead::BEHIND) {
        std::cout << task->getName() << " is " << -days_behind_ahead
                  << " days behind." << std::endl;
      } else if (days_behind_ahead > 0 && mode == DaysBehindAhead::AHEAD) {
        std::cout << task->getName() << " is " << days_behind_ahead
                  << " days ahead." << std::endl;
      } else if (days_behind_ahead == 0 && mode == DaysBehindAhead::ON_TIME) {
        std::cout << task->getName() << " is on time." << std::endl;
      }
    }
  }

private:
  std::vector<std::unique_ptr<Task>> Tasks;
  std::vector<std::unique_ptr<Task>>::iterator currentTaskIterator;
  SortKey sort_key;

  void Initialize();
  void sortTasks();
};
