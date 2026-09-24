#pragma once

#include <condition_variable>
#include <mutex>
#include <grevir/interrupt/start.hpp>

namespace grevir::test {

template <class Spec>
class HostStartPolicy {
 public:
  template <class Body>
  static interrupt::StartResult execute(Body body) noexcept {
    std::unique_lock lock(mutex_);
    if (state_ == State::done) {
      auto result = result_;
      result.disposition = interrupt::CallDisposition::replayed;
      return result;
    }
    if (state_ == State::starting) {
      ++waiters_;
      ready_.wait(lock, [] { return state_ == State::done; });
      --waiters_;
      auto result = result_;
      result.disposition = interrupt::CallDisposition::waited;
      return result;
    }
    state_ = State::starting;
    lock.unlock();
    auto result = body();
    lock.lock();
    result_ = result;
    state_ = State::done;
    lock.unlock();
    ready_.notify_all();
    return result;
  }

  static void reset_for_test() {
    std::lock_guard lock(mutex_);
    state_ = State::idle;
    result_ = {};
    waiters_ = 0;
  }

  static unsigned waiting_for_test() {
    std::lock_guard lock(mutex_);
    return waiters_;
  }

 private:
  enum class State { idle, starting, done };
  inline static std::mutex mutex_{};
  inline static std::condition_variable ready_{};
  inline static State state_ = State::idle;
  inline static interrupt::StartResult result_{};
  inline static unsigned waiters_ = 0;
};

} // namespace grevir::test
