#include "thread_resource.h"

auto main() -> int {
  ExcelusiveResource<Foo> shared(0);

  std::binary_semaphore semaphore{0};

  auto t1 = std::jthread([&shared, &semaphore]() mutable {
    for (int i = 0; i < 1e6; i++) {
      auto access = shared.access();
      access->foo_++;
    }

	semaphore.release();
  });

  auto t2 = std::jthread([&shared, &semaphore]() mutable {
    for (int i = 0; i < 1e6; i++) {
      auto access = shared.access();
      access->foo_++;
    }

	semaphore.acquire();
  });

  auto t3 = std::jthread([&shared]() mutable {
    auto& foo = shared.access()->foo_;

    for (int i = 0; i < 1e6; i++) {
      ++foo;
    }
  });

  t1.join();
  t2.join();

  std::cout << shared.access()->foo_ << '\n';
}
