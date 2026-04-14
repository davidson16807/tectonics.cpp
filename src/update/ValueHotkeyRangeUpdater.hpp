#pragma once

// std libraries
#include <vector>

// glm libraries

// in-house functionality
#include <messages/Message.hpp>
#include <messages/MessageQueue.hpp>      // *Message

namespace update
{

  template<typename T>
  class ValueHotkeyRangeUpdater
  {

    std::string increment;
    std::string decrement;
    T step;
    T lo;
    T hi;

public:

    template<typename String1, typename String2>
    ValueHotkeyRangeUpdater(
      const String1 increment,
      const String2 decrement,
      const T step,
      const T lo,
      const T hi
    ):
      increment(increment),
      decrement(decrement),
      step(step),
      lo(lo),
      hi(hi)
    {}

    T update(
      const T value,
      const messages::KeyboardMessage message
    ) const { 
      return 
        (message.action != messages::release && message.character == increment)? std::clamp(value + step, lo, hi)
      : (message.action != messages::release && message.character == decrement)? std::clamp(value - step, lo, hi)
      : value;
    }

    T update(
      const T value,
      const messages::Message message
    ) const {
      if (std::holds_alternative<messages::KeyboardMessage>(message))
      {
        return update(value, std::get<messages::KeyboardMessage>(message));
      }
      return value;
    }

  };

}
