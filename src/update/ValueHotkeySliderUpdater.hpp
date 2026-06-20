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
  class ValueHotkeySliderUpdater
  {

    std::string increment;
    std::string decrement;
    T step;

public:

    template<typename String1, typename String2>
    ValueHotkeySliderUpdater(
      const String1 increment,
      const String2 decrement,
      const T step
    ):
      increment(increment),
      decrement(decrement),
      step(step)
    {}

    T update(
      const T value,
      const messages::KeyboardMessage message
    ) const { 
      return 
        (message.action != messages::release && message.character == increment)? value + step
      : (message.action != messages::release && message.character == decrement)? value - step
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
