//
// Created by Han on 27/11/2024.
//

#ifndef PARALLELTIMEDFIFO_H
#define PARALLELTIMEDFIFO_H

#include <assert.h>
#include<deque>
#include<mutex>
#define FIFO_DEPTH_INFINITE 0xffffffff

static uint32_t getCurrentClock() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

template<typename T>
class ParallelTimedFifo {
public:
    ParallelTimedFifo() = delete;
    ParallelTimedFifo(const uint32_t latency, const uint32_t depth) : m_depth(depth), m_latency(latency) {}
    void setDepth(const uint32_t depth) {
        assert(depth != 0);
        m_depth = depth;
    }
    void setLatency(const uint32_t latency) {
        m_latency = latency;
    }
    bool valid() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return !m_data.empty() && m_data.front().time <= getCurrentClock();
    }
    void write(const T& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        assert(m_data.size() <= m_depth);
        uint32_t expected_pop_time = getCurrentClock() + m_latency;
        if(expected_pop_time == m_last_pop_time) {
            ++expected_pop_time;
        }
        m_data.emplace_back(TimedElement{data, expected_pop_time});
    }
    T read() {
        std::lock_guard<std::mutex> lock(m_mutex);
        assert(!m_data.empty() && m_data.front().time <= getCurrentClock());
        T out = m_data.front().element;
        m_data.pop_front();
        return out;
    }
    bool read(T& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_data.empty() || m_data.front().time > getCurrentClock()) {
            return false;
        }
        data = m_data.front().element;
        m_data.pop_front();
        return true;
    }
    T& peek_front() {
        std::lock_guard<std::mutex> lock(m_mutex);
        assert(!m_data.empty() && m_data.front().time <= getCurrentClock());
        return m_data.front().element;
    }

private:
    uint32_t m_depth = FIFO_DEPTH_INFINITE;
    uint32_t m_latency = 1;
    uint32_t m_last_pop_time = 0;
    struct TimedElement {
        uint32_t time = 0;
        T element;
    };
    std::deque<TimedElement> m_data;
    std::mutex m_mutex;
};


class InterfaceConnect {
public:

private:

};

#endif //PARALLELTIMEDFIFO_H
