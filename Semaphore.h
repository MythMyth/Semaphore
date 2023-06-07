#include<mutex>
#include<condition_variable>

class Semaphore {
public:
    explicit Semaphore(unsigned int desired) : res(desired){}
    Semaphore(const Semaphore& another ) = delete;
    Semaphore(Semaphore&& another) = delete;
    Semaphore& operator= (const Semaphore& another) = delete;
    Semaphore& operator= (Semaphore&& another) = delete;
    ~Semaphore() {}

    void release(const unsigned int amount = 1U) {
        std::lock_guard lock(m_mutex);
        res += amount;
    }

    void acquire(const unsigned int amount = 1U) {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [&]() {return res >= amount;});
        res -= amount;
    }

    bool try_acquire(const unsigned int amount = 1U) {
        std::lock_guard lock(m_mutex);
        if(res >= amount) {
            res -= amount;
            return true;
        }
        return false;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned int res;

};
