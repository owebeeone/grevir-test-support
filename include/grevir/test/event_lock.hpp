#pragma once

#include <mutex>
#include <grevir/base/compat/string_view.hpp>

namespace grevir::test {

// The host mock serializes every queue producer and its single consumer.
class EventLock {
 public:
  using TaskGuard = EventLock;
  using IsrGuard = EventLock;
  inline static constexpr std::string_view identity{"host_mutex_v1", 13};
  EventLock() noexcept { mutex_.lock(); }
  ~EventLock() noexcept { mutex_.unlock(); }
  EventLock(const EventLock&) = delete;
  EventLock& operator=(const EventLock&) = delete;

 private:
  inline static std::mutex mutex_{};
};

} // namespace grevir::test
