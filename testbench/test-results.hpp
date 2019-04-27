#pragma once

#include "protocols/protocol.hpp"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <string_view>

/**
 * Stores test results of a single run with a chosen error rate and protocol.
 *
 * Tests are always done with a specified error rate and
 * protocol to use. Protocols then write statistics about the runs in here.
 */
class TestResults {
public:
  explicit TestResults(std::string protocolName);
  virtual ~TestResults();

  /** Adds result to internal accumulator.
   *
   * The probability must be 1000 * p, so 1 % (or p = 0.01) becomes 10.
   */
  void addResult(int errorPropability, int trips, bool successfulTransmission,
                 bool correctDiagnosis);

  void writeResults(std::ofstream &out);

  static std::unique_ptr<TestResults> startAutomatedTest(protocol::Protocol &p);

  std::string_view name();

private:
  using IntAccumulatorSet = boost::accumulators::accumulator_set<
      int, boost::accumulators::features<
               boost::accumulators::tag::min, boost::accumulators::tag::max,
               boost::accumulators::tag::mean, boost::accumulators::tag::median,
               boost::accumulators::tag::variance>>;
  using FloatAccumulatorSet = boost::accumulators::accumulator_set<
      float, boost::accumulators::features<boost::accumulators::tag::mean,
                                           boost::accumulators::tag::median>>;

  struct Result {
    IntAccumulatorSet trips;
    FloatAccumulatorSet successfulTransmissions;
    FloatAccumulatorSet correctDiagnosis;
  };
  std::map<int, Result> m_results;
  std::string m_protocolName;
};
