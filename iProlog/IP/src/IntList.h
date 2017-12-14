#ifndef INTLIST
#define INTLIST

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace iProlog { class IntStack; }

namespace iProlog
{
	class IntList
	{

  private:
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
	  const int head_Renamed;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
	  IntList *const tail_Renamed;

  public:
	  virtual ~IntList()
	  {
		  delete tail_Renamed;
	  }

  private:
	  IntList(int const head);

	  IntList(int const X, IntList *const Xs);

  public:
	  static bool isEmpty(IntList *const Xs);

	  static int head(IntList *const Xs);

	  static IntList *const empty;

	  static IntList *tail(IntList *const Xs);

	  static IntList *cons(int const X, IntList *const Xs);

	  static IntList *app(std::vector<int> const &xs, IntList *const Ys);

	  static IntStack *toInts(IntList *Xs);

	  static int len(IntList *const Xs);

	  virtual std::string toString();
	};

}


#endif	//#ifndef INTLIST
