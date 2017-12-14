#ifndef OBSTACK
#define OBSTACK

#include <vector>

namespace iProlog
{

	template<typename T>
	class ObStack : public std::vector<T>
	{

  public:
	  T pop()
	  {
          T popped = this->peek();
          this->pop_back();
          return popped;
	  }

	  void push(T const O)
	  {
		this->push_back(O);
	  }

	  T peek()
	  {
		return this->at(this->size() - 1);
	  }
	};

}


#endif	//#ifndef OBSTACK
