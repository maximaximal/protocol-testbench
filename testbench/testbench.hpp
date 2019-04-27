#pragma once

#include "protocols/protocol.hpp"
#include <list>
#include <memory>

class Testbench {
public:
  Testbench();
  virtual ~Testbench();

  void begin();
private:
  using ProtocolPtr = std::unique_ptr<protocol::Protocol>;
  std::list<ProtocolPtr> m_protocols;
};
