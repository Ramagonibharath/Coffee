using namespace std;

#include "IntStack.h"
#include "stringhelper.h"
#include <algorithm>
#include <iterator>

namespace iProlog
{

int IntStack::SIZE = 16;
int IntStack::MINSIZE = 1 << 15;

//JAVA TO C++ CONVERTER TODO TASK: Calls to same-class constructors are not supported in C++ prior to C++11:
	IntStack::IntStack()
	{
        InitializeInstanceFields();
        stack = std::vector<int>(SIZE);
        clear();
    }

	IntStack::IntStack(int const size)
	{
	  InitializeInstanceFields();
	  stack = std::vector<int>(size);
	  clear();
	}

	int IntStack::getTop()
	{
	  return top;
	}

	int IntStack::setTop(int const top)
	{
	  return this->top = top;
	}

	void IntStack::clear()
	{
	  //for (int i = 0; i <= top; i++)
	  //stack[i] = 0;
	  top = -1;
	}

	bool IntStack::isEmpty()
	{
	  return top < 0;
	}

	void IntStack::push(int const i)
	{
	  // IO.dump("push:"+i);
	  if (++top >= stack.size())
	  {
		expand();
	  }
	  stack[top] = i;
	}

	int IntStack::pop()
	{
	  const int r = stack[top--];
	  shrink();
	  return r;
	}

	int IntStack::get(int const i)
	{
	  return stack[i];
	}

	void IntStack::set(int const i, int const val)
	{
	  stack[i] = val;
	}

	int IntStack::size()
	{
	  return top + 1;
	}

	void IntStack::expand()
	{
	  const int l = stack.size();
      stack.resize(l << 1);
	}

	void IntStack::shrink()
	{
	  int l = stack.size();
	  if (l <= MINSIZE || top << 2 >= l)
	  {
		return;
	  }
	  l = 1 + (top << 1); // still means shrink to at 1/2 or less of the heap
	  if (top < MINSIZE)
	  {
		l = MINSIZE;
	  }

	  const std::vector<int> newstack = std::vector<int>(l);
      stack.resize(top + 1);
	}

	std::vector<int> IntStack::toArray()
	{
        int array_size = size();
	  std::vector<int> array_Renamed(array_size);

	  if (array_size > 0)
	  {
          for (int i = 0; i < array_size; i++)
          {
              array_Renamed[i] = stack[i];
          }
          
	  }
	  return array_Renamed;
	}

	void IntStack::reverse()
	{
	  int l = size();
	  int h = l >> 1;
	  // Prolog.dump("l="+l);
	  for (int i = 0; i < h; i++)
	  {
		int temp = stack[i];
		stack[i] = stack[l - i - 1];
		stack[l - i - 1] = temp;
	  }
	}

	string IntStack::toString()
	{
	   return StringHelper::toArrayString(toArray());
	}

	void IntStack::InitializeInstanceFields()
	{
		top = 0;
	}
}
