#pragma once

// std libraries
#include <variant> // std::variant
#include <string>  // std::string

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
		MouseMotionMessage(
			const glm::vec2 position, 
			const glm::vec2 offset
		) :
			position(position),
			offset(offset)
		{} 
	};

	enum MouseButton
	{
		left,
		right,
		middle
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

	enum KeyboardAction
	{
		release = 0,
		press   = 1,
		repeat  = 2,
		unknown = -1,
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
		MouseClickMessage(
			const int button, 
			const bool is_pressed, 
			const int modifiers
		) :
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
		ScrollMessage(
			const glm::vec2 offset
		) :
			offset(offset)
		{} 
	};

	/*
	A "KeyDownMessage" represents when a key on a keyboard is pressed down
	*/
	struct KeyDownMessage 
	{
		std::string character;
		KeyboardModifier modifiers;
		~KeyDownMessage() {} 
		KeyDownMessage(
			const std::string character, 
			const KeyboardModifier modifiers
		) :
			character(character),
			modifiers(modifiers)
		{} 
	};

	/*
	A "KeyRepeatMessage" represents when a key on a keyboard is held down 
	for a system specified length of time necessary to repeat the action
	*/
	struct KeyRepeatMessage 
	{
		std::string character;
		KeyboardModifier modifiers;
		~KeyRepeatMessage() {} 
		KeyRepeatMessage(
			const std::string character, 
			const KeyboardModifier modifiers
		) :
			character(character),
			modifiers(modifiers)
		{} 
	};

	/*
	A "KeyUpMessage" represents when a key on a keyboard is released
	*/
	struct KeyUpMessage 
	{
		std::string character;
		KeyboardModifier modifiers;
		~KeyUpMessage() {} 
		KeyUpMessage(
			const std::string character, 
			const KeyboardModifier modifiers
		) :
			character(character),
			modifiers(modifiers)
		{} 
	};

	typedef std::variant<
			MouseMotionMessage, 
			MouseClickMessage, 
			ScrollMessage,
			KeyDownMessage,
			KeyRepeatMessage,
			KeyUpMessage
		> Message;

}