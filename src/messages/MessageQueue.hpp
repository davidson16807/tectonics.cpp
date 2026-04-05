#pragma once

// std libraries
#include <unordered_map>
#include <queue>
#include <algorithm> // std::transform

// 3rd party libraries
#include <GLFW/glfw3.h>

// in-house functionality
#include <messages/Message.hpp>

namespace messages 
{


	/*
	"MessageQueue" is a proper object oriented class 
	that seals off event driven functionality within glfw,
	making it easier to guarantee the elimination of side effects
	within other parts of code.
	It encapsulates a queue of "messages" 
	(as understood within the context of Model/View/Update architecture),
	and a set of event callback functions.
	The queue is updated by the event callback functions,
	which can be registered and deregistered to a glfw window 
	using the `register()` and `deregister()` methods.
	A deep copy of the queue can be requested using poll(),
	but there is no way for the queue to be modified by external code. 

	MessageQueue also encapsulates state that's relevant to providing enhanced 
	descriptions of control state at a given moment,
	such as tracking the change in the position of a mouse since the last poll.

	glfw event callbacks are required to run the main thread,
	so "MessageQueue" inherits that requirement and leverages it to avoid 
	considering behavior when running in parallel. 
	*/
	class MessageQueue
	{
		std::unordered_map<int, std::string> registered_nonalphanumerics;
		std::queue<Message> queue;
		glm::vec2 last_position;

public:

		MessageQueue():
			registered_nonalphanumerics({
				{GLFW_KEY_F1,   "f1"},
				{GLFW_KEY_F2,   "f2"},
				{GLFW_KEY_F3,   "f3"},
				{GLFW_KEY_F4,   "f4"},
				{GLFW_KEY_F5,   "f5"},
				{GLFW_KEY_F6,   "f6"},
				{GLFW_KEY_F7,   "f7"},
				{GLFW_KEY_F8,   "f8"},
				{GLFW_KEY_F9,   "f9"},
				{GLFW_KEY_F10,  "f10"},
				{GLFW_KEY_F11,  "f11"},
				{GLFW_KEY_F12,  "f12"},
				{GLFW_KEY_F13,  "f13"},
				{GLFW_KEY_F14,  "f14"},
				{GLFW_KEY_F15,  "f15"},
				{GLFW_KEY_F16,  "f16"},
				{GLFW_KEY_F17,  "f17"},
				{GLFW_KEY_F18,  "f18"},
				{GLFW_KEY_F19,  "f19"},
				{GLFW_KEY_F20,  "f20"},
				{GLFW_KEY_F21,  "f21"},
				{GLFW_KEY_F22,  "f22"},
				{GLFW_KEY_F23,  "f23"},
				{GLFW_KEY_F24,  "f24"},
				{GLFW_KEY_F25,  "f25"},
				{GLFW_KEY_LEFT,   "←"},
				{GLFW_KEY_RIGHT,  "→"},
				{GLFW_KEY_UP,     "↑"},
				{GLFW_KEY_DOWN,   "↓"},
				{GLFW_KEY_TAB,    "\t"},
				{GLFW_KEY_ENTER,  "\n"},
				{GLFW_KEY_BACKSPACE,  "backspace"},
				{GLFW_KEY_DELETE,     "delete"},
				{GLFW_KEY_HOME,       "home"},
				{GLFW_KEY_INSERT,     "insert"},
				{GLFW_KEY_END,        "end"},
				{GLFW_KEY_PAGE_UP,    "page up"},
				{GLFW_KEY_PAGE_DOWN,  "page down"},
			})
		{}

		MessageQueue(const std::unordered_map<int, std::string> registered_nonalphanumerics):
			registered_nonalphanumerics(registered_nonalphanumerics)
		{}

		// NONREGULAR/IMPURE METHODS, methods with deliberate side effects
		/*
		"poll()" calls glfwPollEvents to fire events, then returns a copy of the events that have fired.
		*/
		std::queue<Message> poll()
		{
		    glfwPollEvents();
			std::queue<Message> output = queue;
			std::queue<Message> empty;
			std::swap( queue, empty );
			return output;
		}

		void activate(GLFWwindow* window)
		{

    		glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
			    // NOTE: GLFWWindow contains a void* member that we are free to change and access 
			    // to work around capture limitations within C-style function pointers, 
			    // from https://stackoverflow.com/questions/39731561/use-lambda-as-glfwkeyfun
			    MessageQueue& self = *reinterpret_cast<MessageQueue*>(glfwGetWindowUserPointer(window));
				glm::vec2 position(xpos, ypos);
				self.queue.push(MouseMotionMessage(position, position-self.last_position));
				self.last_position = position;
			});

			glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
			    // NOTE: GLFWWindow contains a void* member that we are free to change and access 
			    // to work around capture limitations within C-style function pointers, 
			    // from https://stackoverflow.com/questions/39731561/use-lambda-as-glfwkeyfun
			    MessageQueue& self = *reinterpret_cast<MessageQueue*>(glfwGetWindowUserPointer(window));
				self.queue.push(MouseClickMessage(button, action == GLFW_PRESS, mods));
			});

			glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
			    // NOTE: GLFWWindow contains a void* member that we are free to change and access 
			    // to work around capture limitations within C-style function pointers, 
			    // from https://stackoverflow.com/questions/39731561/use-lambda-as-glfwkeyfun
			    MessageQueue& self = *reinterpret_cast<MessageQueue*>(glfwGetWindowUserPointer(window));
				self.queue.push(ScrollMessage(glm::vec2(xoffset, yoffset)));
			});

			glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			    // NOTE: GLFWWindow contains a void* member that we are free to change and access 
			    // to work around capture limitations within C-style function pointers, 
			    // from https://stackoverflow.com/questions/39731561/use-lambda-as-glfwkeyfun
			    MessageQueue& self = *reinterpret_cast<MessageQueue*>(glfwGetWindowUserPointer(window));

			    std::string character = "";

			    // try checking the keys that we've specially handled:
			    if (self.registered_nonalphanumerics.contains(key))
			    {
			    	character = self.registered_nonalphanumerics[key];
			    }
			    // if that fails, try the keys that GLFW represents using chars:
			    else
			    {
				    const char* maybe_character = glfwGetKeyName(key, scancode);
				    if (maybe_character != NULL)
				    {
					    character = std::string(1, *maybe_character);
				    }
				    // if all else fails, y'all didn't see nothing, 
				    // the caller should have specified registered_nonalphanumerics if he was interested
				    else
				    {
				    	return;
				    }
			    }

				self.queue.push(KeyboardMessage(character, KeyboardModifier(mods), KeyboardAction(action)));

			});

		}

		void deactivate(GLFWwindow* window)
		{
			glfwSetCursorPosCallback(window, NULL);
		}

	};

}

