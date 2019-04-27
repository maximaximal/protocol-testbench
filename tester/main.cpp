#include <boost/program_options.hpp>
#include <iostream>

#include <testbench-config.hpp>
#include <testbench.hpp>

using std::cout;
using std::endl;

#include <spdlog/fmt/fmt.h>

int main(int argc, char *argv[]) {
  using namespace boost::program_options;
  options_description desc{"Options"};
  desc.add_options()("help,h", "Help sceeen")("version,v", "Version");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count(("help"))) {
    cout << desc << endl;
    return 0;
  }
  if (vm.count(("version"))) {
    fmt::print("Version: {}.{}.{}\n", TESTBENCH_VERSION_MAJOR,
               TESTBENCH_VERSION_MINOR, TESTBENCH_VERSION_PATCH);
    return 0;
  }

  // Default operation is to run all available tests.
  Testbench testbench;
  testbench.begin();
}
