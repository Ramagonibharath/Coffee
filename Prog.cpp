using namespace std;

#include "Prog.h"
#include "Main.h"
#include "Clause.h"
#include "IntList.h"
#include "Spine.h"
#include "TermObject.h"

namespace iProlog
{

	Prog::Prog(const string &fname) : Engine(fname)
	{
	}
    /*
	void Prog::pp(void *const o)
	{
	  Main::pp(o);
	}
    */
    void Prog::pp(std::string const o)
    {
        Main::pp(o);
    }

	void Prog::println(void *const o)
	{
	  Main::println(o);
	}

    void Prog::println(std::string const o)
    {
        Main::println(o);
    }

    string Prog::showTerm(int const x)
    {
        return showTerm(exportTerm(x));
    }

    string Prog::showTerm(TermObject const &O)
	{
        if (O.objType == TermObject::ARRAY)
        {
            return st0(O.arrayValue);
        }
        else if (O.objType == TermObject::INT)
        {
            return StringHelper::toString(O.intValue);
        }
        else if (O.objType == TermObject::STRING)
        {
            return O.strValue;
        }
        else
        {
            return "";
        }
	}

	string Prog::maybeNull(TermObject const &O)
	{
        if (O.objType == TermObject::ARRAY)
        {
            return st0(O.arrayValue);
        }
        else if (O.objType == TermObject::INT)
        {
            return StringHelper::toString(O.intValue);
        }
        else if (O.objType == TermObject::STRING)
        {
            return O.strValue;
        }
        else
        {
            return "$null";
        }
    }

	bool Prog::isListCons(TermObject const &name)
	{
        if (name.objType == TermObject::STRING)
        {
            return "." == name.strValue || "[|]" == name.strValue || "list" == name.strValue;
        }
        return false;
	}

    bool Prog::isOp(TermObject const &name)
	{
        if (name.objType == TermObject::STRING)
        {
            return "/" == name.strValue || "-" == name.strValue || "+" == name.strValue || "=" == name.strValue;
        }
        return false;
	}

	string Prog::st0(std::vector<TermObject> const &args)
	{
	  StringBuilder * const buf = new StringBuilder();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
      const string name = args[0].toString();
	  if (args.size() == 3 && isOp(args[0]))
	  {
		buf->append("(");
		buf->append(maybeNull(args[0]));
		buf->append(" " + name + " ");
		buf->append(maybeNull(args[1]));
		buf->append(")");
	  }
	  else if (args.size() == 3 && isListCons(args[0]))
	  {
		buf->append('[');
		{
		  buf->append(maybeNull(args[1]));
		  TermObject tail = args[2];
		  for (;;)
		  {

			if (TermObject::STRING == tail.objType && ("[]" == tail.strValue || "nil" == tail.strValue))
			{
			  break;
			}
			if (TermObject::ARRAY != tail.objType)
			{
			  buf->append('|');
			  buf->append(maybeNull(tail));
			  break;
			}
            std::vector<TermObject> list = tail.arrayValue;
			if (!(list.size() == 3 && isListCons(list[0])))
			{
			  buf->append('|');
			  buf->append(maybeNull(tail));
			  break;
			}
			else
			{
			  //if (i > 1)
			  buf->append(',');
			  buf->append(maybeNull(list[1]));
			  tail = list[2];
			}
		  }
		}
		buf->append(']');
	  }
	  else if (args.size() == 2 && "$VAR" == name)
	  {
		buf->append("_" + args[1].strValue);
	  }
	  else
	  {
		const string qname = maybeNull(args[0]);
		buf->append(qname);
		buf->append("(");
		for (int i = 1; i < args.size(); i++)
		{
		  const TermObject O = args[i];
		  buf->append(maybeNull(O));
		  if (i < args.size() - 1)
		  {
			buf->append(",");
		  }
		}// it has some bugs... i will fix it soon
		buf->append(")");
	  }
	  return buf->toString();
	}

	void Prog::ppCode()
	{
	  pp("\nSYMS:");

      StringBuilder * const b = new StringBuilder("{");
      bool first = true;
      for (map<string, int>::iterator it = syms->begin(); it != syms->end(); ++it)
      {
          if (!first)
          {
              b->append(", ");
          }
          first = false;
          b->append(it->first);
          b->append("=");
          b->append(it->second);
      }
      b->append("}");
	  pp(b->toString());

	  pp("\nCLAUSES:\n");

	  for (int i = 0; i < clauses.size(); i++)
	  {
		Clause * const C = clauses[i];
		pp("[" + StringHelper::toString(i) + "]:" + showClause(C));
	  }
	  pp("");

	}

	string Prog::showClause(Clause *const s)
	{
	  StringBuilder * const buf = new StringBuilder();
	  const int l = s->hgs.size();
	  buf->append("---base:[" + StringHelper::toString(s->base) + "] neck: " + StringHelper::toString(s->neck) + "-----\n");
	  buf->append(showCells(s->base, s->len)); // TODO
	  buf->append("\n");
	  buf->append(showCell(s->hgs[0]));

	  buf->append(" :- [");
	  for (int i = 1; i < l; i++)
	  {

		const int e = s->hgs[i];
		buf->append(showCell(e));
		if (i < l - 1)
		{
		  buf->append(", ");
		}
	  }

	  buf->append("]\n");

	  buf->append(showTerm(s->hgs[0]));
	  if (l > 1)
	  {
		buf->append(" :- \n");
		for (int i = 1; i < l; i++)
		{
		  const int e = s->hgs[i];
		  buf->append("  ");
		  buf->append(showTerm(e));
		  buf->append("\n");
		}
	  }
	  else
	  {
		buf->append("\n");
	  }
	  return buf->toString();
	}

	void Prog::ppGoals(IntList *bs)
	{
	  while (!IntList::isEmpty(bs))
	  {
		pp(showTerm(IntList::head(bs)));
		bs = IntList::tail(bs);
	  }

	}

	void Prog::ppc(Spine *const S)
	{
	  //stats();
	  IntList * const bs = S->gs;
	  pp("\nppc: t=" + StringHelper::toString(S->ttop) + ",k=" + StringHelper::toString(S->k) + "len=" + StringHelper::toString(IntList::len(bs)));
	  ppGoals(bs);
	}
}
