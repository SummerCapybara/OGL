#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T = int> requires std::integral<T> class ThreadQueue {
	std::queue<T> Queue;
	std::condition_variable cv;
	std::mutex mutex;

  public:
	void push(T value) {
		std::lock_guard<std::mutex> lock(mutex);
		Queue.push(value);
		cv.notify_one();
	}
	void pop() {
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [this] { return !Queue.empty(); });
		Queue.pop();
		lock.unlock();
	}
};