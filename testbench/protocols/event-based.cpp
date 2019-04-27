#include "event-based.hpp"
#include <spdlog/spdlog.h>

namespace protocol {
EventBased::EventBased(std::random_device &rd) : Protocol(rd, "Event-Based") {}

EventBased::~EventBased() {}

int EventBased::startTransfer(Message &tmpMsg1, Message &tmpMsg2) {
  nextMessage(tmpMsg1);
  Message &msg = tmpMsg1;
  int16_t crc16 = getCrc16OfMessage(msg);

  uint8_t crcBytes[2] = {(uint8_t)((crc16 & 0xFF00) >> 8),
                         (uint8_t)(crc16 & 0x00FF)};

  // Event based architectures generally need bigger messages to account for
  // packet numbers, flow control, ... In this work, this is accounted by
  // generously adding 4 int16 values to each package. This means, 8 bytes are
  // transmitted.
  uint8_t additionalInformation[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  msg.insert(msg.end(), std::begin(additionalInformation),
             std::end(additionalInformation));

  msg.insert(msg.end(), std::begin(crcBytes), std::end(crcBytes));

  const std::size_t additionalBytesTransmitted = 2 + 8;

  m_successfulTransmission = false;
  m_correctDiagnosis = false;
  std::size_t retransmissions = 0;
  std::size_t maxRetransmissions = 5;

  while (!m_successfulTransmission && retransmissions < maxRetransmissions) {
    Message &receivedMessage = tmpMsg2;
    receivedMessage = msg;

    simulateHardware(receivedMessage);

    int completeMsgLength = receivedMessage.size();

    int16_t receivedCrc16 =
        (((int16_t)receivedMessage[receivedMessage.size() - 2]) << 8) |
        ((int16_t)receivedMessage[receivedMessage.size() - 1]);

    receivedMessage.erase(receivedMessage.begin() + receivedMessage.size() -
                              additionalBytesTransmitted,
                          receivedMessage.end());

    int16_t calculatedReceivedCrc16 = getCrc16OfMessage(receivedMessage);

    if (calculatedReceivedCrc16 == receivedCrc16) {
      // It can be assumed the transfer was successful!
      m_successfulTransmission = checkMessage(Ok, receivedMessage);
      m_correctDiagnosis = m_successfulTransmission;
    } else {
      // The event based algorithm can only know a packet is missing, as soon as
      // the time for a whole transmission has passed. So another trip is
      // done to account for this delay.
      m_currentTripTime += completeMsgLength;
      m_correctDiagnosis = true;
    }

    ++retransmissions;
  }
  return m_currentTripTime;
}

} // namespace protocol
