#include "continuous.hpp"

namespace protocol {
  Continuous::Continuous(std::random_device &rd) : Protocol(rd, "Continuous") {}
Continuous::~Continuous() {}

  int Continuous::startTransfer(Message &tmpMsg1, Message &tmpMsg2) {
  nextMessage(tmpMsg1);
  Message &msg = tmpMsg1;

  int16_t crc16 = getCrc16OfMessage(msg);

  uint8_t crcBytes[2] = {(uint8_t)((crc16 & 0xFF00) >> 8),
                         (uint8_t)(crc16 & 0xFF)};

  msg.insert(msg.end(), std::begin(crcBytes), std::end(crcBytes));

  const std::size_t additionalBytesTransmitted = 2;

  {
    Message &receivedMessage = tmpMsg2;
    receivedMessage = msg;

    simulateHardware(receivedMessage);

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
      // This transfer can be considered to have been unsuccessful! The next
      // transmission in the stream has to be awaited.
      m_successfulTransmission = false;
      m_correctDiagnosis = checkMessage(ErrorDetected, receivedMessage);
    }
  }

  return m_currentTripTime;
}
} // namespace protocol
