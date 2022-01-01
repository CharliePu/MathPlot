#pragma once
#include <memory>
#include <string>
#include <optional>

#include "Statement.h"

class StatementParser
{
public:
	std::optional<Statement> parse(const std::string& s);
};

