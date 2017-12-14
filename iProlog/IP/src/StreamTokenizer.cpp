using namespace std;

#include <algorithm>
#include "StreamTokenizer.h"

namespace iProlog
{
    int StreamTokenizer::NEED_CHAR = std::numeric_limits<int>::max();
    int StreamTokenizer::SKIP_LF = std::numeric_limits<int>::max() - 1;

	StreamTokenizer::StreamTokenizer(istream *is=NULL)
	{
        InitializeInstanceFields();
        wordChars('a', 'z');
        wordChars('A', 'Z');
        wordChars(128 + 32, 255);
        whitespaceChars(0, ' ');
        commentChar('/');
        quoteChar('"');
        quoteChar('\'');
        parseNumbers();
		input = is;
	}

	void StreamTokenizer::resetSyntax()
	{
		for (int i = ctype.size(); --i >= 0;)
		{
			ctype[i] = 0;
		}
	}

	void StreamTokenizer::wordChars(int low, int hi)
	{
		if (low < 0)
		{
			low = 0;
		}
		if (hi >= ctype.size())
		{
			hi = ctype.size() - 1;
		}
		while (low <= hi)
		{
			ctype[low++] |= CT_ALPHA;
		}
	}

	void StreamTokenizer::whitespaceChars(int low, int hi)
	{
		if (low < 0)
		{
			low = 0;
		}
		if (hi >= ctype.size())
		{
			hi = ctype.size() - 1;
		}
		while (low <= hi)
		{
			ctype[low++] = CT_WHITESPACE;
		}
	}

	void StreamTokenizer::ordinaryChars(int low, int hi)
	{
		if (low < 0)
		{
			low = 0;
		}
		if (hi >= ctype.size())
		{
			hi = ctype.size() - 1;
		}
		while (low <= hi)
		{
			ctype[low++] = 0;
		}
	}

	void StreamTokenizer::ordinaryChar(int ch)
	{
		if (ch >= 0 && ch < ctype.size())
		{
			ctype[ch] = 0;
		}
	}

	void StreamTokenizer::commentChar(int ch)
	{
		if (ch >= 0 && ch < ctype.size())
		{
			ctype[ch] = CT_COMMENT;
		}
	}

	void StreamTokenizer::quoteChar(int ch)
	{
		if (ch >= 0 && ch < ctype.size())
		{
			ctype[ch] = CT_QUOTE;
		}
	}

	void StreamTokenizer::parseNumbers()
	{
		for (int i = '0'; i <= '9'; i++)
		{
			ctype[i] |= CT_DIGIT;
		}
		ctype['.'] |= CT_DIGIT;
		ctype['-'] |= CT_DIGIT;
	}

	void StreamTokenizer::eolIsSignificant(bool flag)
	{
		eolIsSignificantP = flag;
	}

	void StreamTokenizer::slashStarComments(bool flag)
	{
		slashStarCommentsP = flag;
	}

	void StreamTokenizer::slashSlashComments(bool flag)
	{
		slashSlashCommentsP = flag;
	}

	void StreamTokenizer::lowerCaseMode(bool fl)
	{
		forceLower = fl;
	}

	int StreamTokenizer::read()
	{
		if (input != 0)
		{
            char * buffer = new char[1];
            input->read(buffer, 1);
            if (*input)
            {
                return buffer[0];
            }
            else
            {
                return -1;
            }
		}
        else
        {
            return -1;
        }
	}

