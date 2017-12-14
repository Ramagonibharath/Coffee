using namespace std;

#include "Engine.h"
#include "Clause.h"
#include "IntStack.h"
#include "ObStack.h"
#include "Spine.h"
#include "IMap.h"
#include "IntMap.h"
#include "Toks.h"
#include "Main.h"
#include "IntList.h"
#include "Prog.h"
#include "TermObject.h"

namespace iProlog
{
    int Engine::MAXIND = 3; // number of index args
    int Engine::START_INDEX = 20;

    Engine::Engine(const string &fname)
    {
        InitializeInstanceFields();
        syms = new map<string, int>();

        makeHeap();

        trail = new IntStack();
        ustack = new IntStack();

        clauses = dload(fname);

        cls = toNums(clauses);

        spines = new ObStack<Spine *>();

        query = init();
        vmaps = vcreate(MAXIND);
        imaps = index(clauses, vmaps);

    }

    int Engine::MINSIZE = 1 << 15;

    int Engine::tag(int const t, int const w)
    {
        return -((w << 3) + t);
    }

    int Engine::detag(int const w)
    {
        return -w >> 3;
    }

    int Engine::tagOf(int const w)
    {
        return -w & 7;
    }

    int Engine::addSym(const string &sym)
    {
        int I;
        if (syms->find(sym) == syms->end())
        {
            const int i = syms->size();
            I = int(i);
            (*syms)[sym] = I;
            slist.push_back(sym);
        }
        else
        {
            I = syms->at(sym);
        }
        return I;
    }

    string Engine::getSym(int const w)
    {
        if (w < 0 || w >= slist.size())
        {
            return "BADSYMREF=" + StringHelper::toString(w);
        }
        return slist[w];
    }

    void Engine::makeHeap()
    {
        makeHeap(MINSIZE);
    }

    void Engine::makeHeap(int const size)
    {
        heap = vector<int>(size);
        clear();
    }

    int Engine::getTop()
    {
        return top;
    }

    int Engine::setTop(int const top)
    {
        return this->top = top;
    }

    void Engine::clear()
    {
        //for (int i = 0; i <= top; i++)
        //heap[i] = 0;
        top = -1;
    }

    void Engine::push(int const i)
    {
        heap[++top] = i;
    }

    int Engine::size()
    {
        return top + 1;
    }

    void Engine::expand()
    {
        const int l = heap.size();
        heap.resize(l << 1);
    }

    void Engine::ensureSize(int const more)
    {
        if (1 + top + more >= heap.size())
        {
            expand();
        }
    }

    vector<vector<string> > Engine::maybeExpand(const vector<string> &Ws)
    {
        const string W = Ws[0];
        vector<vector<string> > Rss;
        if (W.length() < 2 || "l:" != W.substr(0, 2))
        {
            return Rss;
        }

        const int l = Ws.size();
        const string V = W.substr(2);
        for (int i = 1; i < l; i++)
        {
            vector<string> Rs(4);
            string Vi = 1 == i ? V : V + "__" + StringHelper::toString(i - 1);
            string Vii = V + "__" + StringHelper::toString(i);
            Rs[0] = "h:" + Vi;
            Rs[1] = "c:list";
            Rs[2] = Ws[i];
            Rs[3] = i == l - 1 ? "c:nil" : "v:" + Vii;
            Rss.push_back(Rs);
        }
        return Rss;
    }

    vector<vector<string> > Engine::mapExpand(const vector<vector<string> > &Wss)
    {
        vector<vector<string> > Rss;
        for (vector<vector<string> >::const_iterator Ws = Wss.begin(); Ws != Wss.end(); ++Ws)
        {
            const vector<vector<string> > Hss = maybeExpand(*Ws);

            if (0 == Hss.size())
            {
                vector<string> ws(Ws->size());
                for (int i = 0; i < ws.size(); i++)
                {
                    ws[i] = Ws->at(i);
                }
                Rss.push_back(ws);
            }
            else
            {
                for (vector<vector<string> >::const_iterator X = Hss.begin(); X != Hss.end(); ++X)
                {
                    Rss.push_back(*X);
                }
            }
        }
        return Rss;
    }

