#include "StatementParser.h"
#include <unordered_map>

#include "ExpressionParser.h"

std::unique_ptr<Statement> StatementParser::parse(const std::string& s)
{
    std::unordered_map<Relation, std::string> m {
        { Relation::greaterEqual, ">="},
        { Relation::lessEqual, "<="},
        { Relation::greater, ">"},
        { Relation::less, "<"},
        { Relation::equal, "="},
    };

    size_t operatorPos = 0;
    Relation relation;
    for (auto &pair : m)
    {
        auto pos = s.find(pair.second);
        if (pos != s.npos)
        {
            relation = pair.first;
            operatorPos = pos;
            break;
        }
    }

    if (operatorPos == 0 || operatorPos == s.size() - 1)
    {
        return nullptr;
    }

    ExpressionParser parser;

    auto l = parser.parse(s.substr(0, operatorPos));
    auto r = parser.parse(s.substr(operatorPos + m[relation].size()));

    if (!l || !r)
    {
        return nullptr;
    }


	return std::make_unique<Statement>(std::move(l), relation, std::move(r));
}
