#include "protocol.hpp"

namespace protocol
{
class Continuous : public Protocol {
public:
  explicit Continuous(std::random_device &rd);
  virtual ~Continuous();

  virtual int startTransfer(Message &tmpMsg1, Message &tmpMsg2);
};
}
