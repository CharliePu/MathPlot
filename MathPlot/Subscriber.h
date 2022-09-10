#pragma once

#include <memory>
#include <vector>

class Publisher;

class Subscriber: public std::enable_shared_from_this<Subscriber>
{
public:
	virtual void update(const std::shared_ptr<Publisher> &publisher) = 0;
};