    vector<Clause*> Engine::dload(const string &s)
    {
        const bool fromFile = true;
        const vector<vector<vector<string> > > Wsss = Toks::toSentences(s, fromFile);

        vector<Clause*> Cs;

        for (vector<vector<vector<string> > >::const_iterator Wss = Wsss.begin(); Wss != Wsss.end(); ++Wss)
        {
            // clause starts here
            list<pair<string, IntStack*> > refs_list;
            map<string, list<pair<string, IntStack*> >::iterator> refs;
            IntStack * const cs = new IntStack();
            IntStack * const gs = new IntStack();

            vector<vector<string> > Rss = mapExpand(*Wss);
            int k = 0;
            for (vector<vector<string> >::iterator ws = Rss.begin(); ws != Rss.end(); ++ws)
            {

                // head or body element starts here

                const int l = ws->size();
                gs->push(tag(R, k++));
                cs->push(tag(A, l));

                for (vector<string>::iterator w = ws->begin(); w != ws->end(); ++w)
                {

                    // head or body subterm starts here

                    if (1 == w->length())
                    {
                        *w = "c:" + *w;
                    }

                    const string L = w->substr(2);

                    switch (w->at(0))
                    {
                    case 'c':
                        cs->push(encode(C, L));
                        k++;
                        break;
                    case 'n':
                        cs->push(encode(N, L));
                        k++;
                        break;
                    case 'v':
                    {
                        IntStack *Is;
                        if (refs.find(L) == refs.end())
                        {
                            Is = new IntStack();
                            refs_list.push_back(std::pair<string, IntStack*>(L, Is));
                            list<pair<string, IntStack*> >::iterator back_iter = refs_list.end();
                            --back_iter;
                            refs[L] = back_iter;
                        }
                        else
                        {
                            Is = refs.at(L)->second;
                        }
                        Is->push(k);
                        cs->push(tag(BAD, k)); // just in case we miss this
                        k++;
                    }
                    break;
                    case 'h':
                    {
                        IntStack *Is;
                        if (refs.find(L) == refs.end())
                        {
                            Is = new IntStack();
                            refs_list.push_back(pair<string, IntStack*>(L, Is));
                            list<pair<string, IntStack*> >::iterator back_iter = refs_list.end();
                            --back_iter;
                            refs[L] = back_iter;
                        }
                        else
                        {
                            Is = refs.at(L)->second;
                        }
                        Is->push(k - 1);
                        cs->set(k - 1, tag(A, l - 1));
                        gs->pop();
                    }
                    break;
                    default:
                        Main::pp("FORGOTTEN=" + *w);
                    } // end subterm
                } // end element
            } // end clause

              // linker

            for (list<pair<string, IntStack*> >::iterator K = refs_list.begin(); K != refs_list.end(); ++K)
            {
                IntStack * const Is = K->second;

                // finding the A among refs
                int leader = -1;
                std::vector<int> tmparray = Is->toArray();
                for (vector<int>::const_iterator j = tmparray.begin(); j != tmparray.end(); ++j)
                {
                    if (A == tagOf(cs->get(*j)))
                    {
                        leader = *j;

                        break;
                    }
                }
                if (-1 == leader)
                {
                    // for vars, first V others U
                    leader = Is->get(0);
                    for (vector<int>::const_iterator i = tmparray.begin(); i != tmparray.end(); ++i)
                    {
                        if (*i == leader)
                        {
                            cs->set(*i, tag(V, *i));
                        }
                        else
                        {
                            cs->set(*i, tag(U, leader));
                        }

                    }
                }
                else
                {
                    for (vector<int>::const_iterator i = tmparray.begin(); i != tmparray.end(); ++i)
                    {
                        if (*i == leader)
                        {
                            continue;
                        }
                        cs->set(*i, tag(R, leader));
                    }
                }
            }

            const int neck = 1 == gs->size() ? cs->size() : detag(gs->get(1));
            vector<int> tgs = gs->toArray();

            Clause * const C = putClause(cs->toArray(), tgs, neck);

            Cs.push_back(C);

        } // end clause set

        const int ccount = Cs.size();
        vector<Clause*> cls(ccount);
        for (int i = 0; i < ccount; i++)
        {
            cls[i] = Cs[i];
        }
        return cls;
    }

