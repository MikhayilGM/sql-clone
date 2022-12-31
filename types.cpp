#include "types.h"

void push_parse_types()
{
    TypesParse["INT"] = Types::INT;
    TypesParse["VARCHAR"] = Types::VARCHAR;
    TypesParse["DATE"] = Types::DATE;
}

bool check_type(const Types type, const std::string& expression)
{
    switch (type)
    {
    case Types::INT:
        return is_number(expression);
        break;

    case Types::VARCHAR:
        return true;
        break;

    case Types::DATE:
        return is_date(expression);
        break;
    
    default:
        break;
    }
    return false;
}

bool is_type(const std::string& type_name)
{
    return TypesParse.contains(type_name);
}