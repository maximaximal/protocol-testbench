#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace protocol {
class Protocol {
public:
  enum ProtocolStatus {
    Ok,
    ErrorDetected,
  };
  using Message = std::vector<uint8_t>;

  explicit Protocol(std::random_device &rd, std::string name);
  virtual ~Protocol();

  /// Called from outside, e.g. from the testbench.
  void setMessageSize(std::size_t size);
  void setErrorPropability(float p);
  void activateSubsequentCorruptions(bool activate);

  bool lastTransmissionSuccessful() { return m_successfulTransmission; }
  bool lastDiagnosisCorrect() { return m_correctDiagnosis; }

  int16_t getCrc16OfMessage(const Message &msg);

  std::string name();

  /// Generate a random message into the internal message structure and COPY it
  /// when returning.
  void nextMessage(Message &tmpMessage);
  /** Simulates the hardware this communication protocol is running on.
   *
   * This message can be a different one than the one saved internally, for
   * example the protocol could implement some sort of CRC checking mechanism.
   */
  void simulateHardware(Message &msg);
  bool checkMessage(ProtocolStatus status, Message &msg);

  virtual int startTransfer(Message &tmpMsg1, Message &tmpMsg2);

protected:
  int m_currentTripTime = 0;
  bool m_successfulTransmission = false;
  bool m_correctDiagnosis = false;

private:
  bool m_subsequentCorruptionsActivated = false;
  std::string m_name;

  Message m_currentMessage;
  bool m_lastBitCorrupted = false;
  std::size_t m_messageSize = 4;

  std::mt19937 m_rng;

  // Chance for errors, p_e
  std::bernoulli_distribution m_corruptBit;
  // Chance for errors after error already occured, p_b
  std::bernoulli_distribution m_subsequentCorruption;
};
} // namespace protocol
