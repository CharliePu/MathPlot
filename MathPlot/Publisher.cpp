#include "Publisher.h"
#include "Subscriber.h"

void Publisher::subscribe(const std::shared_ptr<Subscriber>& subscriber)
{
	subscribers.insert(subscriber);
}

void Publisher::unsubscribe(const std::shared_ptr<Subscriber>& subscriber)
{
	subscribers.erase(subscriber);
}

void Publisher::notifySubscribers()
{
	for (auto& subscriber : subscribers)
	{
		subscriber->update(shared_from_this());
	}
}
