#pragma once

// std libraries

// glm libraries
#include <glm/vec2.hpp>   // *vec2

// in-house functionality
#include <messages/Message.hpp>
#include <messages/MessageQueue.hpp>      // *Message
#include <update/OrbitalControlState.hpp> // OrbitalControlState

namespace update
{

  class OrbitalControlUpdater
  {

    float pan_speed;
    float zoom_speed;

public:

    OrbitalControlUpdater():
      pan_speed(0.01f),
      zoom_speed(0.1f)
    {}

    OrbitalControlUpdater(
      const float pan_speed,
      const float zoom_speed
    ):
      pan_speed(pan_speed),
      zoom_speed(zoom_speed)
    {}

    void update(
      const OrbitalControlState& state_in,
      const messages::MouseMotionMessage message,
      OrbitalControlState& state_out
    ) { 
      // std::cout << message.offset.x << " " << message.offset.x << std::endl;
      if(&state_in != &state_out) {
        state_out = state_in;
      }
      switch(state_out.drag_state)
      {
        case OrbitalControlDragState::pan: 
          OrbitalControlState::pan(state_in, pan_speed*message.offset, state_out);
          break;
        case OrbitalControlDragState::zoom: 
          OrbitalControlState::zoom(state_in, zoom_speed*message.offset.y, state_out);
          break;
        default: break;
      }
    }

    void update(
      const OrbitalControlState& state_in,
      const messages::MouseClickMessage message,
      OrbitalControlState& state_out
    ) { 
      if(&state_in != &state_out) {
        state_out = state_in;
      }
      switch(message.button)
      {
        case messages::MouseButton::left:
          state_out.drag_state = OrbitalControlDragState::pan;
          break;
        case messages::MouseButton::middle:
          state_out.drag_state = OrbitalControlDragState::zoom;
          break;
        case messages::MouseButton::right:
          state_out.drag_state = OrbitalControlDragState::rotate;
          break;
        default: break;
      }
      if (!message.is_pressed)
      {
        state_out.drag_state = OrbitalControlDragState::released;
      }
    }

    void update(
      const OrbitalControlState& state_in,
      const messages::ScrollMessage message,
      OrbitalControlState& state_out
    ) { 
      OrbitalControlState::zoom(state_in, message.offset.y, state_out);
    }

    void update(
      const OrbitalControlState& state_in,
      const messages::Message message,
      OrbitalControlState& state_out
    ) {
      if (std::holds_alternative<messages::MouseMotionMessage>(message))
      {
        update(state_in, std::get<messages::MouseMotionMessage>(message), state_out);
      }
      else if (std::holds_alternative<messages::MouseClickMessage>(message))
      {
        update(state_in, std::get<messages::MouseClickMessage>(message), state_out);
      }
      else if (std::holds_alternative<messages::MouseClickMessage>(message))
      {
        update(state_in, std::get<messages::ScrollMessage>(message), state_out);
      }
      // NOTE: I gave up on this, can't get it to work with additional non-variant parameters
      // std::visit(OrbitalControlUpdater(), message, state_in, state_out);
    }
  };
}