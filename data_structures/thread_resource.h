#pragma once

#include <concepts>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <thread>
#include <type_traits>
#include <utility>

template <typename T>
concept Mutex = requires(T mtx) {
                  mtx.lock();
                  mtx.try_lock();
                  mtx.unlock();
                };

template <typename T>
concept SharedMutex = requires(T mtx) {
                        requires Mutex<T>;
                        mtx.lock_shared();
                        mtx.try_lock_shared();
                        mtx.unlock_shared();
                      };

template <typename T>
concept LockType = true;  // TODO(user):

template <typename T>
concept SharedLockType = true;  // TODO (user)

template <
    class T,
    Mutex MutexType,
    LockType LockT = std::scoped_lock<MutexType>,
    SharedLockType SharedLockT = std::shared_lock<MutexType>>
class ThreadResource {
 public:
  enum class AccessType {
    Exclusive,
    Shared,
  };

  template <AccessType AccessT, LockType LockY>
  class Access {
    using ResourceType = std::conditional_t<AccessT == AccessType::Exclusive, T*, const T*>;

   public:
    Access(MutexType& m, T& obj) : lock_(m), resource_(&obj) {}

    Access(Access&&) noexcept
      requires(std::movable<LockY>)
    = default;

    auto operator->() -> ResourceType { return resource_; }

    auto operator->() const -> ResourceType
      requires(AccessT == AccessType::Shared)
    {
      return resource_;
    }

   private:
    LockY lock_;
    ResourceType resource_;
  };

  [[nodiscard]] auto access() { return Access<AccessType::Exclusive, LockT>(mutex_, res_); }

  [[nodiscard]] auto sharedAccess()
    requires(SharedMutex<MutexType>)
  {
    return Access<AccessType::Shared, SharedLockT>(mutex_, res_);
  }

  [[nodiscard]] auto mutexHandle() -> MutexType& { return mutex_; }

  ThreadResource()
    requires(std::default_initializable<T>)
  = default;

  [[nodiscard]] auto copyResource() const -> T
    requires(std::copyable<T> && SharedMutex<MutexType>)
  {
    if constexpr (SharedMutex<MutexType>) {
      auto lock = std::shared_lock(mutex_);
      return res_;
    }

    auto lock = std::scoped_lock(mutex_);
    return res_;
  }

  explicit ThreadResource(auto&&... args) : res_(std::forward<decltype(args)>(args)...) {}

 private:
  mutable MutexType mutex_;
  T res_;
};

struct Foo {
  explicit Foo(int f) : foo_(f) {}
  int foo_{};
};

template <typename T>
struct SharedResource : public ThreadResource<T, std::shared_mutex, std::shared_lock<std::shared_mutex>> {
  using Base = ThreadResource<T, std::shared_mutex, std::shared_lock<std::shared_mutex>>;

  using Base::Base;
};

template <typename T>
struct ExcelusiveResource : public ThreadResource<T, std::mutex, std::scoped_lock<std::mutex>> {
  using Base = ThreadResource<T, std::mutex, std::scoped_lock<std::mutex>>;

  using Base::Base;
};
