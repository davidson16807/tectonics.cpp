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
  class ValueHotkeyPresetUpdater
  {

    std::string key;
    T preset;

public:

    template<typename String1>
    ValueHotkeyPresetUpdater(
      const String1 key,
      const T preset
    ):
      key(key),
      preset(preset)
    {}

    T update(
      const T value,
      const messages::KeyboardMessage message
    ) const { 
      return (message.action != messages::release && message.character == key)? preset : value;
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
