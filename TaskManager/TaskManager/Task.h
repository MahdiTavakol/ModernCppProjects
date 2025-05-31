#pragma once

#include "ChronoDate.h"
#include <string>

enum class Category
{
	WORK,
	PERSONAL,
	STUDY,
	OTHER,
	ALL // Used for filtering tasks, should not have any tasks with that
};

struct TaskStruct
{
	std::string name;
	int priority;
	ChronoDate due_date;
	float progress;
};

class Task
{
public:
	Task() = default;
	Task(const std::string& name_, int priority_, const std::string& description_,
		const ChronoDate& start_date_, const ChronoDate& end_date_, const ChronoDate& due_date_);
	Task(const std::string& name_, int priority_, const std::string& description_, const ChronoDate& due_date_);
	Task(const std::string& name_, int priority_, const ChronoDate& due_date_);
	Task(const std::string& name_, int priority_, const ChronoDate& due_date_, const Category& cat_);
	Task(const std::string& name_, int priority_, const std::string& description_,const ChronoDate& start_date_,  const ChronoDate& due_date_);


	Task(const Task&) = default;
	Task& operator=(const Task&) = default;
	Task(Task&&) = default;
	Task& operator=(Task&&) = default;
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
	bool getCompleted() const { return completed; }
	float getProgress() const { return progress; }

	void finishTask() { completed = true; progress = 100.0f; }

	void daysBehindAhead(int& days_behind_ahead, const ChronoDate& today) const;

	bool start_date_set = false;

	void setCategory(Category cat_) {	this->cat = cat_; }
	Category getCategory() const { return cat; }

	

protected:
	

private:
	Category cat = Category::OTHER; // Default category
	std::string name;
	int priority;
	std::string description = "No descriptions yet!";
	ChronoDate start_date, end_date, due_date;
	bool completed = false;
	
	float progress = 0.0f; // Progress percentage (0.0 to 100.0)
};