	int StreamTokenizer::nextToken()
	{
		if (pushedBack)
		{
			pushedBack = false;
			return ttype;
		}
		std::vector<char> ct = ctype;
		sval = "";

		int c = peekc;
		if (c < 0)
		{
			c = NEED_CHAR;
		}
		if (c == SKIP_LF)
		{
			c = read();
			if (c < 0)
			{
				return ttype = TT_EOF;
			}
			if (c == '\n')
			{
				c = NEED_CHAR;
			}
		}
		if (c == NEED_CHAR)
		{
			c = read();
			if (c < 0)
			{
				return ttype = TT_EOF;
			}
		}
		ttype = c; // Just to be safe

		/* Set peekc so that the next invocation of nextToken will read
		 * another character unless peekc is reset in this invocation
		 */
		peekc = NEED_CHAR;

		int ctype = c < 256 ? ct[c] : CT_ALPHA;
		while ((ctype & CT_WHITESPACE) != 0)
		{
			if (c == '\r')
			{
				LINENO++;
				if (eolIsSignificantP)
				{
					peekc = SKIP_LF;
					return ttype = TT_EOL;
				}
				c = read();
				if (c == '\n')
				{
					c = read();
				}
			}
			else
			{
				if (c == '\n')
				{
					LINENO++;
					if (eolIsSignificantP)
					{
						return ttype = TT_EOL;
					}
				}
				c = read();
			}
			if (c < 0)
			{
				return ttype = TT_EOF;
			}
			ctype = c < 256 ? ct[c] : CT_ALPHA;
		}

		if ((ctype & CT_DIGIT) != 0)
		{
			bool neg = false;
			if (c == '-')
			{
				c = read();
				if (c != '.' && (c < '0' || c > '9'))
				{
					peekc = c;
					return ttype = '-';
				}
				neg = true;
			}
			double v = 0;
			int decexp = 0;
			int seendot = 0;
			while (true)
			{
				if (c == '.' && seendot == 0)
				{
					seendot = 1;
				}
				else if ('0' <= c && c <= '9')
				{
					v = v * 10 + (c - '0');
					decexp += seendot;
				}
				else
				{
					break;
				}
				c = read();
			}
			peekc = c;
			if (decexp != 0)
			{
				double denom = 10;
				decexp--;
				while (decexp > 0)
				{
					denom *= 10;
					decexp--;
				}
				/* Do one division of a likely-to-be-more-accurate number */
				v = v / denom;
			}
			nval = neg ? -v : v;
			return ttype = TT_NUMBER;
		}

		if ((ctype & CT_ALPHA) != 0)
		{
			int i = 0;
			do
			{
				if (i >= buf.size())
				{
					buf.resize(buf.size() * 2);
				}
				buf[i++] = static_cast<char>(c);
				c = read();
				ctype = c < 0 ? CT_WHITESPACE : c < 256 ? ct[c] : CT_ALPHA;
			} while ((ctype & (CT_ALPHA | CT_DIGIT)) != 0);
			peekc = c;
            string tmpstr(buf.begin(), buf.end());
            sval = tmpstr.substr(0, i);
			if (forceLower)
			{
                std::transform(sval.begin(), sval.end(), sval.begin(), ::tolower);
			}
			return ttype = TT_WORD;
		}

		if ((ctype & CT_QUOTE) != 0)
		{
			ttype = c;
			int i = 0;
			/* Invariants (because \Octal needs a lookahead):
			 *   (i)  c contains char value
			 *   (ii) d contains the lookahead
			 */
			int d = read();
			while (d >= 0 && d != ttype && d != '\n' && d != '\r')
			{
				if (d == '\\')
				{
					c = read();
					int first = c; // To allow \377, but not \477
					if (c >= '0' && c <= '7')
					{
						c = c - '0';
						int c2 = read();
						if ('0' <= c2 && c2 <= '7')
						{
							c = (c << 3) + (c2 - '0');
							c2 = read();
							if ('0' <= c2 && c2 <= '7' && first <= '3')
							{
								c = (c << 3) + (c2 - '0');
								d = read();
							}
							else
							{
								d = c2;
							}
						}
						else
						{
						  d = c2;
						}
					}
					else
					{
						switch (c)
						{
						case 'a':
							c = 0x7;
							break;
						case 'b':
							c = '\b';
							break;
						case 'f':
							c = 0xC;
							break;
						case 'n':
							c = '\n';
							break;
						case 'r':
							c = '\r';
							break;
						case 't':
							c = '\t';
							break;
						case 'v':
							c = 0xB;
							break;
						}
						d = read();
					}
				}
				else
				{
					c = d;
					d = read();
				}
				if (i >= buf.size())
				{
					buf.resize(buf.size() * 2);
				}
				buf[i++] = static_cast<char>(c);
			}

			/* If we broke out of the loop because we found a matching quote
			 * character then arrange to read a new character next time
			 * around; otherwise, save the character.
			 */
			peekc = (d == ttype) ? NEED_CHAR : d;

            string tmpstr(buf.begin(), buf.end());
            sval = tmpstr.substr(0, i);
            return ttype;
		}

		if (c == '/' && (slashSlashCommentsP || slashStarCommentsP))
		{
			c = read();
			if (c == '*' && slashStarCommentsP)
			{
				int prevc = 0;
				while ((c = read()) != '/' || prevc != '*')
				{
					if (c == '\r')
					{
						LINENO++;
						c = read();
						if (c == '\n')
						{
							c = read();
						}
					}
					else
					{
						if (c == '\n')
						{
							LINENO++;
							c = read();
						}
					}
					if (c < 0)
					{
						return ttype = TT_EOF;
					}
					prevc = c;
				}
				return nextToken();
			}
			else if (c == '/' && slashSlashCommentsP)
			{
				while ((c = read()) != '\n' && c != '\r' && c >= 0)
				{
					;
				}
				peekc = c;
				return nextToken();
			}
			else
			{
				/* Now see if it is still a single line comment */
				if ((ct['/'] & CT_COMMENT) != 0)
				{
					while ((c = read()) != '\n' && c != '\r' && c >= 0)
					{
						;
					}
					peekc = c;
					return nextToken();
				}
				else
				{
					peekc = c;
					return ttype = '/';
				}
			}
		}

		if ((ctype & CT_COMMENT) != 0)
		{
			while ((c = read()) != '\n' && c != '\r' && c >= 0)
			{
				;
			}
			peekc = c;
			return nextToken();
		}

		return ttype = c;
	}

