#include "testbench.hpp"
#include "protocols/continuous.hpp"
#include "protocols/event-based.hpp"
#include "test-results.hpp"
#include <fstream>
#include <future>
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

Testbench::Testbench() {
  std::random_device rd;
  // m_protocols.push_front();

  auto continuousNoCont = std::make_unique<protocol::Continuous>(rd);
  continuousNoCont->activateSubsequentCorruptions(false);
  auto continuousCont = std::make_unique<protocol::Continuous>(rd);
  continuousCont->activateSubsequentCorruptions(true);

  auto eventBasedNoCont = std::make_unique<protocol::EventBased>(rd);
  eventBasedNoCont->activateSubsequentCorruptions(false);
  auto eventBasedCont = std::make_unique<protocol::EventBased>(rd);
  eventBasedCont->activateSubsequentCorruptions(true);

  m_protocols.push_front(std::move(continuousNoCont));
  m_protocols.push_front(std::move(continuousCont));
  m_protocols.push_front(std::move(eventBasedNoCont));
  m_protocols.push_front(std::move(eventBasedCont));
}
Testbench::~Testbench() {}
void Testbench::begin() {
  // Test all available protocols.

  // This can be done in parallel.
  std::list<std::future<std::unique_ptr<TestResults>>> resultFutures;

  std::transform(
      m_protocols.begin(), m_protocols.end(),
      std::inserter(resultFutures, resultFutures.end()), [](auto &p) -> auto {
        spdlog::info("Preparing protocol \"{}\" for testing.", p->name());
        return std::async([&p]() -> auto {
          spdlog::info("Launched protocol \"{}\" for testing.", p->name());
          return TestResults::startAutomatedTest(*p);
        });
      });

  std::list<std::unique_ptr<TestResults>> results;
  std::transform(
      resultFutures.begin(), resultFutures.end(),
      std::inserter(results, results.end()),
      [](auto &future) -> auto { return future.get(); });

  spdlog::info("Waiting for {} tests to finish.", results.size());

  for (auto &result : results) {
    std::ofstream outfile;
    outfile.open(std::string(result->name()) + std::string(".csv"),
                 std::ios::binary | std::ios::out | std::ios::trunc);
    result->writeResults(outfile);
    outfile.close();
  }
}
