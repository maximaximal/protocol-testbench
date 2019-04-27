#include "protocol.hpp"

namespace protocol {
class EventBased : public Protocol {
public:
  explicit EventBased(std::random_device &rd);
  virtual ~EventBased();

  virtual int startTransfer(Message &tmpMsg1, Message &tmpMsg2);
};
} // namespace protocol
