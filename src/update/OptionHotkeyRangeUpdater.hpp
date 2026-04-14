#pragma once

// std libraries
#include <vector>

// glm libraries

// in-house functionality
#include <messages/Message.hpp>
#include <messages/MessageQueue.hpp>      // *Message

namespace update
{

  template<typename id>
  class OptionHotkeyRangeUpdater
  {

    const std::string last_key;
    const std::string next_key;
    const bool typematic;

public:

    template<typename String1, typename String2>
    OptionHotkeyRangeUpdater(
      const String2 last_key,
      const String1 next_key,
      const bool typematic
    ):
      last_key(last_key),
      next_key(next_key),
      typematic(typematic)
    {}

    id update(
      const id option_id,
      const auto& options,
      const messages::KeyboardMessage message
    ) const { 
      if ((message.action != messages::release && typematic)
          || message.action == messages::press)
      {
        auto c = message.character;
        if      (c == last_key)  { return std::clamp(int(option_id-1), 0, int(options.size())-1); }
        else if (c == next_key)  { return std::clamp(int(option_id+1), 0, int(options.size())-1); }
      }
      return option_id;
    }

    id update(
      const id option_id,
      const auto& options,
      const messages::Message message
    ) const {
      if (std::holds_alternative<messages::KeyboardMessage>(message))
      {
        return update(option_id, options, std::get<messages::KeyboardMessage>(message));
      }
      return option_id;
    }

  };

}
