#ifndef STREAMTOKENIZER
#define STREAMTOKENIZER

#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include "stringhelper.h"

/*
* Copyright (c) 1995, 2005, Oracle and/or its affiliates. All rights reserved.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* This code is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 only, as
* published by the Free Software Foundation.  Oracle designates this
* particular file as subject to the "Classpath" exception as provided
* by Oracle in the LICENSE file that accompanied this code.
*
* This code is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* version 2 for more details (a copy is included in the LICENSE file that
* accompanied this code).
*
* You should have received a copy of the GNU General Public License version
* 2 along with this work; if not, write to the Free Software Foundation,
* Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
*
* Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
* or visit www.oracle.com if you need additional information or have any
* questions.
*/

namespace iProlog
{


    /// <summary>
    /// The <code>StreamTokenizer</code> class takes an input stream and
    /// parses it into "tokens", allowing the tokens to be
    /// read one at a time. The parsing process is controlled by a table
    /// and a number of flags that can be set to various states. The
    /// stream tokenizer can recognize identifiers, numbers, quoted
    /// strings, and various comment styles.
    /// <para>
    /// Each byte read from the input stream is regarded as a character
    /// in the range <code>'&#92;u0000'</code> through <code>'&#92;u00FF'</code>.
    /// The character value is used to look up five possible attributes of
    /// the character: <i>white space</i>, <i>alphabetic</i>,
    /// <i>numeric</i>, <i>string quote</i>, and <i>comment character</i>.
    /// Each character can have zero or more of these attributes.
    /// </para>
    /// <para>
    /// In addition, an instance has four flags. These flags indicate:
    /// <ul>
    /// <li>Whether line terminators are to be returned as tokens or treated
    ///     as white space that merely separates tokens.
    /// <li>Whether C-style comments are to be recognized and skipped.
    /// <li>Whether C++-style comments are to be recognized and skipped.
    /// <li>Whether the characters of identifiers are converted to lowercase.
    /// </ul>
    /// </para>
    /// <para>
    /// A typical application first constructs an instance of this class,
    /// sets up the syntax tables, and then repeatedly loops calling the
    /// <code>nextToken</code> method in each iteration of the loop until
    /// it returns the value <code>TT_EOF</code>.
    /// 
    /// @author  James Gosling
    /// </para>
    /// </summary>
    /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
    /// <seealso cref=     java.io.StreamTokenizer#TT_EOF
    /// @since   JDK1.0 </seealso>

    class StreamTokenizer
    {

        /* Only one of these will be non-null */
    private:
        istream *input;

        std::vector<char> buf;

        /// <summary>
        /// The next character to be considered by the nextToken method.  May also
        /// be NEED_CHAR to indicate that a new character should be read, or SKIP_LF
        /// to indicate that a new character should be read and, if it is a '\n'
        /// character, it should be discarded and a second new character should be
        /// read.
        /// </summary>
        int peekc;

        static int NEED_CHAR;
        static int SKIP_LF;

        bool pushedBack;
        bool forceLower;
        /// <summary>
        /// The line number of the last token read </summary>
        int LINENO;

        bool eolIsSignificantP;
        bool slashSlashCommentsP;
        bool slashStarCommentsP;

        std::vector<char> ctype;
        static const char CT_WHITESPACE = 1;
        static const char CT_DIGIT = 2;
        static const char CT_ALPHA = 4;
        static const char CT_QUOTE = 8;
        static const char CT_COMMENT = 16;

        /// <summary>
        /// After a call to the <code>nextToken</code> method, this field
        /// contains the type of the token just read. For a single character
        /// token, its value is the single character, converted to an integer.
        /// For a quoted string token, its value is the quote character.
        /// Otherwise, its value is one of the following:
        /// <ul>
        /// <li><code>TT_WORD</code> indicates that the token is a word.
        /// <li><code>TT_NUMBER</code> indicates that the token is a number.
        /// <li><code>TT_EOL</code> indicates that the end of line has been read.
        ///     The field can only have this value if the
        ///     <code>eolIsSignificant</code> method has been called with the
        ///     argument <code>true</code>.
        /// <li><code>TT_EOF</code> indicates that the end of the input stream
        ///     has been reached.
        /// </ul>
        /// <para>
        /// The initial value of this field is -4.
        /// 
        /// </para>
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#eolIsSignificant(boolean) </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#quoteChar(int) </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_EOF </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_EOL </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_NUMBER </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_WORD </seealso>
    public:
        int ttype;

        /// <summary>
        /// A constant indicating that the end of the stream has been read.
        /// </summary>
        static const int TT_EOF = -1;

