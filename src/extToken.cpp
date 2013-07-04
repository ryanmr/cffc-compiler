/* 

    ExtToken: an extension of the Token class with new methods for parsing 
   (led, nud, lbp) and describing the token.
   Author: EVW
*/

#include "extToken.h"
#include "parser.h"

#include <string>

using namespace std ;

ExtToken *extendToken (Parser *p, Token *tokens) {
    switch ( tokens->terminal ) {

    // Keywords
    case nameKwd: return new ExtToken(p,tokens,"'name'") ;
    case platformKwd: return new ExtToken(p,tokens,"'platform'") ;
    case initialKwd: return new ExtToken(p,tokens,"'initial'") ;
    case stateKwd: return new ExtToken(p,tokens,"'state'") ;

    case gotoKwd: return new ExtToken(p,tokens,"'goto'") ;
    case whenKwd: return new ExtToken(p,tokens,"'when'") ;
    case performingKwd: return new ExtToken(p,tokens,"'performing'") ;
    case exitKwd: return new ExtToken(p,tokens,"'exit'") ;

    case trueKwd: return new TrueKwdToken(p,tokens);
    case falseKwd: return new FalseKwdToken(p,tokens);

    case intKwd: return new ExtToken(p,tokens,"'int'") ;
    case floatKwd: return new ExtToken(p,tokens,"'float'") ;
    case stringKwd: return new ExtToken(p,tokens,"'string'") ;
    case charKwd: return new ExtToken(p,tokens,"'char'") ;

    /*
        Adding the line below FIXES boolean type support in Decls.
        However, it causes a mysterious error:
            "Expected 'name' but found int const" in parser_tests under bad_syntax_good_tokens.cff
        The cause of the error is that Parser::parse parses for a 'program' syntax,
        that file is definitely not in line with a file and thus it throws an exception when
        'nameKwd' does not appear in the proper location at the beginning of the file.

        Nevertheless, it's not clear why it decides to throw only when the case below is enabled.

        Update 2:
            After investigating, this hides the aforementioned error by having "boolean"
            be absent from the extended token list. Once it is, the Parser::parse interpets the file
            as a Program as it should, but as it is not, it complains.
    */
    case booleanKwd: return new ExtToken(p,tokens,"'boolean'") ;

    // Constants
    case intConst: return new IntConstToken(p,tokens) ;
    case floatConst: return new FloatConstToken(p,tokens) ;
    case stringConst: return new StringConstToken(p,tokens) ;
    case charConst: return new CharConstToken(p,tokens) ;

    // Names
    case variableName: return new VariableNameToken(p,tokens) ;

    // Punctuation
    case leftParen: return new LeftParenToken(p,tokens) ;
    case rightParen: return new ExtToken(p,tokens,")") ;
    case leftCurly: return new ExtToken(p,tokens,"{") ;
    case rightCurly: return new ExtToken(p,tokens,"}") ;

    case colon: return new ExtToken(p,tokens,":") ;
    case comma: return new ExtToken(p,tokens,",") ;
    case semiColon: return new ExtToken(p,tokens,";") ;
    case assign: return new ExtToken(p,tokens,":=") ;

    case plusSign: return new PlusSignToken(p,tokens) ;
    case star: return new StarToken(p,tokens) ;
    case dash: return new DashToken(p,tokens) ;
    case forwardSlash: return new ForwardSlashToken(p,tokens) ;

    case leftAngle:
    case rightAngle:
    case equalsEquals: 
    case lessThanEquals: 
    case greaterThanEquals: 
    case notEquals: 
        return new RelationalOpToken(p, tokens, tokens->lexeme) ;

    case lexicalError: return new ExtToken(p,tokens,"lexical error") ;
    case endOfFile: return new EndOfFileToken(p,tokens) ;

/*
    case endKwd: return new EndKwdToken(p,tokens) ;

    case ifKwd: return new IfKwdToken(p,tokens) ;
    case thenKwd: return new ThenKwdToken(p,tokens) ;
    case elseKwd: return new ElseKwdToken(p,tokens) ;

    case printKwd: return new PrintKwdToken(p,tokens) ;
    case writeKwd: return new WriteKwdToken(p,tokens) ;
    case readKwd: return new ReadKwdToken(p,tokens) ;

    case integerKwd: return new IntegerKwdToken(p,tokens) ;
    case floatKwd: return new FloatKwdToken(p,tokens) ;
    case booleanKwd: return new BooleanKwdToken(p,tokens) ;
    case stringKwd: return new StringKwdToken(p,tokens) ;

    case trueKwd: return new TrueKwdToken(p,tokens) ;
    case falseKwd: return new FalseKwdToken(p,tokens) ;

    case headKwd: return new HeadKwdToken(p,tokens) ;
    case tailKwd: return new TailKwdToken(p,tokens) ;
    case nullKwd: return new NullKwdToken(p,tokens) ;
    case mapKwd: return new MapKwdToken(p,tokens) ;
    case filterKwd: return new FilterKwdToken(p,tokens) ;
    case foldKwd: return new FoldKwdToken(p,tokens) ;
    case zipKwd: return new ZipKwdToken(p,tokens) ;

    case intConst: return new IntConstToken(p,tokens) ;
    case floatConst: return new FloatConstToken(p,tokens) ;
    case stringConst: return new StringConstToken(p,tokens) ;




    case colon: return new ColonToken(p,tokens) ;
    case colonColon: return new ColonColonToken(p,tokens) ;
    case comma: return new ExtToken(p,tokens,",") ;
    case semiColon: return new SemiColonToken(p,tokens) ;
    case equalsSign: return new EqualsSignToken(p,tokens) ;

    case plusSign: return new PlusSignToken(p,tokens) ;
    case dash: return new DashToken(p,tokens) ;
    case star: return new StarToken(p,tokens) ;

    case percent: return new PercentToken(p,tokens) ;

    case backSlash: return new BackSlashToken(p,tokens) ;
    case arrow: return new ExtToken(p,tokens,"->") ;


    case plusPlus: return new PlusPlusToken(p,tokens) ;
    case dotDot: return new DotDotToken(p,tokens) ;
    case tupleOp: return new TupleOpToken(p,tokens) ;
*/

    default: 
        string msg = (string) "Unspecified terminal in extend: tokens->lexeme = " + tokens->lexeme ;
        throw ( p->makeErrorMsg ( msg.c_str() ) ) ;
    }
}

ExtToken *extendTokenList (Parser *p, Token *tokens) {
    ExtToken *extTokens = NULL ;
    ExtToken *prevExtToken = NULL ;

    while (tokens != NULL) {
        ExtToken *currExtToken = extendToken(p, tokens) ;
 
        // Add new ExtToken et to the list
        if (extTokens != NULL) {
            prevExtToken->next = currExtToken ;
        }
        else {
            extTokens = currExtToken ;
        }
        prevExtToken = currExtToken ;

        tokens = tokens->next ;
    }

    return extTokens ;
}
