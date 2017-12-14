#ifndef TOKS
#define TOKS

#include "StreamTokenizer.h"
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include "stringhelper.h"

namespace iProlog
{

	/// <summary>
	/// Reads chars from char streams using the current default encoding
	/// </summary>
	class Toks : public iProlog::StreamTokenizer
	{

	  // reserved words - with syntactic function

  public:
	  static std::string IF;
	  static std::string AND;
	  static std::string DOT;
	  static std::string HOLDS;
	  static std::string LISTS; // todo
	  static std::string IS; // todo

	  static Toks *makeToks(const std::string s, bool const fromFile);

	  Toks(istream *input);

	  virtual std::string getWord();

	  static std::vector<std::vector<std::vector<std::string> > > toSentences(const std::string &s, bool const fromFile);
      /*
	  static std::string toString(std::vector<void*> &Wsss);

	  static void main(std::vector<std::string> &args);
      */
	};
}


#endif	//#ifndef TOKS