    vector<int> Engine::toNums(vector<Clause*> &clauses)
    {
        const int l = clauses.size();
        vector<int> cls(l);
        for (int i = 0; i < l; i++)
        {
            cls[i] = i;
        }
        return cls;
    }

    int Engine::encode(int const t, const string &s)
    {
        int w;
        try
        {
            w = StringHelper::fromString<int>(s);
        }
        catch (const exception &e)
        {
            if (C == t)
            {
                w = addSym(s);
            }
            else
            {
                //pp("bad in encode=" + t + ":" + s);
                return tag(BAD, 666);
            }
        }
        return tag(t, w);
    }

    bool Engine::isVAR(int const x)
    {
        //final int t = tagOf(x);
        //return V == t || U == t;
        return tagOf(x) < 2;
    }

    int Engine::getRef(int const x)
    {
        return heap[detag(x)];
    }

    void Engine::setRef(int const w, int const r)
    {
        heap[detag(w)] = r;
    }

    void Engine::unwindTrail(int const savedTop)
    {
        while (savedTop < trail->getTop())
        {
            const int href = trail->pop();
            // assert href is var

            setRef(href, href);
        }
    }

    int Engine::deref(int x)
    {
        while (isVAR(x))
        {
            const int r = getRef(x);
            if (r == x)
            {
                break;
            }
            x = r;
        }
        return x;
    }

    string Engine::showTerm(int const x)
    {
        return showTerm(exportTerm(x));
    }

    string Engine::showTerm(TermObject const &O)
    {
        return O.toString();
    }

    void Engine::ppTrail()
    {
        for (int i = 0; i <= trail->getTop(); i++)
        {
            const int t = trail->get(i);
            Main::pp("trail[" + StringHelper::toString(i) + "]=" + showCell(t) + ":" + showTerm(t));
        }
    }

    TermObject Engine::exportTerm(int x)
    {
        x = deref(x);

        const int t = tagOf(x);
        const int w = detag(x);

        TermObject res;
        switch (t)
        {
        case C:
            res.objType = TermObject::STRING;
            res.strValue = getSym(w);
            break;
        case N:
            res.objType = TermObject::INT;
            res.intValue = w;
            break;
        case V:
            //case U:
            res.objType = TermObject::STRING;
            res.strValue = "V" + StringHelper::toString(w);
            break;
        case R:
        {
            const int a = heap[w];
            if (A != tagOf(a))
            {
                res.objType = TermObject::STRING;
                res.strValue = "*** should be A, found=" + showCell(a);
                return res;
            }
            const int n = detag(a);
            vector<TermObject> arr(n);
            const int k = w + 1;
            for (int i = 0; i < n; i++)
            {
                const int j = k + i;
                arr[i] = exportTerm(heap[j]);
            }
            res.objType = TermObject::ARRAY;
            res.arrayValue = arr;
        }
        break;
        default:
            res.objType = TermObject::STRING;
            res.strValue = "*BAD TERM*" + showCell(x);
        }
        return res;
    }

    vector<int> Engine::getSpine(vector<int> &cs)
    {
        const int a = cs[1];
        const int w = detag(a);
        vector<int> rs = vector<int>(w - 1);
        for (int i = 0; i < w - 1; i++)
        {
            const int x = cs[3 + i];
            const int t = tagOf(x);
            if (R != t)
            {
                Main::pp("*** getSpine: unexpected tag=" + StringHelper::toString(t));
                return vector<int>();
            }
            rs[i] = detag(x);
        }
        return rs;
    }

