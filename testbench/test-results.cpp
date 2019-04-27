#include "test-results.hpp"
#include <boost/range/irange.hpp>
#include <cmath>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#define VALUES_RANGE irange(0, 10000, 1)

float errorPForErrorInt(int e) { return (e / 1000000.0) * 20; }

TestResults::TestResults(std::string protocolName)
    : m_protocolName(protocolName) {}
TestResults::~TestResults() {}
void TestResults::addResult(int errorPropability, int trips,
                            bool successfulTransmission,
                            bool correctDiagnosis) {
  m_results[errorPropability].trips(trips);
  m_results[errorPropability].successfulTransmissions(
      successfulTransmission ? 1 : 0);
  m_results[errorPropability].correctDiagnosis(correctDiagnosis ? 1 : 0);
}

void TestResults::writeResults(std::ofstream &out) {
  using namespace fmt;
  using namespace boost;
  using namespace boost::accumulators;

  out << fmt::format(
      "p\ttrips_min\ttrips_max\ttrips_mean\ttrips_med\ttrips_sd\tsucc_mean\t"
      "succ_med\tdiag_mean\tdiag_med\n");
  for (auto errorProp : VALUES_RANGE) {
    auto &res = m_results[errorProp];
    out << fmt::format(
        "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\n",
        errorPForErrorInt(errorProp), min(res.trips), max(res.trips),
        mean(res.trips), median(res.trips), sqrt(variance(res.trips)),
        mean(res.successfulTransmissions), median(res.successfulTransmissions),
        mean(res.correctDiagnosis), median(res.correctDiagnosis));
  }
}

std::string_view TestResults::name() { return m_protocolName; }

std::unique_ptr<TestResults>
TestResults::startAutomatedTest(protocol::Protocol &protocol) {
  std::unique_ptr<TestResults> results =
      std::make_unique<TestResults>(protocol.name());

  using namespace boost;

  // Simulate 1 minute at 120 messages per second.
  const int messageCount = 7200;
  const std::size_t messageSize =
      4; // Message size (in bytes) should be 4. This represents a single
         // integer of data to be transmitted.

  protocol.setMessageSize(messageSize);

  protocol::Protocol::Message msg[2];

  for (auto errorProp : VALUES_RANGE) {
    float p = errorPForErrorInt(errorProp);
    protocol.setErrorPropability(p);

    if (errorProp % 100 == 0) {
      spdlog::info("Currently at p={} for protocol \"{}\"", p, results->name());
    }

    for (auto _msgNumber : irange(0, messageCount)) {
      int trips = protocol.startTransfer(msg[0], msg[1]);
      results->addResult(errorProp, trips,
                         protocol.lastTransmissionSuccessful(),
                         protocol.lastDiagnosisCorrect());
    }
  }

  return results;
}
