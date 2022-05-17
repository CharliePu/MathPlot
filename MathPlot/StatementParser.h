#pragma once
#include <string>
#include <optional>

#include "Statement.h"

class StatementParser
{
public:
	[[nodiscard]] std::optional<Statement> parse(const std::string& s) const;
};

