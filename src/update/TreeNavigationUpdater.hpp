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
  class TreeNavigationUpdater
  {

    using ids = std::vector<id>;

    std::string parent_key;
    std::string child_key;
    std::string last_sibling_key;
    std::string next_sibling_key;

public:

    template<typename String1, typename String2, typename String3, typename String4>
    TreeNavigationUpdater(
      const String1 parent_key,
      const String2 child_key,
      const String3 last_sibling_key,
      const String4 next_sibling_key
    ):
      parent_key(parent_key),
      child_key(child_key),
      last_sibling_key(last_sibling_key),
      next_sibling_key(next_sibling_key)
    {}

    id update(
      const id focus_id,
      const ids& parent_ids,
      const messages::KeyboardMessage message
    ) const { 
      if (message.action != messages::release)
      {
        auto c = message.character;
        if      (c == parent_key)       { return std::clamp(focus_id+1, id(0), id(parent_ids.size()-1)); }
        else if (c == child_key)        { return std::clamp(focus_id-1, id(0), id(parent_ids.size()-1)); }
        else if (c == last_sibling_key) { return parent_ids[focus_id]; } 
        else if (c == next_sibling_key) { for(std::size_t i=0; i<parent_ids.size(); i++) { if(parent_ids[i]==focus_id){ return id(i); } } }
      }
      return focus_id;
    }

    id update(
      const id focus_id,
      const ids& parent_ids,
      const messages::Message message
    ) const {
      if (std::holds_alternative<messages::KeyboardMessage>(message))
      {
        return update(focus_id, parent_ids, std::get<messages::KeyboardMessage>(message));
      }
      return focus_id;
    }

  };

}
