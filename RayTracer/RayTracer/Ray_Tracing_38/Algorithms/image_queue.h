#pragma once
#include "image.h"
#include <mutex>
#include <queue>
#include <condition_variable>

// at this moment as there is not any useful
// methods or objects inside the image to be
// used in the image_queue.
// So currently I do not inherite that from the image.
// This might change in the future


// cg ==>> coarse grained

class image_queue_cg_strategy1 
{
public:
	image_queue_cg_strategy1() = default;


	void push(image new_image_)
	{
		std::lock_guard<std::mutex> lk(mut);
		que.push(std::move(new_image_));
		cond.notify_one();
	}

	void wait_and_pop(image& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(que.front());
		que.pop();
	}

	std::shared_ptr<image> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::shared_ptr<image> res{
			std::make_shared<image>(std::move(que.front()))
		};
		que.pop();
		return res;
	}

	bool try_pop(image& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(que.front());
		que.pop();
		return true;
	}

	std::shared_ptr<image> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::shared_ptr<image>();
		std::shared_ptr<image> res{
			std::make_shared<image>(
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
	std::queue<image> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};

class image_queue_cg_strategy2
{
public:
	image_queue_cg_strategy2() = default;


	void push(image new_image_)
	{
		std::shared_ptr<image> data{
			std::make_shared<image>(std::move(new_image_))
		};
		std::lock_guard<std::mutex> lk(mut);
		que.push(data);
		cond.notify_one();
	}

	void wait_and_pop(image& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(*que.front());
		que.pop();
	}

	std::shared_ptr<image> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::shared_ptr<image> res = que.front();
		que.pop();
		return res;
	}

	bool try_pop(image& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(*que.front());
		que.pop();
		return true;
	}

	std::shared_ptr<image> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::shared_ptr<image>();
		std::shared_ptr<image> res = que.front();
		que.pop();
		return res;

	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return que.empty();
	}

protected:
	std::queue<shared_ptr<image>> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};


class image_queue_cg_strategy3
{
public:
	image_queue_cg_strategy3() = default;


	void push(image new_image_)
	{
		std::unique_ptr<image> data{
			std::make_unique<image>(std::move(new_image_))
		};
		std::lock_guard<std::mutex> lk(mut);
		que.push(std::move(data));
		cond.notify_one();
	}

	void wait_and_pop(image& value_)
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		value_ = std::move(*que.front());
		que.pop();
	}

	std::unique_ptr<image> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [this] {return !que.empty(); });
		std::unique_ptr<image> res;
		res = std::move(que.front());
		que.pop();
		return res;
	}

	bool try_pop(image& value_)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return false;
		value_ = std::move(*que.front());
		que.pop();
		return true;
	}

	std::unique_ptr<image> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (que.empty())
			return std::unique_ptr<image>();
		std::unique_ptr<image> res;
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
	std::queue<std::unique_ptr<image>> que;
	mutable std::mutex mut;
	std::condition_variable cond;
};

class image_queue_fg
{
public:
	image_queue_fg():
		head(new node), tail(head.get())
	{}
	image_queue_fg(const image_queue_fg& other_) = delete;
	image_queue_fg& operator=(const image_queue_fg& other_) = delete;

	bool try_pop(std::unique_ptr<image>& img_, std::unique_ptr<output>& wrt_)
	{
		std::unique_ptr<node> old_head = try_pop_head(img_, wrt_);
		return old_head.get();
	}

	void wait_and_pop(std::unique_ptr<image>& img_, std::unique_ptr<output>& wrt_)
	{
		std::unique_ptr<node> old_head = wait_pop_head(img_,wrt_);
	}

	bool empty()
	{
		std::lock_guard<std::mutex> head_lock{ head_mutex };
		return (head.get() == get_tail());
	}

	void push(std::unique_ptr<image> img_, std::unique_ptr<output> wrt_)
	{
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->img = std::move(img_);
			tail->wrt = std::move(wrt_);
			node* const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}
		cond.notify_one();
	}


private:
	struct node
	{
		std::unique_ptr<image> img;
		std::unique_ptr<output> wrt;
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

	std::unique_ptr<node> try_pop_head(std::unique_ptr<image>& img_, std::unique_ptr<output>& wrt_)
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<node>();
		}
		img_ = std::move(head->img);
		wrt_ = std::move(head->wrt);
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
	std::unique_ptr<node> wait_pop_head(std::unique_ptr<image>& img_, std::unique_ptr<output>& wrt_)
	{
		std::unique_lock<std::mutex> head_lock{ wait_for_data() };
		img_ = std::move(head->img);
		wrt_ = std::move(head->wrt);
		return pop_head();
	}
};
