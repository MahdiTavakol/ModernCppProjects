#include "Task.h"

Task::Task(const std::string &name_, int priority_,
           const std::string &description_, const ChronoDate &start_date_,
           const ChronoDate &end_date_, const ChronoDate &due_date_)
    : name{name_}, priority{priority_}, description{description},
      start_date{start_date_}, end_date{end_date_}, due_date{due_date_} {}

Task::Task(const std::string &name_, int priority_,
           const std::string &description_, const ChronoDate &due_date_)
    : name{name_}, priority{priority_},
      description{description_}, due_date{due_date_} {}

Task::Task(const std::string &name_, int priority_, const ChronoDate &due_date_)
    : name{name_}, priority{priority_}, due_date{due_date_} {}

Task::Task(const std::string &name_, int priority_, const ChronoDate &due_date_,
           const Category &cat_)
    : name{name_}, priority{priority_}, due_date{due_date_}, cat{cat_} {
  if (cat == Category::ALL) {
    throw std::runtime_error("Category 'ALL' is not valid for a task.");
  }
}

Task::Task(const std::string &name_, int priority_,
           const std::string &description_, const ChronoDate &start_date_,
           const ChronoDate &due_date_)
    : name{name_}, priority{priority_}, start_date{start_date_},
      description{description_}, due_date{due_date_}, start_date_set{true} {}

void Task::daysBehindAhead(int &days_behind_ahead,
                           const ChronoDate &today) const {
  if (!start_date_set) {
    throw std::runtime_error("Start and end dates are not set for this task.");
  }
  double expected_progress;
  int task_duration_days = due_date - start_date;
  double progress_per_day = 100.0 / task_duration_days;
  if (today < start_date) {
    expected_progress = 0.0;
    days_behind_ahead = start_date - today;
  } else {
    int diff = today - start_date;
    expected_progress = static_cast<double>(diff) * progress_per_day;
    days_behind_ahead =
        static_cast<int>((expected_progress - progress) / progress_per_day);
  }
}
