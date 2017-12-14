using namespace std;

#include "Spine.h"
#include "IntList.h"

namespace iProlog
{

	Spine::Spine(std::vector<int> const &gs0, int const base, IntList *const gs, int const ttop, int const k, std::vector<int> const &cs)
        : hd(gs0[0])
        , base(base)
        , gs(IntList::tail(IntList::app(gs0, gs))) /* prepends the goals of clause with head hs */
        , ttop(ttop)
	{
	  InitializeInstanceFields();
	  this->k = k;
	  this->cs = cs;
	}

	Spine::Spine(int const hd, int const ttop) : hd(hd), base(0), gs(IntList::empty), ttop(ttop)
	{
	  InitializeInstanceFields();

	  k = -1;
	  cs.clear();
	}

	void Spine::InitializeInstanceFields()
	{
		k = 0;
	}
}
