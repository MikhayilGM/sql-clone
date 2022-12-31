#include "commands.h"

void push_parse_commands()
{
    CommandParse["CREATE"] = Command::CREATE;
    CommandParse["DROP"] = Command::DROP;
    CommandParse["DATABASE"] = Command::DATABASE;
    CommandParse["TABLE"] = Command::TABLE;
    CommandParse["INSERT_INTO"] = Command::INSERTINTO;
    CommandParse["SELECT"] = Command::SELECT;
    CommandParse["UPDATE"] = Command::UPDATE;
    CommandParse["DELETE"] = Command::DELETE;
    CommandParse["OPEN"] = Command::OPENDATABASE;
}