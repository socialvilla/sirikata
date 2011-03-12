/** \file
 *  This C header file was generated by $ANTLR version 3.1.3 Mar 17, 2009 19:23:44
 *
 *     -  From the grammar source file : .//LexWhenPred.g
 *     -                            On : 2011-03-12 11:53:42
 *     -                 for the lexer : LexWhenPredLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer LexWhenPredLexer has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pLexWhenPredLexer, which is returned from a call to LexWhenPredLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in pLexWhenPredLexer are  as follows:
 *
 *  -  void      pLexWhenPredLexer->T__35(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__36(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__37(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__38(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__39(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__40(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__41(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__42(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__43(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__44(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__45(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__46(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__47(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__48(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__49(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__50(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__51(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__52(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__53(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->T__54(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->StringLiteral(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->DoubleStringCharacter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->SingleStringCharacter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->EscapeSequence(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->CharacterEscapeSequence(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->NonEscapeCharacter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->SingleEscapeCharacter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->EscapeCharacter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->HexEscapeSequence(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->UnicodeEscapeSequence(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->NumericLiteral(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->HexIntegerLiteral(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->HexDigit(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->DecimalLiteral(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->DecimalDigit(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->ExponentPart(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->Identifier(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->IdentifierStart(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->IdentifierPart(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->UnicodeLetter(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->UnicodeCombiningMark(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->UnicodeDigit(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->UnicodeConnectorPunctuation(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->Comment(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->LineComment(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->LTERM(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->WhiteSpace(pLexWhenPredLexer)
 *  -  void      pLexWhenPredLexer->Tokens(pLexWhenPredLexer)
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_LexWhenPredLexer_H
#define _LexWhenPredLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct LexWhenPredLexer_Ctx_struct LexWhenPredLexer, * pLexWhenPredLexer;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

/** Context tracking structure for LexWhenPredLexer
 */
struct LexWhenPredLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mT__35)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__36)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__37)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__38)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__39)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__40)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__41)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__42)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__43)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__44)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__45)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__46)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__47)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__48)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__49)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__50)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__51)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__52)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__53)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mT__54)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mStringLiteral)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mDoubleStringCharacter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mSingleStringCharacter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mEscapeSequence)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mCharacterEscapeSequence)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mNonEscapeCharacter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mSingleEscapeCharacter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mEscapeCharacter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mHexEscapeSequence)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mUnicodeEscapeSequence)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mNumericLiteral)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mHexIntegerLiteral)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mHexDigit)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mDecimalLiteral)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mDecimalDigit)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mExponentPart)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mIdentifier)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mIdentifierStart)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mIdentifierPart)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mUnicodeLetter)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mUnicodeCombiningMark)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mUnicodeDigit)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mUnicodeConnectorPunctuation)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mComment)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mLineComment)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mLTERM)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mWhiteSpace)	(struct LexWhenPredLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct LexWhenPredLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct LexWhenPredLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pLexWhenPredLexer LexWhenPredLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API pLexWhenPredLexer LexWhenPredLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define HexEscapeSequence      16
#define LineComment      33
#define DecimalDigit      21
#define EOF      -1
#define HexDigit      22
#define Identifier      9
#define SingleStringCharacter      13
#define PROG      5
#define T__51      51
#define T__52      52
#define T__53      53
#define T__54      54
#define Comment      32
#define IDENTIFIER      8
#define SingleEscapeCharacter      18
#define ExponentPart      25
#define UnicodeLetter      28
#define EscapeCharacter      20
#define WhiteSpace      34
#define T__50      50
#define IdentifierPart      27
#define UnicodeCombiningMark      31
#define T__42      42
#define LTERM      4
#define T__43      43
#define UnicodeDigit      29
#define T__40      40
#define T__41      41
#define T__46      46
#define T__47      47
#define NumericLiteral      11
#define T__44      44
#define T__45      45
#define T__48      48
#define T__49      49
#define UnicodeEscapeSequence      17
#define WHEN_PRED_BLOCK      6
#define IdentifierStart      26
#define DoubleStringCharacter      12
#define DecimalLiteral      23
#define StringLiteral      10
#define T__35      35
#define T__36      36
#define T__37      37
#define DOTTED_IDENTIFIER      7
#define T__38      38
#define T__39      39
#define HexIntegerLiteral      24
#define NonEscapeCharacter      19
#define CharacterEscapeSequence      15
#define EscapeSequence      14
#define UnicodeConnectorPunctuation      30
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for LexWhenPredLexer
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