    string Engine::showCell(int const w)
    {
        const int t = tagOf(w);
        const int val = detag(w);
        string s = "";
        switch (t)
        {
        case V:
            s = "v:" + StringHelper::toString(val);
            break;
        case U:
            s = "u:" + StringHelper::toString(val);
            break;
        case N:
            s = "n:" + StringHelper::toString(val);
            break;
        case C:
            s = "c:" + getSym(val);
            break;
        case R:
            s = "r:" + StringHelper::toString(val);
            break;
        case A:
            s = "a:" + StringHelper::toString(val);
            break;
        default:
            s = "*BAD*=" + StringHelper::toString(w);
        }
        return s;
    }

    string Engine::showCells(int const base, int const len)
    {
        StringBuilder * const buf = new StringBuilder();
        for (int k = 0; k < len; k++)
        {
            const int instr = heap[base + k];

            buf->append("[" + StringHelper::toString(base + k) + "]");
            buf->append(showCell(instr));
            buf->append(" ");
        }
        return buf->toString();
    }

    string Engine::showCells(vector<int> &cs)
    {
        StringBuilder * const buf = new StringBuilder();
        for (int k = 0; k < cs.size(); k++)
        {
            buf->append("[" + StringHelper::toString(k) + "]");
            buf->append(showCell(cs[k]));
            buf->append(" ");
        }
        return buf->toString();
    }

    void Engine::ppc(Spine *const C)
    {
        // override
    }

    void Engine::ppGoals(IntList *const gs)
    {
        // override
    }

    void Engine::ppSpines()
    {
        // override
    }

