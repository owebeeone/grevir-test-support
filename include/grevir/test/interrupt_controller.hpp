#pragma once

namespace grevir::test {

class InterruptController {
 public:
  using Callback = void (*)() noexcept;

  bool install(Callback callback) noexcept {
    if (callback_ != nullptr || reject_install_) { return false; }
    callback_ = callback;
    return true;
  }

  void mask() noexcept { enabled_ = false; }
  void enable() noexcept {
    enabled_ = true;
    dispatch();
  }
  void raise() noexcept {
    pending_ = true;
    dispatch();
  }
  void reject_install(bool value) noexcept { reject_install_ = value; }
  bool pending() const noexcept { return pending_; }
  bool installed() const noexcept { return callback_ != nullptr; }
  bool enabled() const noexcept { return enabled_; }
  void reset() noexcept {
    callback_ = nullptr;
    enabled_ = false;
    pending_ = false;
    reject_install_ = false;
    dispatching_ = false;
  }

 private:
  void dispatch() noexcept {
    if (dispatching_) { return; }
    while (enabled_ && pending_ && callback_ != nullptr) {
      pending_ = false;
      dispatching_ = true;
      callback_();
      dispatching_ = false;
    }
  }

  Callback callback_ = nullptr;
  bool enabled_ = false;
  bool pending_ = false;
  bool reject_install_ = false;
  bool dispatching_ = false;
};

} // namespace grevir::test
