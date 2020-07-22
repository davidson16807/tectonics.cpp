#pragma once

// std libraries
#include <queue>

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
		std::queue<Message> queue;
		glm::vec2 last_position;

		void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
		{
			glm::vec2 position(xpos, ypos);
			queue.push(MouseMotionMessage(position, position-last_position));
			last_position = position;
		}
		void mouseButtonCallback(GLFWwindow* window, int button, bool action, int mods)
		{
			queue.push(MouseClickMessage(button, action == GLFW_PRESS, mods));
		}
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			queue.push(ScrollMessage(glm::vec2(xoffset, yoffset)));
		}


public:
		MessageQueue()
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
		}
		void deactivate(GLFWwindow* window)
		{
			glfwSetCursorPosCallback(window, NULL);
		}
	};

}