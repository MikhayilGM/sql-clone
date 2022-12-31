#include <map>
#include <string>

enum class Command
{
    CREATE = 0,
    DROP,
    DATABASE,
    TABLE,
    INSERTINTO,
    SELECT,
    UPDATE,
    DELETE,
    OPENDATABASE,
};

extern std::map<std::string, Command> CommandParse;

void push_parse_commands();