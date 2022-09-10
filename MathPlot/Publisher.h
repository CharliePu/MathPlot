#pragma once

#include <unordered_set>
#include <memory>

class Subscriber;

class Publisher: public std::enable_shared_from_this<Publisher>
{
public:
	virtual ~Publisher() = default;

	void subscribe(const std::shared_ptr<Subscriber> &subscriber);
	void unsubscribe(const std::shared_ptr <Subscriber> &subscriber);
	void notifySubscribers();
private:
	std::unordered_set<std::shared_ptr<Subscriber>> subscribers;
};

