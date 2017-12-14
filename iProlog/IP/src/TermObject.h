#ifndef TERMOBJECT
#define TERMOBJECT

#include <string>
#include <vector>

namespace iProlog
{
    class TermObject
    {
    public:
        enum OBJECT_TYPE {NONE, INT, STRING, ARRAY};
        OBJECT_TYPE objType;
        int intValue;
        std::string strValue;
        std::vector<TermObject> arrayValue;
        
        std::string toString() const;
    };
}

#endif //#ifndef TERMOBJECT
