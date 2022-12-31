#include <map>
#include <string>
#include "utility.h"

enum class Types
{
    INT, 
    VARCHAR,
    DATE,
};

extern std::map<std::string, Types> TypesParse;

void push_parse_types();

bool check_type(const Types type, const std::string& expression);

bool is_type(const std::string& type_name);