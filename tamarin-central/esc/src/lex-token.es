/* -*- mode: java; tab-width: 4; insert-tabs-mode: nil; indent-tabs-mode: nil -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

use namespace intrinsic;

public namespace Token

{
    use default namespace Token;

    /* A token has a "kind" (the token type) and a "text" (the lexeme).

       For predefined tokens the kind is equal to its index in the
       token store.

       NOTE, the parser in-lines tokenKind and tokenText manually, and
       knows what a Tok looks like, so don't treat Tok as an abstraction.
    */
    class Tok
    {
        var kind;
        var text;

        function Tok(kind,text)
            : kind = kind
            , text = text
        {
        }
    }

    function tokenKind (tid: int) : int
        tokenStore[tid].kind;

    function tokenText ( tid : int ) : String
        tokenStore[tid].text;

    // Map from (kind, text) to index in tokenStore.  It is used to
    // dramatically speed up duplicate testing in makeInstance(), below.

    const tokenHash = 
        new Util::Hashtable(function(x) { return Util::hash_number(x.kind) ^ Util::hash_string(x.text) },
                            function(a,b) { return a.kind === b.kind && a.text === b.text },
                            false );
    
    // Map from token number to a Tok structure.
    // FIXME: Really "Vector.<Tok>".
    //
    // Metric: Self-compiling parse.es, about 1000 unique tokens are
    // created (beyond the 157 that initially populate the token store).

    const tokenStore = new Array;

    // Attribute table for tokens.  Used by the parser for token
    // classification.  Entries are bit vectors (some tokens have
    // multiple attributes).

    const CTXRESERVED = 1;       // It is a contextually reserved identifier
    const RESERVED = 2;          // It is a reserved identifier
    const LITERAL = 4;           // It is a token that yields a literal
    const LEXEME = 8;            // It is a token that has a useful lexeme
    const EQUALITY = 16;         // It is ==, !=, ===, !==
    const RELATIONAL = 32;       // It is <, <=, >, >=, In, InstanceOf, Is, Cast
    const RELTYPE = 64;          // Relational operator with typeExpr rhs
    const SHIFTOP = 128;         // It is <<, >>, or >>>
    const ADDITIVEOP = 256;      // It is + or -
    const MULTIPLICATIVEOP = 512;// It is *, /, or %
    const OPASSIGN = 1024;       // += and so on

    const tokenAttr = new Array;

    function isCtxReserved(token)
        (tokenAttr[token] & CTXRESERVED) != 0;

    function isReserved(token)
        (tokenAttr[token] & RESERVED) != 0;

    function hasLexeme(token)
        (tokenAttr[token] & LEXEME) != 0;

    function isLiteral(token)
        (tokenAttr[token] & LITERAL) != 0;

    function isEquality(token)
        (tokenAttr[token] & EQUALITY) != 0;

    function isRelational(token, allowIn)
        (tokenAttr[token] & RELATIONAL) != 0 && (allowIn || token != In);

    function isRelationalType(token)
        (tokenAttr[token] & RELTYPE) != 0;

    function isShift(token)
        (tokenAttr[token] & SHIFTOP) != 0;

    function isAdditive(token)
        (tokenAttr[token] & ADDITIVEOP) != 0;

    function isMultiplicative(token)
        (tokenAttr[token] & MULTIPLICATIVEOP) != 0;

    function isOpAssign(token)
        (tokenAttr[token] & OPASSIGN) != 0;

    // hash_helper is used to avoid allocation on the fast path inside
    // makeInstance.  It may not make much difference, but there you
    // have it.

    var hash_helper = { kind: 0, text: "" };

    function makeInstance(kind:int, text:String) : int {
        use namespace Util;

        hash_helper.kind = kind;
        hash_helper.text = text;
        var tid = tokenHash.read(hash_helper);
        if (tid === false) {
            tid = tokenStore.length;
            tokenStore.push(new Tok(kind, text));
            tokenHash.write({kind: kind, text: text}, tid);
        }
        return tid;
    }

    function setup(id, name, attr=0) {
        tokenStore[id] = new Tok(id,name);
        tokenAttr[id] = attr;
    }

    // punctuation and operators

    const Minus = 0;                                        setup(Token::Minus,"minus", ADDITIVEOP);
    const MinusMinus = Token::Minus + 1;                    setup(Token::MinusMinus,"minusminus");
    const Not = Token::MinusMinus + 1;                      setup(Token::Not,"not");
    const NotEqual = Token::Not + 1;                        setup(Token::NotEqual,"notequals", EQUALITY);
    const StrictNotEqual = Token::NotEqual + 1;             setup(Token::StrictNotEqual,"strictnotequals", EQUALITY);
    const Remainder = Token::StrictNotEqual + 1;            setup(Token::Remainder,"modulus", MULTIPLICATIVEOP);
    const RemainderAssign = Token::Remainder + 1;           setup(Token::RemainderAssign,"modulusassign", OPASSIGN);
    const BitwiseAnd = Token::RemainderAssign + 1;          setup(Token::BitwiseAnd,"bitwiseand");
    const LogicalAnd = Token::BitwiseAnd + 1;               setup(Token::LogicalAnd,"logicaland");
    const LogicalAndAssign = Token::LogicalAnd + 1;         setup(Token::LogicalAndAssign,"logicalandassign", OPASSIGN);
    const BitwiseAndAssign = Token::LogicalAndAssign + 1;   setup(Token::BitwiseAndAssign,"bitwiseandassign", OPASSIGN);
    const LeftParen = Token::BitwiseAndAssign + 1;          setup(Token::LeftParen,"leftparen");
    const RightParen = Token::LeftParen + 1;                setup(Token::RightParen,"rightparen");
    const Mult = Token::RightParen + 1;                     setup(Token::Mult,"mult", MULTIPLICATIVEOP);
    const MultAssign = Token::Mult + 1;                     setup(Token::MultAssign,"multassign", OPASSIGN);
    const Comma = Token::MultAssign  + 1;                   setup(Token::Comma,"comma");
    const Dot = Token::Comma + 1;                           setup(Token::Dot,"dot");
    const DoubleDot = Token::Dot + 1;                       setup(Token::DoubleDot,"doubledot");
    const TripleDot = Token::DoubleDot + 1;                 setup(Token::TripleDot,"tripledot");
    const LeftDotAngle = Token::TripleDot + 1;              setup(Token::LeftDotAngle,"leftdotangle");
    const Div = Token::LeftDotAngle + 1;                    setup(Token::Div,"div", MULTIPLICATIVEOP);
    const DivAssign = Token::Div + 1;                       setup(Token::DivAssign,"divassign", OPASSIGN);
    const Colon = Token::DivAssign + 1;                     setup(Token::Colon,"colon");
    const DoubleColon = Token::Colon + 1;                   setup(Token::DoubleColon,"doublecolon");
    const SemiColon = Token::DoubleColon + 1;               setup(Token::SemiColon,"semicolon");
    const QuestionMark = Token::SemiColon + 1;              setup(Token::QuestionMark,"questionmark");
    const At = Token::QuestionMark + 1;                     setup(Token::At,"at");
    const LeftBracket = Token::At + 1;                      setup(Token::LeftBracket,"leftbracket");
    const RightBracket = Token::LeftBracket + 1;            setup(Token::RightBracket,"rightbracket");
    const LogicalXor = Token::RightBracket + 1;             setup(Token::LogicalXor,"logicalxor");
    const LogicalXorAssign = Token::LogicalXor + 1;         setup(Token::LogicalXorAssign,"logicalxorassign", OPASSIGN);
    const LeftBrace = Token::LogicalXorAssign + 1;          setup(Token::LeftBrace,"leftbrace");
    const LogicalOr = Token::LeftBrace + 1;                 setup(Token::LogicalOr,"logicalor");
    const LogicalOrAssign = Token::LogicalOr + 1;           setup(Token::LogicalOrAssign,"logicalorassign", OPASSIGN);
    const BitwiseOr = Token::LogicalOrAssign + 1;           setup(Token::BitwiseOr,"bitwiseor");
    const BitwiseOrAssign = Token::BitwiseOr + 1;           setup(Token::BitwiseOrAssign,"bitwiseorassign", OPASSIGN);
    const BitwiseXor = Token::BitwiseOrAssign + 1;          setup(Token::BitwiseXor,"bitwisexor");
    const BitwiseXorAssign = Token::BitwiseXor + 1;         setup(Token::BitwiseXorAssign,"bitwisexorassign", OPASSIGN);
    const RightBrace = Token::BitwiseXorAssign + 1;         setup(Token::RightBrace,"rightbrace");
    const BitwiseNot = Token::RightBrace + 1;               setup(Token::BitwiseNot,"bitwisenot");
    const Plus = Token::BitwiseNot + 1;                     setup(Token::Plus,"plus", ADDITIVEOP);
    const PlusPlus = Token::Plus + 1;                       setup(Token::PlusPlus,"plusplus");
    const PlusAssign = Token::PlusPlus + 1;                 setup(Token::PlusAssign,"plusassign", OPASSIGN);
    const LessThan = Token::PlusAssign + 1;                 setup(Token::LessThan,"lessthan", RELATIONAL);
    const LeftShift = Token::LessThan + 1;                  setup(Token::LeftShift,"leftshift", SHIFTOP);
    const LeftShiftAssign = Token::LeftShift + 1;           setup(Token::LeftShiftAssign,"leftshiftassign", OPASSIGN);
    const LessThanOrEqual = Token::LeftShiftAssign + 1;     setup(Token::LessThanOrEqual,"lessthanorequals", RELATIONAL);
    const Assign = Token::LessThanOrEqual + 1;              setup(Token::Assign,"assign");
    const MinusAssign = Token::Assign + 1;                  setup(Token::MinusAssign,"minusassign", OPASSIGN);
    const Equal = Token::MinusAssign + 1;                   setup(Token::Equal,"equals", EQUALITY);
    const StrictEqual = Token::Equal + 1;                   setup(Token::StrictEqual,"strictequals", EQUALITY);
    const GreaterThan = Token::StrictEqual + 1;             setup(Token::GreaterThan,"greaterthan", RELATIONAL);
    const GreaterThanOrEqual = Token::GreaterThan + 1;      setup(Token::GreaterThanOrEqual,"greaterthanorequals", RELATIONAL);
    const RightShift = Token::GreaterThanOrEqual + 1;       setup(Token::RightShift,"rightshift", SHIFTOP);
    const RightShiftAssign = Token::RightShift + 1;         setup(Token::RightShiftAssign,"rightshiftassign", OPASSIGN);
    const UnsignedRightShift = Token::RightShiftAssign + 1; setup(Token::UnsignedRightShift,"unsignedrightshift", SHIFTOP);
    const UnsignedRightShiftAssign = Token::UnsignedRightShift + 1;   setup(Token::UnsignedRightShiftAssign,"unsignedrightshiftassign", OPASSIGN);

    // reserved identifiers

    const Break = Token::UnsignedRightShiftAssign + 1;      setup(Token::Break,"break", RESERVED);
    const Case = Token::Break + 1;                          setup(Token::Case,"case", RESERVED);
    const Catch = Token::Case + 1;                          setup(Token::Catch,"catch", RESERVED);
    const Class = Token::Catch + 1;                         setup(Token::Class,"class", RESERVED);
    const Continue = Token::Class + 1;                      setup(Token::Continue,"continue", RESERVED);
    const Default = Token::Continue + 1;                    setup(Token::Default,"default", RESERVED);
    const Delete = Token::Default + 1;                      setup(Token::Delete,"delete", RESERVED);
    const Do = Token::Delete + 1;                           setup(Token::Do,"do", RESERVED);
    const Else = Token::Do + 1;                             setup(Token::Else,"else", RESERVED);
    const Enum = Token::Else + 1;                           setup(Token::Enum,"enum", RESERVED);
    const Extends = Token::Enum + 1;                        setup(Token::Extends,"extends", RESERVED);
    const False = Token::Extends + 1;                       setup(Token::False,"false", RESERVED|LITERAL);
    const Finally = Token::False + 1;                       setup(Token::Finally,"finally", RESERVED);
    const For = Token::Finally + 1;                         setup(Token::For,"for", RESERVED);
    const Function = Token::For + 1;                        setup(Token::Function,"function", RESERVED);
    const If = Token::Function + 1;                         setup(Token::If,"if", RESERVED);
    const In = Token::If + 1;                               setup(Token::In,"in", RESERVED|RELATIONAL);
    const InstanceOf = Token::In + 1;                       setup(Token::InstanceOf,"instanceof", RESERVED|RELATIONAL);
    const New = Token::InstanceOf + 1;                      setup(Token::New,"new", RESERVED);
    const Null = Token::New + 1;                            setup(Token::Null,"null", RESERVED|LITERAL);
    const Return = Token::Null + 1;                         setup(Token::Return,"return", RESERVED);
    const Super = Token::Return + 1;                        setup(Token::Super,"super", RESERVED);
    const Switch = Token::Super + 1;                        setup(Token::Switch,"switch", RESERVED);
    const This = Token::Switch + 1;                         setup(Token::This,"this", RESERVED);
    const Throw = Token::This + 1;                          setup(Token::Throw,"throw", RESERVED);
    const True = Token::Throw + 1;                          setup(Token::True,"true", RESERVED|LITERAL);
    const Try = Token::True + 1;                            setup(Token::Try,"try", RESERVED);
    const TypeOf = Token::Try + 1;                          setup(Token::TypeOf,"typeof", RESERVED);
    const Var = Token::TypeOf + 1;                          setup(Token::Var,"var", RESERVED);
    const Void = Token::Var + 1;                            setup(Token::Void,"void", RESERVED);
    const While = Token::Void + 1;                          setup(Token::While,"while", RESERVED);
    const With = Token::While + 1;                          setup(Token::With,"with", RESERVED);

    // contextually reserved identifiers

    const Call = Token::With + 1;                           setup(Token::Call,"call", CTXRESERVED);
    const Cast = Token::Call + 1;                           setup(Token::Cast,"cast", CTXRESERVED|RELATIONAL|RELTYPE);
    const Const = Token::Cast + 1;                          setup(Token::Const,"const", CTXRESERVED);
    const Decimal = Token::Const + 1;                       setup(Token::Decimal,"decimal", CTXRESERVED);
    const Double = Token::Decimal + 1;                      setup(Token::Double,"double", CTXRESERVED);
    const Dynamic = Token::Double + 1;                      setup(Token::Dynamic,"dynamic", CTXRESERVED);
    const Each = Token::Dynamic + 1;                        setup(Token::Each,"each", CTXRESERVED);
    const Eval = Token::Each + 1;                           setup(Token::Eval,"eval", CTXRESERVED);
    const Final = Token::Eval + 1;                          setup(Token::Final,"final", CTXRESERVED);
    const Get = Token::Final + 1;                           setup(Token::Get,"get", CTXRESERVED);
    const Has = Token::Get + 1;                             setup(Token::Has,"has", CTXRESERVED);
    const Implements = Token::Has + 1;                      setup(Token::Implements,"implements", CTXRESERVED);
    const Import = Token::Implements + 1;                   setup(Token::Import,"import", CTXRESERVED);
    const Int = Token::Import + 1;                          setup(Token::Int,"int", CTXRESERVED);
    const Interface = Token::Int + 1;                       setup(Token::Interface,"interface", CTXRESERVED);
    const Internal = Token::Interface + 1;                  setup(Token::Internal,"internal", CTXRESERVED);
    const Intrinsic = Token::Internal + 1;                  setup(Token::Intrinsic,"intrinsic", CTXRESERVED);
    const Is = Token::Intrinsic + 1;                        setup(Token::Is,"is", CTXRESERVED|RELATIONAL|RELTYPE);
    const Let = Token::Is + 1;                              setup(Token::Let,"let", CTXRESERVED);
    const Namespace = Token::Let + 1;                       setup(Token::Namespace,"namespace", CTXRESERVED);
    const Native = Token::Namespace + 1;                    setup(Token::Native,"native", CTXRESERVED);
    const Number = Token::Native + 1;                       setup(Token::Number,"Number", CTXRESERVED);
    const Override = Token::Number + 1;                     setup(Token::Override,"override", CTXRESERVED);
    const Package = Token::Override + 1;                    setup(Token::Package,"package", CTXRESERVED);
    const Precision = Token::Package + 1;                   setup(Token::Precision,"precision", CTXRESERVED);
    const Private = Token::Precision + 1;                   setup(Token::Private,"private", CTXRESERVED);
    const Protected = Token::Private + 1;                   setup(Token::Protected,"protected", CTXRESERVED);
    const Prototype = Token::Protected + 1;                 setup(Token::Prototype,"prototype", CTXRESERVED);
    const Public = Token::Prototype + 1;                    setup(Token::Public,"public", CTXRESERVED);
    const Rounding = Token::Public + 1;                     setup(Token::Rounding,"rounding", CTXRESERVED);
    const Standard = Token::Rounding + 1;                   setup(Token::Standard,"standard", CTXRESERVED);
    const Strict = Token::Standard + 1;                     setup(Token::Strict,"strict", CTXRESERVED);
    const To = Token::Strict + 1;                           setup(Token::To,"to", CTXRESERVED|RELATIONAL|RELTYPE);
    const Set = Token::To + 1;                              setup(Token::Set,"set", CTXRESERVED);
    const Static = Token::Set + 1;                          setup(Token::Static,"static", CTXRESERVED);
    const Type = Token::Static + 1;                         setup(Token::Type,"type", CTXRESERVED);
    const UInt = Token::Type + 1;                           setup(Token::UInt,"uint", CTXRESERVED);
    const Undefined = Token::UInt + 1;                      setup(Token::Undefined,"undefined", CTXRESERVED|LITERAL);
    const Unit = Token::Undefined + 1;                      setup(Token::Unit,"unit", CTXRESERVED);
    const Use = Token::Unit + 1;                            setup(Token::Use,"use", CTXRESERVED);
    const Xml = Token::Use + 1;                             setup(Token::Xml,"xml", CTXRESERVED);
    const Yield = Token::Xml + 1;                           setup(Token::Yield,"yield", CTXRESERVED);

    // sundry

    const AttributeIdentifier = Token::Yield + 1;           setup(Token::AttributeIdentifier,"attributeidentifier", LEXEME);
    const Identifier = Token::AttributeIdentifier + 1;      setup(Token::Identifier,"identifier", LEXEME);
    const IntLiteral = Token::Identifier + 1;               setup(Token::IntLiteral,"intliteral", LEXEME|LITERAL);
    const UIntLiteral = Token::IntLiteral + 1;              setup(Token::UIntLiteral,"uintliteral", LEXEME|LITERAL);
    const DoubleLiteral = Token::UIntLiteral + 1;           setup(Token::DoubleLiteral,"doubleliteral", LEXEME|LITERAL);
    const DecimalLiteral = Token::DoubleLiteral + 1;        setup(Token::DecimalLiteral,"decimalliteral", LEXEME|LITERAL);
    const RegexpLiteral = Token::DecimalLiteral + 1;        setup(Token::RegexpLiteral,"regexpliteral", LEXEME|LITERAL);
    const StringLiteral = Token::RegexpLiteral + 1;         setup(Token::StringLiteral,"stringliteral", LEXEME|LITERAL);

    const XmlLiteral = Token::StringLiteral + 1;            setup(Token::XmlLiteral,"xmlliteral");
    const XmlPart = Token::XmlLiteral + 1;                  setup(Token::XmlPart,"xmlpart");
    const XmlMarkup = Token::XmlPart + 1;                   setup(Token::XmlMarkup,"xmlmarkup");
    const XmlText = Token::XmlMarkup + 1;                   setup(Token::XmlText,"xmltext");
    const XmlTagEndEnd = Token::XmlText + 1;                setup(Token::XmlTagEndEnd,"xmltagendend");
    const XmlTagStartEnd = Token::XmlTagEndEnd + 1;         setup(Token::XmlTagStartEnd,"xmltagstartend");


    // meta

    const EOS = Token::XmlTagStartEnd + 1;                  setup(Token::EOS,"EOS");
    const BREAK = Token::EOS + 1;                           setup(Token::BREAK,"BREAK");
    const NONE = Token::BREAK + 1;                          setup(Token::NONE,"NONE");
}