        /// <summary>
        /// A constant indicating that the end of the line has been read.
        /// </summary>
        static const int TT_EOL = '\n';

        /// <summary>
        /// A constant indicating that a number token has been read.
        /// </summary>
        static const int TT_NUMBER = -2;

        /// <summary>
        /// A constant indicating that a word token has been read.
        /// </summary>
        static const int TT_WORD = -3;

        /* A constant indicating that no token has been read, used for
        * initializing ttype.  FIXME This could be made public and
        * made available as the part of the API in a future release.
        */
    private:
        static const int TT_NOTHING = -4;

        /// <summary>
        /// If the current token is a word token, this field contains a
        /// string giving the characters of the word token. When the current
        /// token is a quoted string token, this field contains the body of
        /// the string.
        /// <para>
        /// The current token is a word when the value of the
        /// <code>ttype</code> field is <code>TT_WORD</code>. The current token is
        /// a quoted string token when the value of the <code>ttype</code> field is
        /// a quote character.
        /// </para>
        /// <para>
        /// The initial value of this field is null.
        /// 
        /// </para>
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#quoteChar(int) </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_WORD </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
    public:
        std::string sval;

        /// <summary>
        /// If the current token is a number, this field contains the value
        /// of that number. The current token is a number when the value of
        /// the <code>ttype</code> field is <code>TT_NUMBER</code>.
        /// <para>
        /// The initial value of this field is 0.0.
        /// 
        /// </para>
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#TT_NUMBER </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        double nval;

        /// <summary>
        /// Private constructor that initializes everything except the streams. </summary>
        virtual ~StreamTokenizer()
        {
            delete input;
        }

    private:

        /// <summary>
        /// Creates a stream tokenizer that parses the specified input
        /// stream. The stream tokenizer is initialized to the following
        /// default state:
        /// <ul>
        /// <li>All byte values <code>'A'</code> through <code>'Z'</code>,
        ///     <code>'a'</code> through <code>'z'</code>, and
        ///     <code>'&#92;u00A0'</code> through <code>'&#92;u00FF'</code> are
        ///     considered to be alphabetic.
        /// <li>All byte values <code>'&#92;u0000'</code> through
        ///     <code>'&#92;u0020'</code> are considered to be white space.
        /// <li><code>'/'</code> is a comment character.
        /// <li>Single quote <code>'&#92;''</code> and double quote <code>'"'</code>
        ///     are string quote characters.
        /// <li>Numbers are parsed.
        /// <li>Ends of lines are treated as white space, not as separate tokens.
        /// <li>C-style and C++-style comments are not recognized.
        /// </ul>
        /// </summary>
        /// @deprecated As of JDK version 1.1, the preferred way to tokenize an
        /// input stream is to convert it into a character stream, for example:
        /// <blockquote><pre>
        ///   Reader r = new BufferedReader(new InputStreamReader(is));
        ///   StreamTokenizer st = new StreamTokenizer(r);
        /// </pre></blockquote>
        /// 
        /// <param name="is">        an input stream. </param>
        /// <seealso cref=        java.io.BufferedReader </seealso>
        /// <seealso cref=        java.io.InputStreamReader </seealso>
        /// <seealso cref=        java.io.StreamTokenizer#StreamTokenizer(java.io.Reader) </seealso>
    public:
        /// <summary>
        /// Create a tokenizer that parses the given character stream.
        /// </summary>
        /// <param name="r">  a Reader object providing the input stream.
        /// @since   JDK1.1 </param>
        StreamTokenizer(istream *is);

        /// <summary>
        /// Resets this tokenizer's syntax table so that all characters are
        /// "ordinary." See the <code>ordinaryChar</code> method
        /// for more information on a character being ordinary.
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#ordinaryChar(int) </seealso>
        virtual void resetSyntax();

        /// <summary>
        /// Specifies that all characters <i>c</i> in the range
        /// <code>low&nbsp;&lt;=&nbsp;<i>c</i>&nbsp;&lt;=&nbsp;high</code>
        /// are word constituents. A word token consists of a word constituent
        /// followed by zero or more word constituents or number constituents.
        /// </summary>
        /// <param name="low">   the low end of the range. </param>
        /// <param name="hi">    the high end of the range. </param>
        virtual void wordChars(int low, int hi);