	void StreamTokenizer::pushBack()
	{
		if (ttype != TT_NOTHING) // No-op if nextToken() not called
		{
			pushedBack = true;
		}
	}

	int StreamTokenizer::lineno()
	{
		return LINENO;
	}

	string StreamTokenizer::toString()
	{
		string ret;
		switch (ttype)
		{
		  case TT_EOF:
			ret = "EOF";
			break;
		  case TT_EOL:
			ret = "EOL";
			break;
		  case TT_WORD:
			ret = sval;
			break;
		  case TT_NUMBER:
			ret = "n=" + StringHelper::toString(nval);
			break;
		  case TT_NOTHING:
			ret = "NOTHING";
			break;
		  default:
		  {
				/*
				 * ttype is the first character of either a quoted string or
				 * is an ordinary character. ttype can definitely not be less
				 * than 0, since those are reserved values used in the previous
				 * case statements
				 */
				if (ttype < 256 && ((ctype[ttype] & CT_QUOTE) != 0))
				{
					ret = sval;
					break;
				}

				std::vector<char> s(3);
				s[0] = s[2] = '\'';
				s[1] = static_cast<char>(ttype);

				ret = string(s.begin(), s.end());
				break;
		  }
		}
		return "Token[" + ret + "], line " + StringHelper::toString(LINENO);
	}

	void StreamTokenizer::InitializeInstanceFields()
	{
		input = NULL;
		buf = std::vector<char>(20);
		peekc = NEED_CHAR;
		pushedBack = false;
		forceLower = false;
		LINENO = 1;
		eolIsSignificantP = false;
		slashSlashCommentsP = false;
		slashStarCommentsP = false;
		ctype = std::vector<char>(256);
		ttype = TT_NOTHING;
		sval = "";
		nval = 0;
	}
}
