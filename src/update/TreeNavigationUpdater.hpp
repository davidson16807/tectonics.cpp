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

    const std::string parent_key;
    const std::string child_key;
    const bool typematic_generations;
    const std::string last_sibling_key;
    const std::string next_sibling_key;
    const bool typematic_siblings;

public:

    template<typename String1, typename String2, typename String3, typename String4>
    TreeNavigationUpdater(
      const String1 parent_key,
      const String2 child_key,
      const bool typematic_generations,
      const String3 last_sibling_key,
      const String4 next_sibling_key,
      const bool typematic_siblings
    ):
      parent_key(parent_key),
      child_key(child_key),
      typematic_generations(typematic_generations),
      last_sibling_key(last_sibling_key),
      next_sibling_key(next_sibling_key),
      typematic_siblings(typematic_siblings)
    {}

    id update(
      const id focus_id,
      const ids& parent_ids,
      const messages::KeyboardMessage message
    ) const { 
      if ((message.action != messages::release && typematic_generations) || message.action == messages::press)
      {
        auto c = message.character;
        if      (c == parent_key)       { return parent_ids[focus_id]; } 
        else if (c == child_key)        { for(std::size_t i=0; i<parent_ids.size(); i++) { if(parent_ids[i]==focus_id){ return id(i); } } }
      }
      if ((message.action != messages::release && typematic_siblings) || message.action == messages::press)
      {
        auto c = message.character;
             if (c == last_sibling_key) { return (focus_id-1 >= 0 && parent_ids[focus_id-1] == parent_ids[focus_id])? focus_id-1 : focus_id; }
        else if (c == next_sibling_key) { return (focus_id+1 < id(parent_ids.size()) && parent_ids[focus_id+1] == parent_ids[focus_id])? focus_id+1 : focus_id;}
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
