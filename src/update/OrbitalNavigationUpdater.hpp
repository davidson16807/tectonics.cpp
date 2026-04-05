#pragma once

// std libraries

// glm libraries
#include <glm/vec2.hpp>   // *vec2

// in-house functionality
#include <messages/Message.hpp>
#include <messages/MessageQueue.hpp>      // *Message
#include <update/OrbitalNavigationState.hpp> // OrbitalNavigationState

namespace update
{

  class OrbitalNavigationUpdater
  {

    float pan_speed;
    float zoom_speed;

public:

    OrbitalNavigationUpdater():
      pan_speed(0.01f),
      zoom_speed(0.1f)
    {}

    OrbitalNavigationUpdater(
      const float pan_speed,
      const float zoom_speed
    ):
      pan_speed(pan_speed),
      zoom_speed(zoom_speed)
    {}

    void update(
      const OrbitalNavigationState& state_in,
      const messages::MouseMotionMessage message,
      OrbitalNavigationState& state_out
    ) { 
      // std::cout << message.offset.x << " " << message.offset.x << std::endl;
      if(&state_in != &state_out) {
        state_out = state_in;
      }
      switch(state_out.drag_state)
      {
        case OrbitalNavigationDragState::pan: 
          OrbitalNavigationState::pan(state_in, pan_speed*message.offset, state_out);
          break;
        case OrbitalNavigationDragState::zoom: 
          OrbitalNavigationState::zoom(state_in, zoom_speed*message.offset.y, state_out);
          break;
        default: break;
      }
    }

    void update(
      const OrbitalNavigationState& state_in,
      const messages::MouseClickMessage message,
      OrbitalNavigationState& state_out
    ) { 
      if(&state_in != &state_out) {
        state_out = state_in;
      }
      switch(message.button)
      {
        case messages::MouseButton::left:
          state_out.drag_state = OrbitalNavigationDragState::pan;
          break;
        case messages::MouseButton::middle:
          state_out.drag_state = OrbitalNavigationDragState::zoom;
          break;
        case messages::MouseButton::right:
          state_out.drag_state = OrbitalNavigationDragState::rotate;
          break;
        default: break;
      }
      if (!message.is_pressed)
      {
        state_out.drag_state = OrbitalNavigationDragState::released;
      }
    }

    void update(
      const OrbitalNavigationState& state_in,
      const messages::ScrollMessage message,
      OrbitalNavigationState& state_out
    ) { 
      OrbitalNavigationState::zoom(state_in, message.offset.y, state_out);
    }

    void update(
      const OrbitalNavigationState& state_in,
      const messages::Message message,
      OrbitalNavigationState& state_out
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
      // std::visit(OrbitalNavigationUpdater(), message, state_in, state_out);
    }
  };
}