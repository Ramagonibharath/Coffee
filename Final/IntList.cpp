using namespace std;

#include "IntList.h"
#include "IntStack.h"

namespace iProlog
{

	IntList::IntList(int const head) : head_Renamed(head), tail_Renamed(NULL)
	{
	}

	IntList::IntList(int const X, IntList *const Xs) : head_Renamed(X), tail_Renamed(Xs)
	{
	}

	bool IntList::isEmpty(IntList *const Xs)
	{
	  return NULL == Xs;
	}

	int IntList::head(IntList *const Xs)
	{
	  return Xs->head_Renamed;
	}

IntList *const IntList::empty = 0;

	IntList *IntList::tail(IntList *const Xs)
	{
	  return Xs->tail_Renamed;
	}

	IntList *IntList::cons(int const X, IntList *const Xs)
	{
	  return new IntList(X, Xs);
	}

	IntList *IntList::app(std::vector<int> const &xs, IntList *const Ys)
	{
	  IntList *Zs = Ys;
	  for (int i = xs.size() - 1; i >= 0; i--)
	  {
		Zs = cons(xs[i], Zs);
	  }
	  return Zs;
	}

	IntStack *IntList::toInts(IntList *Xs)
	{
	  IntStack * const is = new IntStack();
	  while (!isEmpty(Xs))
	  {
		is->push(head(Xs));
		Xs = tail(Xs);
	  }
	  return is;
	}

	int IntList::len(IntList *const Xs)
	{
	  return toInts(Xs)->size();
	}

	string IntList::toString()
	{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
	  return toInts(this)->toString();
	}
}
