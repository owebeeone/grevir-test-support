#pragma once

#include <mutex>

namespace grevir::test {

// The host mock serializes every queue producer and its single consumer.
class EventLock {
 public:
  EventLock() noexcept { mutex_.lock(); }
  ~EventLock() noexcept { mutex_.unlock(); }
  EventLock(const EventLock&) = delete;
  EventLock& operator=(const EventLock&) = delete;

 private:
  inline static std::mutex mutex_{};
};

} // namespace grevir::test