        /// <summary>
        /// Specifies that all characters <i>c</i> in the range
        /// <code>low&nbsp;&lt;=&nbsp;<i>c</i>&nbsp;&lt;=&nbsp;high</code>
        /// are white space characters. White space characters serve only to
        /// separate tokens in the input stream.
        /// 
        /// <para>Any other attribute settings for the characters in the specified
        /// range are cleared.
        /// 
        /// </para>
        /// </summary>
        /// <param name="low">   the low end of the range. </param>
        /// <param name="hi">    the high end of the range. </param>
        virtual void whitespaceChars(int low, int hi);

        /// <summary>
        /// Specifies that all characters <i>c</i> in the range
        /// <code>low&nbsp;&lt;=&nbsp;<i>c</i>&nbsp;&lt;=&nbsp;high</code>
        /// are "ordinary" in this tokenizer. See the
        /// <code>ordinaryChar</code> method for more information on a
        /// character being ordinary.
        /// </summary>
        /// <param name="low">   the low end of the range. </param>
        /// <param name="hi">    the high end of the range. </param>
        /// <seealso cref=     java.io.StreamTokenizer#ordinaryChar(int) </seealso>
        virtual void ordinaryChars(int low, int hi);

        /// <summary>
        /// Specifies that the character argument is "ordinary"
        /// in this tokenizer. It removes any special significance the
        /// character has as a comment character, word component, string
        /// delimiter, white space, or number character. When such a character
        /// is encountered by the parser, the parser treats it as a
        /// single-character token and sets <code>ttype</code> field to the
        /// character value.
        /// 
        /// <para>Making a line terminator character "ordinary" may interfere
        /// with the ability of a <code>StreamTokenizer</code> to count
        /// lines. The <code>lineno</code> method may no longer reflect
        /// the presence of such terminator characters in its line count.
        /// 
        /// </para>
        /// </summary>
        /// <param name="ch">   the character. </param>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        virtual void ordinaryChar(int ch);

        /// <summary>
        /// Specified that the character argument starts a single-line
        /// comment. All characters from the comment character to the end of
        /// the line are ignored by this stream tokenizer.
        /// 
        /// <para>Any other attribute settings for the specified character are cleared.
        /// 
        /// </para>
        /// </summary>
        /// <param name="ch">   the character. </param>
        virtual void commentChar(int ch);

        /// <summary>
        /// Specifies that matching pairs of this character delimit string
        /// constants in this tokenizer.
        /// <para>
        /// When the <code>nextToken</code> method encounters a string
        /// constant, the <code>ttype</code> field is set to the string
        /// delimiter and the <code>sval</code> field is set to the body of
        /// the string.
        /// </para>
        /// <para>
        /// If a string quote character is encountered, then a string is
        /// recognized, consisting of all characters after (but not including)
        /// the string quote character, up to (but not including) the next
        /// occurrence of that same string quote character, or a line
        /// terminator, or end of file. The usual escape sequences such as
        /// <code>"&#92;n"</code> and <code>"&#92;t"</code> are recognized and
        /// converted to single characters as the string is parsed.
        /// 
        /// </para>
        /// <para>Any other attribute settings for the specified character are cleared.
        /// 
        /// </para>
        /// </summary>
        /// <param name="ch">   the character. </param>
        /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#sval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        virtual void quoteChar(int ch);

        /// <summary>
        /// Specifies that numbers should be parsed by this tokenizer. The
        /// syntax table of this tokenizer is modified so that each of the twelve
        /// characters:
        /// <blockquote><pre>
        ///      0 1 2 3 4 5 6 7 8 9 . -
        /// </pre></blockquote>
        /// <para>
        /// has the "numeric" attribute.
        /// </para>
        /// <para>
        /// When the parser encounters a word token that has the format of a
        /// double precision floating-point number, it treats the token as a
        /// number rather than a word, by setting the <code>ttype</code>
        /// field to the value <code>TT_NUMBER</code> and putting the numeric
        /// value of the token into the <code>nval</code> field.
        /// 
        /// </para>
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#nval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_NUMBER </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        virtual void parseNumbers();

        /// <summary>
        /// Determines whether or not ends of line are treated as tokens.
        /// If the flag argument is true, this tokenizer treats end of lines
        /// as tokens; the <code>nextToken</code> method returns
        /// <code>TT_EOL</code> and also sets the <code>ttype</code> field to
        /// this value when an end of line is read.
        /// <para>
        /// A line is a sequence of characters ending with either a
        /// carriage-return character (<code>'&#92;r'</code>) or a newline
        /// character (<code>'&#92;n'</code>). In addition, a carriage-return
        /// character followed immediately by a newline character is treated
        /// as a single end-of-line token.
        /// </para>
        /// <para>
        /// If the <code>flag</code> is false, end-of-line characters are
        /// treated as white space and serve only to separate tokens.
        /// 
        /// </para>
        /// </summary>
        /// <param name="flag">   <code>true</code> indicates that end-of-line characters
        ///                 are separate tokens; <code>false</code> indicates that
        ///                 end-of-line characters are white space. </param>
        /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_EOL </seealso>
        virtual void eolIsSignificant(bool flag);

