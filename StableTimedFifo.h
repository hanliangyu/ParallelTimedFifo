//
// Created by Han on 15/02/2025.
//

#ifndef STABLETIMEDFIFO_H
#define STABLETIMEDFIFO_H

#include <string>
#include <utility>
#include <vector>

//! What we want:
//! 1. A stable interface class that can be used for system simulation
//! 2. This interface should behave in a stable manner even if the simulation is run in parallel
//! Basic idea:
//! 1. All observable states of the fifo is determined before each cycle
//! 2. Actions performed by upstream and downstream within the current cycle does not affect the observable states
namespace stf { // Stable Timed Fifo --> stf
    constexpr uint32_t kMaxFifoDepth    = 512;
    constexpr uint32_t kDefaultLatency  = 1;

    template<typename T>
    class StableTimedFifo {
    public:
        StableTimedFifo() = delete;
        explicit StableTimedFifo(std::string name, const uint32_t latency = kDefaultLatency, const bool stable = false) :
                                 mName(std::move(name)),
                                 mDepth(latency + 1),
                                 mLatency(latency),
                                 mStablePerf(stable){}
        ~StableTimedFifo() = default;
        void setDepth(const uint32_t depth) {
            mDepth = std::min(depth, kMaxFifoDepth);
        }
        void setLatency(const uint32_t latency) {
            mLatency = std::max(latency, 1u);
        }
        void setStability(const bool stable) {
            mStablePerf = stable;
        }


    private:
        struct TimedElement {
            uint32_t _readyTime = 0;
            T        _data{};
        };
        std::string                 mName{};
        std::vector<TimedElement>   mData{};
        uint32_t                    mDepth          = kMaxFifoDepth;
        uint32_t                    mLatency        = kDefaultLatency;
        bool                        mStablePerf     = false;

    };
}


#endif //STABLETIMEDFIFO_H
