using namespace std;

#include <chrono>
#include <ctime>

#include "Main.h"
#include "Engine.h"
#include "Prog.h"

namespace iProlog
{

    void Main::println(void *const o)
    {
        cout << o << endl;
    }

    void Main::println(std::string const o)
    {
        cout << o << endl;
    }
    
    /*
    void Main::pp(void *const o)
    {
        cout << o << endl;
    }
    */
    void Main::pp(std::string const o)
    {
        cout << o << endl;
    }


    void Main::run(const string &fname0)
    {
        const bool p = true;

        const string fname = fname0 + ".nl";
        Engine *P;

        if (p)
        {
            P = new Prog(fname);
            pp("CODE");
            (static_cast<Prog*>(P))->ppCode();
        }
        else
        {
            P = new Engine(fname);
        }

        pp("RUNNING");
        chrono::time_point<chrono::steady_clock> t_start = std::chrono::high_resolution_clock::now();
        P->run();
        chrono::time_point<chrono::steady_clock> t_end = std::chrono::high_resolution_clock::now();
        cout << "time=" << std::chrono::duration<double>(t_end - t_start).count() << endl;
    }
    /*
    void Main::srun(const string &fname0)
    {
        const string fname = fname0 + ".nl";
        Prog * const P = new Prog(fname);

        pp("CODE");
        P->ppCode();

        pp("RUNNING");
        auto t_start = std::chrono::high_resolution_clock::now();

        Stream<void*> * const S = P->stream();
        S->forEach([&](void *x)
        {
            Main::pp(P->showTerm(x));
        });

        auto t_end = std::chrono::high_resolution_clock::now();
        cout << "time=" << std::chrono::duration<double>(t_end - t_start).count() << endl;
    }
    */
}

int main(int arg, char **argv)
{
    string fname = string(argv[1]);
    iProlog::Main::run(fname);
}