        /// <summary>
        /// Determines whether or not the tokenizer recognizes C-style comments.
        /// If the flag argument is <code>true</code>, this stream tokenizer
        /// recognizes C-style comments. All text between successive
        /// occurrences of <code>/*</code> and <code>*&#47;</code> are discarded.
        /// <para>
        /// If the flag argument is <code>false</code>, then C-style comments
        /// are not treated specially.
        /// 
        /// </para>
        /// </summary>
        /// <param name="flag">   <code>true</code> indicates to recognize and ignore
        ///                 C-style comments. </param>
        virtual void slashStarComments(bool flag);

        /// <summary>
        /// Determines whether or not the tokenizer recognizes C++-style comments.
        /// If the flag argument is <code>true</code>, this stream tokenizer
        /// recognizes C++-style comments. Any occurrence of two consecutive
        /// slash characters (<code>'/'</code>) is treated as the beginning of
        /// a comment that extends to the end of the line.
        /// <para>
        /// If the flag argument is <code>false</code>, then C++-style
        /// comments are not treated specially.
        /// 
        /// </para>
        /// </summary>
        /// <param name="flag">   <code>true</code> indicates to recognize and ignore
        ///                 C++-style comments. </param>
        virtual void slashSlashComments(bool flag);

        /// <summary>
        /// Determines whether or not word token are automatically lowercased.
        /// If the flag argument is <code>true</code>, then the value in the
        /// <code>sval</code> field is lowercased whenever a word token is
        /// returned (the <code>ttype</code> field has the
        /// value <code>TT_WORD</code> by the <code>nextToken</code> method
        /// of this tokenizer.
        /// <para>
        /// If the flag argument is <code>false</code>, then the
        /// <code>sval</code> field is not modified.
        /// 
        /// </para>
        /// </summary>
        /// <param name="fl">   <code>true</code> indicates that all word tokens should
        ///               be lowercased. </param>
        /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#TT_WORD </seealso>
        virtual void lowerCaseMode(bool fl);

        /// <summary>
        /// Read the next character </summary>
    private:
        int read();

        /// <summary>
        /// Parses the next token from the input stream of this tokenizer.
        /// The type of the next token is returned in the <code>ttype</code>
        /// field. Additional information about the token may be in the
        /// <code>nval</code> field or the <code>sval</code> field of this
        /// tokenizer.
        /// <para>
        /// Typical clients of this
        /// class first set up the syntax tables and then sit in a loop
        /// calling nextToken to parse successive tokens until TT_EOF
        /// is returned.
        /// 
        /// </para>
        /// </summary>
        /// <returns>     the value of the <code>ttype</code> field. </returns>
        /// <exception cref="IOException">  if an I/O error occurs. </exception>
        /// <seealso cref=        java.io.StreamTokenizer#nval </seealso>
        /// <seealso cref=        java.io.StreamTokenizer#sval </seealso>
        /// <seealso cref=        java.io.StreamTokenizer#ttype </seealso>
    public:
        virtual int nextToken();

        /// <summary>
        /// Causes the next call to the <code>nextToken</code> method of this
        /// tokenizer to return the current value in the <code>ttype</code>
        /// field, and not to modify the value in the <code>nval</code> or
        /// <code>sval</code> field.
        /// </summary>
        /// <seealso cref=     java.io.StreamTokenizer#nextToken() </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#nval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#sval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        virtual void pushBack();

        /// <summary>
        /// Return the current line number.
        /// </summary>
        /// <returns>  the current line number of this stream tokenizer. </returns>
        virtual int lineno();

        /// <summary>
        /// Returns the string representation of the current stream token and
        /// the line number it occurs on.
        /// 
        /// <para>The precise string returned is unspecified, although the following
        /// example can be considered typical:
        /// 
        /// <blockquote><pre>Token['a'], line 10</pre></blockquote>
        /// 
        /// </para>
        /// </summary>
        /// <returns>  a string representation of the token </returns>
        /// <seealso cref=     java.io.StreamTokenizer#nval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#sval </seealso>
        /// <seealso cref=     java.io.StreamTokenizer#ttype </seealso>
        virtual std::string toString();


    private:
        void InitializeInstanceFields();
    };

}


#endif	//#ifndef STREAMTOKENIZER
