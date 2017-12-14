using namespace std;

#include "Toks.h"
#include "Main.h"
#include <sstream>

namespace iProlog
{

    string Toks::IF = "if";
    string Toks::AND = "and";
    string Toks::DOT = ".";
    string Toks::HOLDS = "holds";
    string Toks::LISTS = "lists";
    string Toks::IS = "is";

    Toks *Toks::makeToks(const string s, bool const fromFile)
    {
        istream *is;
        if (fromFile)
        {
            is = new ifstream(s.c_str());
        }
        else
        {
            is = new istringstream(s);
        }
        Toks * const T = new Toks(is);
        return T;
    }

    Toks::Toks(istream *input) : iProlog::StreamTokenizer(input)
    {
        resetSyntax();
        eolIsSignificant(false);
        ordinaryChar('.');
        ordinaryChars('!', '/'); // 33-47
        ordinaryChars(':', '@'); // 55-64
        ordinaryChars('[', '`'); // 91-96
        ordinaryChars('{', '~'); // 123-126
        wordChars('_', '_');
        wordChars('a', 'z');
        wordChars('A', 'Z');
        wordChars('0', '9');
        slashStarComments(true);
        slashSlashComments(true);
        ordinaryChar('%');
    }

    string Toks::getWord()
    {
        string t = "";

        int c = TT_EOF;
        c = nextToken();
        while (iswspace(c) && c != TT_EOF)
        {
            c = nextToken();
        }

        switch (c)
        {
        case TT_WORD:
        {
            const char first = sval[0];
            if (isupper(first) || '_' == first)
            {
                t = "v:" + sval;
            }
            else
            {
                try
                {
                    const int n = StringHelper::fromString<int>(sval);
                    if (abs(n) < 1 << 28)
                    {
                        t = "n:" + sval;
                    }
                    else
                    {
                        t = "c:" + sval;
                    }
                }
                //JAVA TO C++ CONVERTER WARNING: 'final' catch parameters are not available in C++:
                //ORIGINAL LINE: catch (final Exception e)
                catch (const exception &e)
                {
                    t = "c:" + sval;
                }
            }
        }
        break;

        case StreamTokenizer::TT_EOF:
        {
            t = "";
        }
        break;

        default:
        {
            t = "" + StringHelper::toString(static_cast<char>(c));
        }

        }
        return t;
    }

    vector<vector<vector<string> > > Toks::toSentences(const string &s, bool const fromFile)
    {
        vector<vector<vector<string> > > Wsss = vector<vector<vector<string> > >();
        vector<vector<string> > Wss = vector<vector<string> >();
        vector<string> Ws = vector<string>();
        Toks * const toks = makeToks(s, fromFile);
        string t = "";
        while ("" != (t = toks->getWord()))
        {

            if (DOT == t)
            {
                Wss.push_back(Ws);
                Wsss.push_back(Wss);
                Wss = vector<vector<string> >();
                Ws = vector<string>();
            }
            else if (("c:" + IF) == t)
            {

                Wss.push_back(Ws);

                Ws = vector<string>();
            }
            else if (("c:" + AND) == t)
            {
                Wss.push_back(Ws);

                Ws = vector<string>();
            }
            else if (("c:" + HOLDS) == t)
            {
                const string w = Ws[0];
                Ws[0] = "h:" + w.substr(2);
            }
            else if (("c:" + LISTS) == t)
            {
                const string w = Ws[0];
                Ws[0] = "l:" + w.substr(2);
            }
            else if (("c:" + IS) == t)
            {
                const string w = Ws[0];
                Ws[0] = "f:" + w.substr(2);
            }
            else
            {
                Ws.push_back(t);
            }
        }
        return Wsss;
    }
    /*
    string Toks::toString(std::vector<void*> &Wsss)
    {
    return Arrays::deepToString(Wsss);
    }

    void Toks::main(std::vector<string> &args)
    {
    Main::pp(toSentences("prog.nl", true));
    }
    */
}
