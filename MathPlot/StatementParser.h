#pragma once
#include <memory>
#include <string>

#include "Statement.h"

class StatementParser
{
public:
	std::unique_ptr<Statement> parse(const std::string& s);
};

