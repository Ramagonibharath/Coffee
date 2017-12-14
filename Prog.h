#ifndef PROG
#define PROG

#include "Engine.h"
#include <string>
#include <vector>
#include <limits>
#include <functional>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace iProlog { class Clause; }
namespace iProlog { class IntList; }
namespace iProlog { class Spine; }
namespace iProlog { class TermObject; }

namespace iProlog
{
	//import java.util.Arrays;

	class Prog : public Engine
	{
  public:
	  Prog(const std::string &fname);

	  //static void pp(void *const o);

      static void pp(std::string const o);
      
      static void println(void *const o);

      static void println(std::string const o);

	  virtual std::string showTerm(TermObject const &O);

      virtual std::string showTerm(int const x);

      static std::string maybeNull(TermObject const &O);

	  static bool isListCons(TermObject const &name);

	  static bool isOp(TermObject const &name);

	  static std::string st0(std::vector<TermObject> const &args);

	  virtual void ppCode();

	  virtual std::string showClause(Clause *const s);

	  /*
	  String showHead(final Cls s) {
	    final int h = s.gs[0];
	    return showCell(h) + "=>" + showTerm(h);
	  }
	  */

	  virtual void ppGoals(IntList *bs);

	  virtual void ppc(Spine *const S);

	  /////////////// end of show
	};

}


#endif	//#ifndef PROG
