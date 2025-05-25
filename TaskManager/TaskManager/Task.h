#pragma once

#include "ChronoDate.h"
#include <string>


class Task
{
public:
	Task() = default;
	Task(const std::string& name_, int priority_, const std::string& description_,
		const ChronoDate& start_date_, const ChronoDate& end_date_, const ChronoDate& due_date_);
	Task(const std::string& name_, int priority_, const std::string& description_, const ChronoDate& due_date_);
	Task(const std::string& name_, int priority_, const ChronoDate& due_date_);
	~Task() = default;



	void setName(const std::string& name_) { name = name_; }
	void setPriority(int priority_) { priority = priority_; }
	void setDescription(const std::string& description_) { description = description_; }
	void setStartDate(const ChronoDate& start_date_) { start_date = start_date_; }
	void setEndDate(const ChronoDate& end_date_) { end_date = end_date_; }
	void setDueDate(const ChronoDate& due_date_) { due_date = due_date_; }
	void setCompleted(bool completed_) { completed = completed_; }
	void setProgress(float progress_) { progress = progress_; }

	std::string getName() const { return name; }
	int getPriority() const { return priority; }
	void getDescription(std::string& description_) const { description_ = description; }
	void getStartDate(ChronoDate& start_date_) const { start_date_ = start_date; }
	void getEndDate(ChronoDate& end_date_) const { end_date_ = end_date; }
	const ChronoDate& getDueDate() const { return due_date; }
	void setCompleted(bool& completed_) const { completed_ = completed; }
	void setProgress(float& progress_) const { progress_ = progress; }

	void finishTask() { completed = true; progress = 100.0f; }



protected:
	void daysBehindAhead(int& days_behind_ahead, const ChronoDate& today) const;

private:
	std::string name;
	int priority;
	std::string description;
	ChronoDate start_date, end_date, due_date;
	bool completed = false;
	float progress = 0.0f; // Progress percentage (0.0 to 100.0)
};