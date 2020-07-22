#pragma once

// std libraries
#include <variant>   // *vec2

// 3rd party libraries

// in-house functionality

namespace messages 
{

	/*
	A "MouseMotionMessage" represents a change in the position of a mouse and its new position
	It stores all information that was reported by glfw, 
	in addition to the change in position since the last poll event.
	*/
	struct MouseMotionMessage 
	{
		glm::vec2 position;
		glm::vec2 offset;
		~MouseMotionMessage() {} 
		MouseMotionMessage(glm::vec2 position, glm::vec2 offset) :
			position(position),
			offset(offset)
		{} 
	};

	enum MouseButton
	{
		left,
		middle,
		right
	};

	enum KeyboardModifier
	{
		shift = 1 << 0,
		ctrl  = 1 << 1,
		alt   = 1 << 2,
		super = 1 << 3,
		caps  = 10,
		num   = 20
	};

	/*
	A "MouseClickMessage" represents a mouse click as reported by glfw.
	*/
	struct MouseClickMessage
	{
		int button; 
		int is_pressed; 
		int modifiers;
		~MouseClickMessage() {} 
		MouseClickMessage(int button, bool is_pressed, int modifiers) :
			button(button),
			is_pressed(is_pressed),
			modifiers(modifiers)
		{} 
	};

	/*
	A "ScrollMessage" represents any scroll, regardless of whether its from touchpad or mouse.
	*/
	struct ScrollMessage 
	{
		glm::vec2 offset;
		~ScrollMessage() {} 
		ScrollMessage(glm::vec2 offset) :
			offset(offset)
		{} 
	};

	typedef std::variant<MouseMotionMessage, MouseClickMessage, ScrollMessage> Message;

}