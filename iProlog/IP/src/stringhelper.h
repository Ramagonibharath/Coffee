#ifndef STRINGHELPER
#define STRINGHELPER

//----------------------------------------------------------------------------------------
//	Copyright © 2007 - 2017 Tangible Software Solutions Inc.
//	This class can be used by anyone provided that the copyright notice remains intact.
//
//	This class is used to replace some string methods, including
//	conversions to or from strings.
//----------------------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

class StringHelper
{
public:
	static std::vector<std::string> split(const std::string &source, char delimiter)
	{
		std::vector<std::string> output;
		std::istringstream ss(source);
		std::string nextItem;

		while (std::getline(ss, nextItem, delimiter))
		{
			output.push_back(nextItem);
		}

		return output;
	}

	template<typename T>
	static std::string toString(const T &subject)
	{
		std::ostringstream ss;
		ss << subject;
		return ss.str();
	}

    template<typename T>
    static std::string toArrayString(std::vector<T> v)
    {
        if (v.size() == 0) return "[]";

        std::string result = "[";
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            std::ostringstream stm;
            stm << v[i];
            result += stm.str() + ", ";
        }

        // remove the last ", " and add a ']'
        return result.substr(0, result.size() - 2) + ']';
    }

    template<typename T>
	static T fromString(const std::string &subject)
	{
		std::istringstream ss(subject);
		T target;
		ss >> target;
        if (ss.fail())
        {
            throw std::exception();
        }
		return target;
	}
};


#endif	//#ifndef STRINGHELPER
