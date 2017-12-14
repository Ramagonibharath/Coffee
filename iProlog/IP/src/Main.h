#ifndef MAIN
#define MAIN

#include <string>
#include <vector>
#include <iostream>

namespace iProlog
{

	class Main
	{

  public:
	  static void println(void *const o);

      static void println(std::string const o);
      
      //static void pp(void *const o);

      static void pp(std::string const o);
      
      static void run(const std::string &fname0);
      /*
	  static void srun(const std::string &fname0);

	  static void main(std::vector<std::string> &args);
      */
	};

}


#endif	//#ifndef MAIN
