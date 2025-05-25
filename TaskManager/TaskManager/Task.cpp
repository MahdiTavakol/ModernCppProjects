#include "Task.h"

Task::Task(const std::string& name_, int priority_, const std::string& description_,
	const ChronoDate& start_date_, const ChronoDate& end_date_, const ChronoDate& due_date_)
	: name{ name_ }, priority{ priority_ }, description{ description },
	start_date{ start_date }, end_date{ end_date }, due_date{ due_date } {}

Task::Task(const std::string& name_, int priority_, const std::string& description_, const ChronoDate& due_date_)
	: name{ name_ }, priority{ priority_ }, description{ description }, due_date{ due_date } {}

Task::Task(const std::string& name_, int priority_, const ChronoDate& due_date_)
	: name{ name_ }, priority{ priority_ }, due_date{ due_date } {}

void Task::daysBehindAhead(int& days_behind_ahead, const ChronoDate& today) const {

	double expected_progress;
	ChronoDate task_duration = end_date - start_date;
	int task_duration_days = task_duration.day();
	double progress_per_day = 100.0 / task_duration_days;
	if (today < start_date) {
		expected_progress = 0.0;
		days_behind_ahead = task_duration_days;
	}
	else if (today > end_date) {
		expected_progress = 100.0;
		days_behind_ahead = -task_duration_days;
	}
	else {
		auto diff = today - start_date;
		expected_progress = diff * progress_per_day;
		days_behind_ahead = (expected_progress - progress) / progress_per_day;
	}
}