    bool Engine::unify(int const base)
    {
        while (!ustack->isEmpty())
        {
            const int x1 = deref(ustack->pop());
            const int x2 = deref(ustack->pop());
            if (x1 != x2)
            {
                const int t1 = tagOf(x1);
                const int t2 = tagOf(x2);
                const int w1 = detag(x1);
                const int w2 = detag(x2);

                if (isVAR(x1))
                { // unb. var. v1
                    if (isVAR(x2) && w2 > w1)
                    { // unb. var. v2
                        heap[w2] = x1;
                        if (w2 <= base)
                        {
                            trail->push(x2);
                        }
                    }
                    else
                    { // x2 nonvar or older
                        heap[w1] = x2;
                        if (w1 <= base)
                        {
                            trail->push(x1);
                        }
                    }
                }
                else if (isVAR(x2))
                { // x1 is NONVAR
                    heap[w2] = x1;
                    if (w2 <= base)
                    {
                        trail->push(x2);
                    }
                }
                else if (R == t1 && R == t2)
                { // both should be R
                    if (!unify_args(w1, w2))
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool Engine::unify_args(int const w1, int const w2)
    {
        const int v1 = heap[w1];
        const int v2 = heap[w2];
        // both should be A
        const int n1 = detag(v1);
        const int n2 = detag(v2);
        if (n1 != n2)
        {
            return false;
        }
        const int b1 = 1 + w1;
        const int b2 = 1 + w2;
        for (int i = n1 - 1; i >= 0; i--)
        {
            const int i1 = b1 + i;
            const int i2 = b2 + i;
            const int u1 = heap[i1];
            const int u2 = heap[i2];
            if (u1 == u2)
            {
                continue;
            }
            ustack->push(u2);
            ustack->push(u1);
        }
        return true;
    }

    Clause *Engine::putClause(vector<int> cs, vector<int> &gs, int const neck)
    {
        const int base = size();
        const int b = tag(V, base);
        const int len = cs.size();
        pushCells(b, 0, len, cs);
        for (int i = 0; i < gs.size(); i++)
        {
            gs[i] = relocate(b, gs[i]);
        }
        const vector<int> xs = getIndexables(gs[0]);
        return new Clause(len, gs, base, neck, xs);
    }

    int Engine::relocate(int const b, int const cell)
    {
        return tagOf(cell) < 3 ? cell + b : cell;
    }

    void Engine::pushCells(int const b, int const from, int const to, int const base)
    {
        ensureSize(to - from);
        for (int i = from; i < to; i++)
        {
            push(relocate(b, heap[base + i]));
        }
    }

    void Engine::pushCells(int const b, int const from, int const to, vector<int> &cs)
    {
        ensureSize(to - from);
        for (int i = from; i < to; i++)
        {
            push(relocate(b, cs[i]));
        }
    }

    int Engine::pushHead(int const b, Clause *const C)
    {
        pushCells(b, 0, C->neck, C->base);
        const int head = C->hgs[0];
        return relocate(b, head);
    }

    vector<int> Engine::pushBody(int const b, int const head, Clause *const C)
    {
        pushCells(b, C->neck, C->len, C->base);
        const int l = C->hgs.size();
        vector<int> gs(l);
        gs[0] = head;
        for (int k = 1; k < l; k++)
        {
            const int cell = C->hgs[k];
            gs[k] = relocate(b, cell);
        }
        return gs;
    }

    void Engine::makeIndexArgs(Spine *const G, int const goal)
    {
        if (0 != G->xs.size())
        {
            return;
        }

        const int p = 1 + detag(goal);
        const int n = min(MAXIND, detag(getRef(goal)));

        vector<int> xs(MAXIND);

        for (int i = 0; i < n; i++)
        {
            const int cell = deref(heap[p + i]);
            xs[i] = cell2index(cell);
        }

        G->xs = xs;

        if (0 == imaps.size())
        {
            return;
        }
        const vector<int> cs = IMap<int>::get(imaps, vmaps, xs);
        G->cs = cs;
    }

    vector<int> Engine::getIndexables(int const ref)
    {
        const int p = 1 + detag(ref);
        const int n = detag(getRef(ref));
        vector<int> xs(MAXIND);
        for (int i = 0; i < MAXIND && i < n; i++)
        {
            const int cell = deref(heap[p + i]);
            xs[i] = cell2index(cell);
        }
        return xs;
    }

    int Engine::cell2index(int const cell)
    {
        int x = 0;
        const int t = tagOf(cell);
        switch (t)
        {
        case R:
            x = getRef(cell);
            break;
        case C:
        case N:
            x = cell;
            break;
            // 0 otherwise - assert: tagging with R,C,N <>0
        }
        return x;
    }

    bool Engine::match(vector<int> &xs, Clause *const C0)
    {
        for (int i = 0; i < MAXIND; i++)
        {
            const int x = xs[i];
            const int y = C0->xs[i];
            if (0 == x || 0 == y)
            {
                continue;
            }
            if (x != y)
            {
                return false;
            }
        }
        return true;
    }

    Spine *Engine::unfold(Spine *const G)
    {

        const int ttop = trail->getTop();
        const int htop = getTop();
        const int base = htop + 1;

        const int goal = IntList::head(G->gs);

        makeIndexArgs(G, goal);

        const int last = G->cs.size();
        for (int k = G->k; k < last; k++)
        {
            Clause * const C0 = clauses[G->cs[k]];

            if (!match(G->xs, C0))
            {
                continue;
            }

            const int base0 = base - C0->base;
            const int b = tag(V, base0);
            const int head = pushHead(b, C0);

            ustack->clear(); // set up unification stack

            ustack->push(head);
            ustack->push(goal);

            if (!unify(base))
            {
                unwindTrail(ttop);
                setTop(htop);
                continue;
            }
             vector<int> gs = pushBody(b, head, C0);
            IntList * const newgs = IntList::tail(IntList::app(gs, IntList::tail(G->gs)));
            G->k = k + 1;
            if (!IntList::isEmpty(newgs))
            {
                return new Spine(gs, base, IntList::tail(G->gs), ttop, 0, cls);
            }
            else
            {
                return answer(ttop);
            }
        } // end for
        return 0;
    }

    Clause *Engine::getQuery()
    {
        return clauses[clauses.size() - 1];
    }

    Spine *Engine::init()
    {
        const int base = size();

        Clause * const G = getQuery();
        Spine * const Q = new Spine(G->hgs, base, IntList::empty, trail->getTop(), 0, cls);
        spines->push(Q);
        return Q;
    }

    Spine *Engine::answer(int const ttop)
    {
        return new Spine(spines->at(0)->hd, ttop);
    }

    bool Engine::hasClauses(Spine *const S)
    {
        return S->k < S->cs.size();
    }

    bool Engine::hasGoals(Spine *const S)
    {
        return !IntList::isEmpty(S->gs);
    }

    void Engine::popSpine()
    {
        Spine * const G = spines->pop();
        unwindTrail(G->ttop);
        setTop(G->base - 1);
    }

    Spine *Engine::yield()
    {
        while (!spines->empty())
        {
            Spine * const G = spines->peek();
            if (!hasClauses(G))
            {
                popSpine(); // no clauses left
                continue;
            }
            Spine * const C = unfold(G);
            if (0 == C)
            {
                popSpine(); // no matches
                continue;
            }
            if (hasGoals(C))
            {
                spines->push(C);
                continue;
            }
            return C; // answer
        }
        return NULL;
    }

    TermObject Engine::ask()
    {
        query = yield();
        if (0 == query)
        {
            TermObject R;
            R.objType == TermObject::NONE;
            return R;
        }
        const int res = answer(query->ttop)->hd;
        const TermObject R = exportTerm(res);
        unwindTrail(query->ttop);
        return R;
    }

    void Engine::run()
    {
        long long ctr = 0LL;
        for (;; ctr++)
        {
            const TermObject A = ask();
            if (A.objType == TermObject::NONE)
            {
                break;
            }
            if (ctr < 5)
            {
                Prog::println("[" + StringHelper::toString(ctr) + "] " + "*** ANSWER=" + showTerm(A));
            }
        }
        if (ctr > 5)
        {
            Prog::println("...");
        }
        Prog::println("TOTAL ANSWERS=" + StringHelper::toString(ctr));
    }

    vector<IntMap*> Engine::vcreate(int const l)
    {
        vector<IntMap*> vss(l);
        for (int i = 0; i < l; i++)
        {
            vss[i] = new IntMap();
        }
        return vss;
    }

    void Engine::put(vector<IMap<int>*> &imaps, vector<IntMap*> &vss, vector<int> const &keys, int const val)
    {
        for (int i = 0; i < imaps.size(); i++)
        {
            const int key = keys[i];
            if (key != 0)
            {
                IMap<int>::put(imaps, i, key, val);
            }
            else
            {
                vss[i]->add(val);
            }
        }
    }

    vector<IMap<int>*> Engine::index(vector<Clause*> &clauses, vector<IntMap*> &vmaps)
    {
        if (clauses.size() < START_INDEX)
        {
            return vector<IMap<int>*>();
        }

        vector<IMap<int>*> imaps = IMap<int>::create(vmaps.size());
        for (int i = 0; i < clauses.size(); i++)
        {
            Clause * const c = clauses[i];

            put(imaps, vmaps, c->xs, i + 1); // $$$ UGLY INC

        }
        Main::pp("INDEX");
        Main::pp(IMap<int>::show(imaps));

        std::string result = "[";
        for (std::size_t i = 0; i < vmaps.size(); ++i)
        {
            std::ostringstream stm;
            stm << vmaps[i]->toString();
            result += stm.str() + ", ";
        }

        Main::pp(result.substr(0, result.size() - 2) + ']');
        return imaps;
    }

    void Engine::InitializeInstanceFields()
    {
        top = 0;
    }
}
