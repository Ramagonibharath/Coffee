#include <iostream>
#include <sstream>
#include "TermObject.h"
#include "stringhelper.h"

namespace iProlog
{
    std::string TermObject::toString() const
    {
        if (objType == TermObject::ARRAY)
        {
            if (arrayValue.size() == 0) return "[]";

            std::string result = "[";
            for (std::size_t i = 0; i < arrayValue.size(); ++i)
            {
                std::ostringstream stm;
                stm << arrayValue[i].toString();
                result += stm.str() + ", ";
            }

            // remove the last ", " and add a ']'
            return result.substr(0, result.size() - 2) + ']';
        }
        else if (objType == TermObject::INT)
        {
            return StringHelper::toString(intValue);
        }
        else if (objType == TermObject::STRING)
        {
            return strValue;
        }
        else
        {
            return "$null";
        }
    }

}
