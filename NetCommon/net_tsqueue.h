#pragma once
#include"net_common.h"

// queue needs to be thread safe because multiple connection might wanna write at the same time
// also the queue is implemented using locks -> no reading while something is bing written in the queue
namespace net
{
	template<typename T>
	class tsqueue
	{
	public:
		tsqueue() = default;
		tsqueue(const tsqueue<T>&) = delete;				// deleting the copy constructor, since I have mutexes
		virtual ~tsqueue() { clear(); }

	public:
		const T& front()
		{
			std::scoped_lock lock(muxQueue);				// protects anything else from running
			return deqQueue.front();
		}
		
		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.back();
		}

		void push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_back(std::move(item));
		}
		
		void push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_front(std::move(item));
		}

		bool empty()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.empty();
		}

		size_t count()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.size();
		}

		void clear()
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.clear();
		}

		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = atd::move(deqQueue.back());
			deqQueue.pop_back();
			return t;
		}

	protected:
		std::mutex muxQueue;
		std::deque<T> deqQueue;
	};
}