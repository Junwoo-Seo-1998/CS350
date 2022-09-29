#pragma once
#include <functional>
#include "Core/Utils/Math.h"


enum class EventType : int
{
	None = 0,
	WindowClose, WindowResize,
	KeyPressed,
	KeyReleased,
};

enum class EventCategory : int
{
	None = 0,
	ApplicationEvent = BIT(0),
	KeyBoardEvent = BIT(1),
};

class EventDispatcher;

class Event
{
	friend  EventDispatcher;
public:
	virtual ~Event() = default;
	virtual const char* GetEventName() const = 0;
	virtual EventType GetEventType() const = 0;
	virtual int GetCategory() const = 0;

	//debug purpose 
	virtual std::string GetEventNameString() const;

	bool IsInCategory(EventCategory category);

	bool m_Handled = false;
};

class EventDispatcher
{
	template<typename T>
	using EventFunction = std::function<bool(T&)>;
public:
	EventDispatcher(Event& event);
	template<typename T>
	bool Dispatch(EventFunction<T> function);

private:
	Event& m_Event;
};
	

template <typename T>
bool EventDispatcher::Dispatch(EventFunction<T> function)
{
	if (m_Event.GetEventType() == T::GetStaticType())
	{
		m_Event.m_Handled = function(*(T*)&m_Event);
		return true;
	}
	return false;
}

#define EVENT_TYPE(type)			static EventType GetStaticType(){ return type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetEventName() const override { return #type; };
#define EVENT_CATEGORY(category)	virtual int GetCategory() const override { return static_cast<int>(category); }

#define BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

