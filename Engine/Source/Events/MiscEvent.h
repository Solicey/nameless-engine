#pragma once

#include "Event.h"
#include "ECS/Entity/Entity.h"

namespace NL
{
	class RuntimeCameraSwitchedEvent : public Event
	{
	public:
		RuntimeCameraSwitchedEvent(Entity entity) : m_Entity(entity) {}

		inline Entity GetEntity() const { return m_Entity; }

		EVENT_CLASS_TYPE(RuntimeCameraSwitched)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMisc)

	private:
		Entity m_Entity;
	};
}