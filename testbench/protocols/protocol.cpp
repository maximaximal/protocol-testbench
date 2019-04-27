#include "protocol.hpp"
#include <algorithm>
#include <boost/crc.hpp>
#include <spdlog/spdlog.h>

#define SUBSEQUENT_CORRUPTION_PROP 0.4

namespace protocol {
Protocol::Protocol(std::random_device &rd, std::string name)
    : m_rng(rd()), m_subsequentCorruption(SUBSEQUENT_CORRUPTION_PROP),
      m_name(name) {
  setMessageSize(12);
}
Protocol::~Protocol() {}
void Protocol::setMessageSize(std::size_t size) {
  m_currentMessage.resize(size);
  m_messageSize = size;
}
void Protocol::setErrorPropability(float p) {
  m_corruptBit = std::bernoulli_distribution(p);
}
void Protocol::activateSubsequentCorruptions(bool activate) {
  m_subsequentCorruptionsActivated = activate;
}
std::string protocol::Protocol::name() {
  return m_name + (m_subsequentCorruptionsActivated
                       ? "_with_subsequent_corruptions"
                       : "");
}
void Protocol::nextMessage(Message &msg) {
  m_currentMessage.resize(m_messageSize);
  msg.resize(m_messageSize);
  std::generate(m_currentMessage.begin(), m_currentMessage.end(), m_rng);

  // Begin new message transmission.
  msg = m_currentMessage;
  m_currentTripTime = 0;
}
void Protocol::simulateHardware(protocol::Protocol::Message &message) {
  m_currentTripTime += message.size();

  for (uint8_t &b : message) {
    for (uint8_t bit = 0; bit < 8; ++bit) {
      if (m_corruptBit(m_rng)) {
        b ^= 1u << bit;
        m_lastBitCorrupted = true;
      } else if (m_lastBitCorrupted && m_subsequentCorruptionsActivated &&
                 m_subsequentCorruption(m_rng)) {
        b ^= 1u << bit;
        m_lastBitCorrupted = true;
      } else {
        m_lastBitCorrupted = false;
      }
    }
  }
}
int16_t Protocol::getCrc16OfMessage(const Message &msg) {
  boost::crc_16_type result;
  result.process_bytes(msg.data(), msg.size());
  return result.checksum();
}
bool protocol::Protocol::checkMessage(protocol::Protocol::ProtocolStatus status,
                                      protocol::Protocol::Message &msg) {
  switch (status) {
  case ProtocolStatus::Ok:
    return std::equal(m_currentMessage.begin(), m_currentMessage.end(),
                      msg.begin(), msg.end());
  case ProtocolStatus::ErrorDetected:
    return !std::equal(m_currentMessage.begin(), m_currentMessage.end(),
                       msg.begin(), msg.end());
  }
  return false;
}

int Protocol::startTransfer(Message &tmpMsg1, Message &tmpMsg2) {
  // The ideal protocol would have a transfer time of 0.
  return 0;
}

} // namespace protocol
