#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>



// cg ==>> coarse grained

template<typename T>
class queue_1 
{
public:
	queue_1() = default;

	
	void push(T new_value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		que.push(std::move(new_value_));
		cond.notify_one();
	}

	void wait_and_pop(T& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(que.front());
		que.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::shared_ptr<T> res{
			std::make_shared<T>(std::move(que.front()))
		};
		que.pop();
		return res;
	}

	bool try_pop(T& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(que.front());
		que.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res{
			std::make_shared<T>(
				std::move(que.front())) };
		que.pop();
		return res;

	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return que.empty();
	}

protected:
	std::queue<T> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};


template<typename T>
class queue_2
{
public:
	queue_2() = default;


	void push(T new_value_)
	{
		std::shared_ptr<T> data{
			std::make_shared<T>(std::move(new_value_))
		};
		std::lock_guard<std::mutex> lk(mut);
		que.push(data);
		cond.notify_one();
	}

	void wait_and_pop(T& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(*que.front());
		que.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::shared_ptr<T> res = que.front();
		que.pop();
		return res;
	}

	bool try_pop(T& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(*que.front());
		que.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res = que.front();
		que.pop();
		return res;

	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return que.empty();
	}

protected:
	std::queue<shared_ptr<T>> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};

template<typename T>
class queue_3
{
public:
	queue_3() = default;


	void push(T new_value_)
	{
		std::unique_ptr<T> data{
			std::make_unique<T>(std::move(new_value_))
		};
		std::lock_guard<std::mutex> lk(mut);
		que.push(std::move(data));
		cond.notify_one();
	}

	void wait_and_pop(T& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(*que.front());
		que.pop();
	}

	std::unique_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::unique_ptr<T> res;
		res = std::move(que.front());
		que.pop();
		return res;
	}

	bool try_pop(T& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(*que.front());
		que.pop();
		return true;
	}

	std::unique_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::unique_ptr<T>();
		std::unique_ptr<T> res;
		res = std::move(que.front());
		que.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return que.empty();
	}

protected:
	std::queue<std::unique_ptr<T>> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};


template<typename T>
class queue_4
{
public:
	queue_4():
		head(new node), tail(head.get())
	{}
	queue_4(const queue_4& other_) = delete;
	queue_4& operator=(const queue_4& other_) = delete;

	bool try_pop(std::unique_ptr<T>& value_)
	{
		std::unique_ptr<node> old_head = try_pop_head(value_);
		return old_head.get();
	}

	void wait_and_pop(std::unique_ptr<T>& value_)
	{
		std::unique_ptr<node> old_head = wait_pop_head(value_);
	}

	bool empty()
	{
		std::lock_guard<std::mutex> head_lock{ head_mutex };
		return (head.get() == get_tail());
	}

	void push(std::unique_ptr<T> value_)
	{
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->value = std::move(value_);
			node* const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}
		cond.notify_one();
	}


private:
	struct node
	{
		std::unique_ptr<T> value;
		std::unique_ptr<node> next;
	};
	std::unique_ptr<node> head;
	node* tail;

	std::mutex head_mutex;
	std::mutex tail_mutex;
	std::condition_variable cond;

	node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	std::unique_ptr<node> try_pop_head()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<node>();
		}
		return pop_head();
	}

	std::unique_ptr<node> try_pop_head(std::unique_ptr<T>& value_)
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<node>();
		}
		value_ = std::move(head->value);
		return pop_head();
	}
	
	std::unique_ptr<node> pop_head()
	{
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}
	std::unique_lock<std::mutex> wait_for_data()
	{
		std::unique_lock<std::mutex> head_lock{ head_mutex };
		cond.wait(head_lock, [&] {return head.get() != get_tail(); });
		return std::move(head_lock);
	}
	std::unique_ptr<node> wait_pop_head()
	{
		std::unique_lock<std::mutex>head_lock{ wait_for_data() };
		return pop_head();
	}
	std::unique_ptr<node> wait_pop_head(std::unique_ptr<T>& value_)
	{
		std::unique_lock<std::mutex> head_lock{ wait_for_data() };
		value_ = std::move(head->value);
		return pop_head();
	}
};
