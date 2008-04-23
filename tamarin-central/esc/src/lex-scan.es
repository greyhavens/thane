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

public namespace Lex

{
    use default namespace Lex;

    class Scanner
    {
        var src : String;
        var filename : String;
        var curIndex : int;
        var markIndex : int;
        var lnCoord : int;
        var keyword_or_ident: Boolean;
        const notPartOfIdent = [];

        function syntaxError(msg) {
            Util::syntaxError(filename, lnCoord, msg);
        }

        function internalError(msg) {
            Util::internalError(filename, lnCoord, msg);
        }

        /* public */
        function Scanner (src:String, filename:String, keyword_or_ident:Boolean = false)
            : src = src
            , filename = filename
            , curIndex = 0
            , markIndex = 0
            , lnCoord = 1
            , keyword_or_ident = keyword_or_ident
        {
            // The array notPartOfIdent maps ASCII and Unicode space
            // characters that are not part of an identifier to true.
            // It is used by the scanner to determine whether it has
            // seen a keyword.

            // FIXME: Put this data in a common static, since we may
            // create a number of lexers to re-scan identifiers
            // containing escape sequences and it would be nice not to
            // have to recreate the table every time.
            //
            // ESC doesn't have class initializers yet, so fix later.

            for ( let i=0 ; i < 128 ; i++ ) {
                let c = String.fromCharCode(i);
                if ( c >= "a" && c <= "z" ||
                     c >= "A" && c <= "Z" ||
                     c >= "0" && c <= "9" ||
                     c == "_" || c == "$" ||
                     c == "\\")
                    continue;
                notPartOfIdent[i] = true;
            }
            let unicode_spaces = [0x00A0, 0x1680, 0x180E, 0x2000, 0x2001, 0x2002, 0x2003,
                                  0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 0x200A,
                                  0x202F, 0x205F, 0x3000,
                                  0x2028, 0x2029];
            for ( let i=0 ; i < unicode_spaces.length ; i++ )
                notPartOfIdent[unicode_spaces[i]] = true;
            notPartOfIdent[NaN] = true;
        }

        /* public */
        function regexp () {
            // Capture the initial '/'
            markIndex = curIndex - 1;

            // Body
            // Avoid switches here, the verifier gets confused (verifier bug).
            while (true) {
                let c = src.charCodeAt(curIndex++) | 0;
                if (c == 47 /* Char::Slash */)
                    break;
                else if (c == 92 /* Char::Backslash */) {
                    let d = src.charCodeAt(curIndex++) | 0;
                    if (d == 10 /* Char::Newline */ ||
                        d == 13 /* Char::CarriageReturn */ ||
                        d == 0x2028 /* Char::UnicodeLS */ ||
                        d == 0x2029 /* Char::UnicodePS */)
                        Lex::syntaxError("Illegal newline in regexp literal");
                    else if (d == 0 /* Char::Nul */) {
                        if (curIndex == src.length)
                            Lex::syntaxError("Unexpected end of program in regexp literal");
                    }
                }
                else if (c == 10 /* Char::Newline */ ||
                         c == 13 /* Char::CarriageReturn */ ||
                         c == 0x2028 /* Char::LS */ ||
                         c == 0x2029 /* Char::PS */) {
                    Lex::syntaxError("Illegal newline in regexp literal");
                }
                else if (c == 0 /* Char::Nul */) {
                    if (curIndex == src.length)
                        Lex::syntaxError("Unexpected end of program in regexp literal");
                }
            }

            // Flags
            while (Char::isUnicodeIdentifierPart(src.charCodeAt(curIndex) | 0))
                ++curIndex;

            return Token::makeInstance (Token::RegexpLiteral,lexeme());
        }

        /* public */
        function div () : int {
            switch (src.charCodeAt(curIndex)) {
            case  61 /* Char::Equal */ : 
                curIndex++;
                return Token::DivAssign;
            case  62 /* Char::RightAngle */: 
                curIndex++;
                return Token::XmlTagEndEnd;
            default :
                return Token::Div;
            }
        }

        function lexeme()
            src.slice (markIndex,curIndex);

        /* Get the next token.

           Never returns Token::Eol.

           lnCoord will be the line number of the
           ending character of the token.  This probably only matters
           for strings, and it would be good in that case to have the
           starting position too.

           How end-of-input is detected without checking explicitly:
           We hop along the string reading the next char c.  Reading a
           character past the end results in NaN.  c is converted to
           int, eg c|0.  Now any NaN will be converted to 0.  Ergo we
           check for end-of-input every time we see 0, which is
           allowed in the input (string literals, regexp literals,
           comments) but tends to be very rare in practice.

           Note that subscanners that consume input until something
           happens (like strings and comments) will need to perform
           the conversion to int and check for 0 as well.

           (For most practical purposes it's like there's a 0 sitting
           at the end of the string.  When we get ESC ready for
           production use we would replace the charCodeAt call by a
           call on an inlinable primitive method that returns 0 if it
           reads beyond the end of the string.  Then we do not pay for
           int-to-double conversion, double returns, and double-to-int
           conversion like now.)
        */

        /* public */
        function start () : int {
            while (true) {
            bigswitch:
                switch (src.charCodeAt(curIndex++) | 0) {
                case   0 /* Char::Nul */:
                    if (curIndex >= src.length) {
                        curIndex = src.length;
                        return Token::EOS;
                    }
                    Lex::syntaxError("Illegal character in input: NUL");

                case  10 /* Char::Newline */:
                    lnCoord++;
                    continue;

                case  13 /* Char::CarriageReturn */: 
                    if (src.charCodeAt(curIndex) == 10 /* Char::Newline */)
                        curIndex++;
                    lnCoord++;
                    continue;

                case  32 /* Char::Space */: 
                    continue;

                case   9 /* Char::Tab */: 
                    continue;

                case  11 /* Char::VerticalTab */: 
                    continue;

                case  12 /* Char::Formfeed */: 
                    continue;

                case  160 /* Char::NoBreakSpace */: 
                    continue;

                case  40 /* Char::LeftParen */: 
                    return Token::LeftParen;

                case  41 /* Char::RightParen */: 
                    return Token::RightParen;

                case  44 /* Char::Comma */: 
                    return Token::Comma;

                case  59 /* Char::Semicolon */: 
                    return Token::SemiColon;

                case  63 /* Char::QuestionMark */: 
                    return Token::QuestionMark;

                case  91 /* Char::LeftBracket */: 
                    return Token::LeftBracket;

                case  93 /* Char::RightBracket */: 
                    return Token::RightBracket;

                case 123 /* Char::LeftBrace */: 
                    return Token::LeftBrace;

                case 125 /* Char::RightBrace */: 
                    return Token::RightBrace;

                case 126 /* Char::Tilde */: 
                    return Token::BitwiseNot;

                case  64 /* Char::At */: 
                    return Token::At;

                case  47 /* Char::Slash */: 
                    switch (src.charCodeAt(curIndex)) {
                    case 47 /* Char::Slash */:
                        curIndex++;
                        lineComment ();
                        continue;

                    case 42 /* Char::Asterisk */:
                        curIndex++;
                        blockComment();
                        continue;

                    default:
                        markIndex = curIndex-1;
                        return Token::BREAK;
                    }

                case  39 /* Char::SingleQuote */: 
                    return stringLiteral (39);

                case  34 /* Char::DoubleQuote */: 
                    return stringLiteral (34);

                case  46 /* Char::Dot */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  46 /* Char::Dot */:
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 46 /* Char::Dot */) {
                            curIndex++;
                            return Token::TripleDot;
                        }
                        return Token::DoubleDot;

                    case  60 /* Char::LeftAngle */: 
                        curIndex++;
                        return Token::LeftDotAngle;

                    case  48 /* Char::Zero */:
                    case  49 /* Char::One */:
                    case  50 /* Char::Two */:
                    case  51 /* Char::Three */:
                    case  52 /* Char::Four */:
                    case  53 /* Char::Five */:
                    case  54 /* Char::Six */:
                    case  55 /* Char::Seven */:
                    case  56 /* Char::Eight */:
                    case  57 /* Char::Nine */:
                        markIndex = --curIndex;
                        return numberLiteral ();

                    default :
                        return Token::Dot;
                    }

                case  45 /* Char::Dash */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  45 /* Char::Dash */ : 
                        curIndex++;
                        return Token::MinusMinus;

                    case  61 /* Char::Equal */: 
                        curIndex++;
                        return Token::MinusAssign;

                    default :
                        return Token::Minus;
                    }

                case  33 /* Char::Bang */: 
                    if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                            curIndex++;
                            return Token::StrictNotEqual;
                        }
                        return Token::NotEqual;
                    }
                    return Token::Not;

                case  37 /* Char::Percent */: 
                    if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                        curIndex++;
                        return Token::RemainderAssign;
                    }
                    return Token::Remainder;

                case  38 /* Char::Ampersand */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  61 /* Char::Equal */:
                        curIndex++;
                        return Token::BitwiseAndAssign;

                    case  38 /* Char::Ampersand */:
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                            curIndex++;
                            return Token::LogicalAndAssign;
                        }
                        return Token::LogicalAnd;

                    default:
                        return Token::BitwiseAnd;
                    }

                case  42 /* Char::Asterisk */: 
                    if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                        curIndex++;
                        return Token::MultAssign;
                    }
                    return Token::Mult;

                case  58 /* Char::Colon */: 
                    if (src.charCodeAt(curIndex) == 58 /* Char::Colon */) {
                        curIndex++;
                        return Token::DoubleColon;
                    }
                    return Token::Colon;

                case  94 /* Char::Caret */: 
                    if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                        curIndex++;
                        return Token::BitwiseXorAssign;
                    }
                    return Token::BitwiseXor;

                case 124 /* Char::Bar */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  61 /* Char::Equal */:
                        curIndex++;
                        return Token::BitwiseOrAssign;
                    case 124 /* Char::Bar */:
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                            curIndex++;
                            return Token::LogicalOrAssign;
                        }
                        return Token::LogicalOr;
                    default:
                        return Token::BitwiseOr;
                    }

                case  43 /* Char::Plus */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  43 /* Char::Plus */:
                        curIndex++;
                        return Token::PlusPlus;
                    case  61 /* Char::Equal */:
                        curIndex++;
                        return Token::PlusAssign;
                    default:
                        return Token::Plus;
                    }

                case  60 /* Char::LeftAngle */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  61 /* Char::Equal */:
                        curIndex++;
                        return Token::LessThanOrEqual;
                    case  60 /* Char::LeftAngle */:
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                            curIndex++;
                            return Token::LeftShiftAssign;
                        }
                        return Token::LeftShift;
                    case  47 /* Char::Slash */:
                        curIndex++;
                        return Token::XmlTagStartEnd;
                    default:
                        return Token::LessThan;
                    }
                    return leftAngle ();

                case  61 /* Char::Equal */: 
                    if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                        curIndex++;
                        if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                            curIndex++;
                            return Token::StrictEqual;
                        }
                        return Token::Equal;
                    }
                    return Token::Assign;

                case  62 /* Char::RightAngle */: 
                    switch (src.charCodeAt(curIndex)) {
                    case  61 /* Char::Equal */:
                        curIndex++;
                        return Token::GreaterThanOrEqual;
                    case  62 /* Char::RightAngle */:
                        curIndex++;
                        switch (src.charCodeAt(curIndex)) {
                        case  61 /* Char::Equal */:
                            curIndex++;
                            return Token::RightShiftAssign;
                        case  62 /* Char::RightAngle */:
                            curIndex++;
                            if (src.charCodeAt(curIndex) == 61 /* Char::Equal */) {
                                curIndex++;
                                return Token::UnsignedRightShiftAssign;
                            }
                            return Token::UnsignedRightShift;
                        default:
                            return Token::RightShift;
                        }
                    default:
                        return Token::GreaterThan;
                    }

                // Begin generated code

                case 98: /* Char::b */
                    if (src.charCodeAt(curIndex+0) == 114 /* Char::r */ &&
                        src.charCodeAt(curIndex+1) == 101 /* Char::e */ &&
                        src.charCodeAt(curIndex+2) == 97 /* Char::a */ &&
                        src.charCodeAt(curIndex+3) == 107 /* Char::k */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                        curIndex += 4;
                        return Token::Break;
                    }
                    break bigswitch;
                case 99: /* Char::c */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 108: /* Char::l */
                            if (src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                                curIndex += 3;
                                return Token::Call;
                            }
                            break bigswitch;
                        case 115: /* Char::s */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 101: /* Char::e */
                                if (!(notPartOfIdent[src.charCodeAt(curIndex+3)])) 
                                    break bigswitch;
                                curIndex += 3;
                                return Token::Case;
                            case 116: /* Char::t */
                                if (!(notPartOfIdent[src.charCodeAt(curIndex+3)])) 
                                    break bigswitch;
                                curIndex += 3;
                                return Token::Cast;
                            default:
                                break bigswitch;
                            }
                        case 116: /* Char::t */
                            if (src.charCodeAt(curIndex+2) == 99 /* Char::c */ &&
                                src.charCodeAt(curIndex+3) == 104 /* Char::h */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                                curIndex += 4;
                                return Token::Catch;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 108: /* Char::l */
                        if (src.charCodeAt(curIndex+1) == 97 /* Char::a */ &&
                            src.charCodeAt(curIndex+2) == 115 /* Char::s */ &&
                            src.charCodeAt(curIndex+3) == 115 /* Char::s */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                            curIndex += 4;
                            return Token::Class;
                        }
                        break bigswitch;
                    case 111: /* Char::o */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 110: /* Char::n */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 115: /* Char::s */
                                if (src.charCodeAt(curIndex+3) == 116 /* Char::t */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                                    curIndex += 4;
                                    return Token::Const;
                                }
                                break bigswitch;
                            case 116: /* Char::t */
                                if (src.charCodeAt(curIndex+3) == 105 /* Char::i */ &&
                                    src.charCodeAt(curIndex+4) == 110 /* Char::n */ &&
                                    src.charCodeAt(curIndex+5) == 117 /* Char::u */ &&
                                    src.charCodeAt(curIndex+6) == 101 /* Char::e */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                                    curIndex += 7;
                                    return Token::Continue;
                                }
                                break bigswitch;
                            default:
                                break bigswitch;
                            }
                        default:
                            break bigswitch;
                        }
                    default:
                        break bigswitch;
                    }
                case 100: /* Char::d */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 101: /* Char::e */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 99: /* Char::c */
                            if (src.charCodeAt(curIndex+2) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+3) == 109 /* Char::m */ &&
                                src.charCodeAt(curIndex+4) == 97 /* Char::a */ &&
                                src.charCodeAt(curIndex+5) == 108 /* Char::l */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                                curIndex += 6;
                                return Token::Decimal;
                            }
                            break bigswitch;
                        case 102: /* Char::f */
                            if (src.charCodeAt(curIndex+2) == 97 /* Char::a */ &&
                                src.charCodeAt(curIndex+3) == 117 /* Char::u */ &&
                                src.charCodeAt(curIndex+4) == 108 /* Char::l */ &&
                                src.charCodeAt(curIndex+5) == 116 /* Char::t */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                                curIndex += 6;
                                return Token::Default;
                            }
                            break bigswitch;
                        case 108: /* Char::l */
                            if (src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                                src.charCodeAt(curIndex+3) == 116 /* Char::t */ &&
                                src.charCodeAt(curIndex+4) == 101 /* Char::e */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                curIndex += 5;
                                return Token::Delete;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 111: /* Char::o */
                        if (src.charCodeAt(curIndex+1) == 117 /* Char::u */ &&
                            src.charCodeAt(curIndex+2) == 98 /* Char::b */ &&
                            src.charCodeAt(curIndex+3) == 108 /* Char::l */ &&
                            src.charCodeAt(curIndex+4) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                            curIndex += 5;
                            return Token::Double;
                        }
                        if (!(notPartOfIdent[src.charCodeAt(curIndex+1)])) 
                            break bigswitch;
                        curIndex += 1;
                        return Token::Do;
                    case 121: /* Char::y */
                        if (src.charCodeAt(curIndex+1) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+2) == 97 /* Char::a */ &&
                            src.charCodeAt(curIndex+3) == 109 /* Char::m */ &&
                            src.charCodeAt(curIndex+4) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+5) == 99 /* Char::c */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                            curIndex += 6;
                            return Token::Dynamic;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 101: /* Char::e */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        if (src.charCodeAt(curIndex+1) == 99 /* Char::c */ &&
                            src.charCodeAt(curIndex+2) == 104 /* Char::h */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Each;
                        }
                        break bigswitch;
                    case 108: /* Char::l */
                        if (src.charCodeAt(curIndex+1) == 115 /* Char::s */ &&
                            src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Else;
                        }
                        break bigswitch;
                    case 110: /* Char::n */
                        if (src.charCodeAt(curIndex+1) == 117 /* Char::u */ &&
                            src.charCodeAt(curIndex+2) == 109 /* Char::m */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Enum;
                        }
                        break bigswitch;
                    case 118: /* Char::v */
                        if (src.charCodeAt(curIndex+1) == 97 /* Char::a */ &&
                            src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Eval;
                        }
                        break bigswitch;
                    case 120: /* Char::x */
                        if (src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                            src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                            src.charCodeAt(curIndex+3) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+4) == 100 /* Char::d */ &&
                            src.charCodeAt(curIndex+5) == 115 /* Char::s */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                            curIndex += 6;
                            return Token::Extends;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 102: /* Char::f */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        if (src.charCodeAt(curIndex+1) == 108 /* Char::l */ &&
                            src.charCodeAt(curIndex+2) == 115 /* Char::s */ &&
                            src.charCodeAt(curIndex+3) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                            curIndex += 4;
                            return Token::False;
                        }
                        break bigswitch;
                    case 105: /* Char::i */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 110: /* Char::n */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 97: /* Char::a */
                                switch(src.charCodeAt(curIndex+3)) {
                                case 108: /* Char::l */
                                    if (src.charCodeAt(curIndex+4) == 108 /* Char::l */ &&
                                        src.charCodeAt(curIndex+5) == 121 /* Char::y */ &&
                                        notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                                        curIndex += 6;
                                        return Token::Finally;
                                    }
                                    if (!(notPartOfIdent[src.charCodeAt(curIndex+4)])) 
                                        break bigswitch;
                                    curIndex += 4;
                                    return Token::Final;
                                default:
                                    break bigswitch;
                                }
                            default:
                                break bigswitch;
                            }
                        default:
                            break bigswitch;
                        }
                    case 111: /* Char::o */
                        if (src.charCodeAt(curIndex+1) == 114 /* Char::r */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                            curIndex += 2;
                            return Token::For;
                        }
                        break bigswitch;
                    case 117: /* Char::u */
                        if (src.charCodeAt(curIndex+1) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+2) == 99 /* Char::c */ &&
                            src.charCodeAt(curIndex+3) == 116 /* Char::t */ &&
                            src.charCodeAt(curIndex+4) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+5) == 111 /* Char::o */ &&
                            src.charCodeAt(curIndex+6) == 110 /* Char::n */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                            curIndex += 7;
                            return Token::Function;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 103: /* Char::g */
                    if (src.charCodeAt(curIndex+0) == 101 /* Char::e */ &&
                        src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                        curIndex += 2;
                        return Token::Get;
                    }
                    break bigswitch;
                case 104: /* Char::h */
                    if (src.charCodeAt(curIndex+0) == 97 /* Char::a */ &&
                        src.charCodeAt(curIndex+1) == 115 /* Char::s */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                        curIndex += 2;
                        return Token::Has;
                    }
                    break bigswitch;
                case 105: /* Char::i */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 102: /* Char::f */
                        if (!(notPartOfIdent[src.charCodeAt(curIndex+1)])) 
                            break bigswitch;
                        curIndex += 1;
                        return Token::If;
                    case 109: /* Char::m */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 112: /* Char::p */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 108: /* Char::l */
                                if (src.charCodeAt(curIndex+3) == 101 /* Char::e */ &&
                                    src.charCodeAt(curIndex+4) == 109 /* Char::m */ &&
                                    src.charCodeAt(curIndex+5) == 101 /* Char::e */ &&
                                    src.charCodeAt(curIndex+6) == 110 /* Char::n */ &&
                                    src.charCodeAt(curIndex+7) == 116 /* Char::t */ &&
                                    src.charCodeAt(curIndex+8) == 115 /* Char::s */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+9)]) {
                                    curIndex += 9;
                                    return Token::Implements;
                                }
                                break bigswitch;
                            case 111: /* Char::o */
                                if (src.charCodeAt(curIndex+3) == 114 /* Char::r */ &&
                                    src.charCodeAt(curIndex+4) == 116 /* Char::t */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                    curIndex += 5;
                                    return Token::Import;
                                }
                                break bigswitch;
                            default:
                                break bigswitch;
                            }
                        default:
                            break bigswitch;
                        }
                    case 110: /* Char::n */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 115: /* Char::s */
                            if (src.charCodeAt(curIndex+2) == 116 /* Char::t */ &&
                                src.charCodeAt(curIndex+3) == 97 /* Char::a */ &&
                                src.charCodeAt(curIndex+4) == 110 /* Char::n */ &&
                                src.charCodeAt(curIndex+5) == 99 /* Char::c */ &&
                                src.charCodeAt(curIndex+6) == 101 /* Char::e */ &&
                                src.charCodeAt(curIndex+7) == 111 /* Char::o */ &&
                                src.charCodeAt(curIndex+8) == 102 /* Char::f */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+9)]) {
                                curIndex += 9;
                                return Token::InstanceOf;
                            }
                            break bigswitch;
                        case 116: /* Char::t */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 101: /* Char::e */
                                switch(src.charCodeAt(curIndex+3)) {
                                case 114: /* Char::r */
                                    switch(src.charCodeAt(curIndex+4)) {
                                    case 102: /* Char::f */
                                        if (src.charCodeAt(curIndex+5) == 97 /* Char::a */ &&
                                            src.charCodeAt(curIndex+6) == 99 /* Char::c */ &&
                                            src.charCodeAt(curIndex+7) == 101 /* Char::e */ &&
                                            notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                            curIndex += 8;
                                            return Token::Interface;
                                        }
                                        break bigswitch;
                                    case 110: /* Char::n */
                                        if (src.charCodeAt(curIndex+5) == 97 /* Char::a */ &&
                                            src.charCodeAt(curIndex+6) == 108 /* Char::l */ &&
                                            notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                                            curIndex += 7;
                                            return Token::Internal;
                                        }
                                        break bigswitch;
                                    default:
                                        break bigswitch;
                                    }
                                default:
                                    break bigswitch;
                                }
                            case 114: /* Char::r */
                                if (src.charCodeAt(curIndex+3) == 105 /* Char::i */ &&
                                    src.charCodeAt(curIndex+4) == 110 /* Char::n */ &&
                                    src.charCodeAt(curIndex+5) == 115 /* Char::s */ &&
                                    src.charCodeAt(curIndex+6) == 105 /* Char::i */ &&
                                    src.charCodeAt(curIndex+7) == 99 /* Char::c */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                    curIndex += 8;
                                    return Token::Intrinsic;
                                }
                                break bigswitch;
                            default:
                                if (!(notPartOfIdent[src.charCodeAt(curIndex+2)])) 
                                    break bigswitch;
                                curIndex += 2;
                                return Token::Int;
                            }
                        default:
                            if (!(notPartOfIdent[src.charCodeAt(curIndex+1)])) 
                                break bigswitch;
                            curIndex += 1;
                            return Token::In;
                        }
                    case 115: /* Char::s */
                        if (!(notPartOfIdent[src.charCodeAt(curIndex+1)])) 
                            break bigswitch;
                        curIndex += 1;
                        return Token::Is;
                    default:
                        break bigswitch;
                    }
                case 108: /* Char::l */
                    if (src.charCodeAt(curIndex+0) == 101 /* Char::e */ &&
                        src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                        curIndex += 2;
                        return Token::Let;
                    }
                    break bigswitch;
                case 110: /* Char::n */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 109: /* Char::m */
                            if (src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                                src.charCodeAt(curIndex+3) == 115 /* Char::s */ &&
                                src.charCodeAt(curIndex+4) == 112 /* Char::p */ &&
                                src.charCodeAt(curIndex+5) == 97 /* Char::a */ &&
                                src.charCodeAt(curIndex+6) == 99 /* Char::c */ &&
                                src.charCodeAt(curIndex+7) == 101 /* Char::e */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                curIndex += 8;
                                return Token::Namespace;
                            }
                            break bigswitch;
                        case 116: /* Char::t */
                            if (src.charCodeAt(curIndex+2) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+3) == 118 /* Char::v */ &&
                                src.charCodeAt(curIndex+4) == 101 /* Char::e */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                curIndex += 5;
                                return Token::Native;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 101: /* Char::e */
                        if (src.charCodeAt(curIndex+1) == 119 /* Char::w */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                            curIndex += 2;
                            return Token::New;
                        }
                        break bigswitch;
                    case 117: /* Char::u */
                        if (src.charCodeAt(curIndex+1) == 108 /* Char::l */ &&
                            src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Null;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 111: /* Char::o */
                    if (src.charCodeAt(curIndex+0) == 118 /* Char::v */ &&
                        src.charCodeAt(curIndex+1) == 101 /* Char::e */ &&
                        src.charCodeAt(curIndex+2) == 114 /* Char::r */ &&
                        src.charCodeAt(curIndex+3) == 114 /* Char::r */ &&
                        src.charCodeAt(curIndex+4) == 105 /* Char::i */ &&
                        src.charCodeAt(curIndex+5) == 100 /* Char::d */ &&
                        src.charCodeAt(curIndex+6) == 101 /* Char::e */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                        curIndex += 7;
                        return Token::Override;
                    }
                    break bigswitch;
                case 112: /* Char::p */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        if (src.charCodeAt(curIndex+1) == 99 /* Char::c */ &&
                            src.charCodeAt(curIndex+2) == 107 /* Char::k */ &&
                            src.charCodeAt(curIndex+3) == 97 /* Char::a */ &&
                            src.charCodeAt(curIndex+4) == 103 /* Char::g */ &&
                            src.charCodeAt(curIndex+5) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                            curIndex += 6;
                            return Token::Package;
                        }
                        break bigswitch;
                    case 114: /* Char::r */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 101: /* Char::e */
                            if (src.charCodeAt(curIndex+2) == 99 /* Char::c */ &&
                                src.charCodeAt(curIndex+3) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+4) == 115 /* Char::s */ &&
                                src.charCodeAt(curIndex+5) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+6) == 111 /* Char::o */ &&
                                src.charCodeAt(curIndex+7) == 110 /* Char::n */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                curIndex += 8;
                                return Token::Precision;
                            }
                            break bigswitch;
                        case 105: /* Char::i */
                            if (src.charCodeAt(curIndex+2) == 118 /* Char::v */ &&
                                src.charCodeAt(curIndex+3) == 97 /* Char::a */ &&
                                src.charCodeAt(curIndex+4) == 116 /* Char::t */ &&
                                src.charCodeAt(curIndex+5) == 101 /* Char::e */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+6)]) {
                                curIndex += 6;
                                return Token::Private;
                            }
                            break bigswitch;
                        case 111: /* Char::o */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 116: /* Char::t */
                                switch(src.charCodeAt(curIndex+3)) {
                                case 101: /* Char::e */
                                    if (src.charCodeAt(curIndex+4) == 99 /* Char::c */ &&
                                        src.charCodeAt(curIndex+5) == 116 /* Char::t */ &&
                                        src.charCodeAt(curIndex+6) == 101 /* Char::e */ &&
                                        src.charCodeAt(curIndex+7) == 100 /* Char::d */ &&
                                        notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                        curIndex += 8;
                                        return Token::Protected;
                                    }
                                    break bigswitch;
                                case 111: /* Char::o */
                                    if (src.charCodeAt(curIndex+4) == 116 /* Char::t */ &&
                                        src.charCodeAt(curIndex+5) == 121 /* Char::y */ &&
                                        src.charCodeAt(curIndex+6) == 112 /* Char::p */ &&
                                        src.charCodeAt(curIndex+7) == 101 /* Char::e */ &&
                                        notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                        curIndex += 8;
                                        return Token::Prototype;
                                    }
                                    break bigswitch;
                                default:
                                    break bigswitch;
                                }
                            default:
                                break bigswitch;
                            }
                        default:
                            break bigswitch;
                        }
                    case 117: /* Char::u */
                        if (src.charCodeAt(curIndex+1) == 98 /* Char::b */ &&
                            src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                            src.charCodeAt(curIndex+3) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+4) == 99 /* Char::c */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                            curIndex += 5;
                            return Token::Public;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 114: /* Char::r */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 101: /* Char::e */
                        if (src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                            src.charCodeAt(curIndex+2) == 117 /* Char::u */ &&
                            src.charCodeAt(curIndex+3) == 114 /* Char::r */ &&
                            src.charCodeAt(curIndex+4) == 110 /* Char::n */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                            curIndex += 5;
                            return Token::Return;
                        }
                        break bigswitch;
                    case 111: /* Char::o */
                        if (src.charCodeAt(curIndex+1) == 117 /* Char::u */ &&
                            src.charCodeAt(curIndex+2) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+3) == 100 /* Char::d */ &&
                            src.charCodeAt(curIndex+4) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+5) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+6) == 103 /* Char::g */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                            curIndex += 7;
                            return Token::Rounding;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 115: /* Char::s */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 101: /* Char::e */
                        if (src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                            curIndex += 2;
                            return Token::Set;
                        }
                        break bigswitch;
                    case 116: /* Char::t */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 97: /* Char::a */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 110: /* Char::n */
                                if (src.charCodeAt(curIndex+3) == 100 /* Char::d */ &&
                                    src.charCodeAt(curIndex+4) == 97 /* Char::a */ &&
                                    src.charCodeAt(curIndex+5) == 114 /* Char::r */ &&
                                    src.charCodeAt(curIndex+6) == 100 /* Char::d */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+7)]) {
                                    curIndex += 7;
                                    return Token::Standard;
                                }
                                break bigswitch;
                            case 116: /* Char::t */
                                if (src.charCodeAt(curIndex+3) == 105 /* Char::i */ &&
                                    src.charCodeAt(curIndex+4) == 99 /* Char::c */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                    curIndex += 5;
                                    return Token::Static;
                                }
                                break bigswitch;
                            default:
                                break bigswitch;
                            }
                        case 114: /* Char::r */
                            if (src.charCodeAt(curIndex+2) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+3) == 99 /* Char::c */ &&
                                src.charCodeAt(curIndex+4) == 116 /* Char::t */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                curIndex += 5;
                                return Token::Strict;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 117: /* Char::u */
                        if (src.charCodeAt(curIndex+1) == 112 /* Char::p */ &&
                            src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                            src.charCodeAt(curIndex+3) == 114 /* Char::r */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                            curIndex += 4;
                            return Token::Super;
                        }
                        break bigswitch;
                    case 119: /* Char::w */
                        if (src.charCodeAt(curIndex+1) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+2) == 116 /* Char::t */ &&
                            src.charCodeAt(curIndex+3) == 99 /* Char::c */ &&
                            src.charCodeAt(curIndex+4) == 104 /* Char::h */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                            curIndex += 5;
                            return Token::Switch;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 116: /* Char::t */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 104: /* Char::h */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 105: /* Char::i */
                            if (src.charCodeAt(curIndex+2) == 115 /* Char::s */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                                curIndex += 3;
                                return Token::This;
                            }
                            break bigswitch;
                        case 114: /* Char::r */
                            if (src.charCodeAt(curIndex+2) == 111 /* Char::o */ &&
                                src.charCodeAt(curIndex+3) == 119 /* Char::w */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                                curIndex += 4;
                                return Token::Throw;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 111: /* Char::o */
                        if (!(notPartOfIdent[src.charCodeAt(curIndex+1)])) 
                            break bigswitch;
                        curIndex += 1;
                        return Token::To;
                    case 114: /* Char::r */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 117: /* Char::u */
                            if (src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                                curIndex += 3;
                                return Token::True;
                            }
                            break bigswitch;
                        case 121: /* Char::y */
                            if (!(notPartOfIdent[src.charCodeAt(curIndex+2)])) 
                                break bigswitch;
                            curIndex += 2;
                            return Token::Try;
                        default:
                            break bigswitch;
                        }
                    case 121: /* Char::y */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 112: /* Char::p */
                            switch(src.charCodeAt(curIndex+2)) {
                            case 101: /* Char::e */
                                if (src.charCodeAt(curIndex+3) == 111 /* Char::o */ &&
                                    src.charCodeAt(curIndex+4) == 102 /* Char::f */ &&
                                    notPartOfIdent[src.charCodeAt(curIndex+5)]) {
                                    curIndex += 5;
                                    return Token::TypeOf;
                                }
                                if (!(notPartOfIdent[src.charCodeAt(curIndex+3)])) 
                                    break bigswitch;
                                curIndex += 3;
                                return Token::Type;
                            default:
                                break bigswitch;
                            }
                        default:
                            break bigswitch;
                        }
                    default:
                        break bigswitch;
                    }
                case 117: /* Char::u */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 105: /* Char::i */
                        if (src.charCodeAt(curIndex+1) == 110 /* Char::n */ &&
                            src.charCodeAt(curIndex+2) == 116 /* Char::t */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::UInt;
                        }
                        break bigswitch;
                    case 110: /* Char::n */
                        switch(src.charCodeAt(curIndex+1)) {
                        case 100: /* Char::d */
                            if (src.charCodeAt(curIndex+2) == 101 /* Char::e */ &&
                                src.charCodeAt(curIndex+3) == 102 /* Char::f */ &&
                                src.charCodeAt(curIndex+4) == 105 /* Char::i */ &&
                                src.charCodeAt(curIndex+5) == 110 /* Char::n */ &&
                                src.charCodeAt(curIndex+6) == 101 /* Char::e */ &&
                                src.charCodeAt(curIndex+7) == 100 /* Char::d */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+8)]) {
                                curIndex += 8;
                                return Token::Undefined;
                            }
                            break bigswitch;
                        case 105: /* Char::i */
                            if (src.charCodeAt(curIndex+2) == 116 /* Char::t */ &&
                                notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                                curIndex += 3;
                                return Token::Unit;
                            }
                            break bigswitch;
                        default:
                            break bigswitch;
                        }
                    case 115: /* Char::s */
                        if (src.charCodeAt(curIndex+1) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                            curIndex += 2;
                            return Token::Use;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 118: /* Char::v */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 97: /* Char::a */
                        if (src.charCodeAt(curIndex+1) == 114 /* Char::r */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                            curIndex += 2;
                            return Token::Var;
                        }
                        break bigswitch;
                    case 111: /* Char::o */
                        if (src.charCodeAt(curIndex+1) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+2) == 100 /* Char::d */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::Void;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 119: /* Char::w */
                    switch(src.charCodeAt(curIndex+0)) {
                    case 104: /* Char::h */
                        if (src.charCodeAt(curIndex+1) == 105 /* Char::i */ &&
                            src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                            src.charCodeAt(curIndex+3) == 101 /* Char::e */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                            curIndex += 4;
                            return Token::While;
                        }
                        break bigswitch;
                    case 105: /* Char::i */
                        if (src.charCodeAt(curIndex+1) == 116 /* Char::t */ &&
                            src.charCodeAt(curIndex+2) == 104 /* Char::h */ &&
                            notPartOfIdent[src.charCodeAt(curIndex+3)]) {
                            curIndex += 3;
                            return Token::With;
                        }
                        break bigswitch;
                    default:
                        break bigswitch;
                    }
                case 120: /* Char::x */
                    if (src.charCodeAt(curIndex+0) == 109 /* Char::m */ &&
                        src.charCodeAt(curIndex+1) == 108 /* Char::l */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+2)]) {
                        curIndex += 2;
                        return Token::Xml;
                    }
                    break bigswitch;
                case 121: /* Char::y */
                    if (src.charCodeAt(curIndex+0) == 105 /* Char::i */ &&
                        src.charCodeAt(curIndex+1) == 101 /* Char::e */ &&
                        src.charCodeAt(curIndex+2) == 108 /* Char::l */ &&
                        src.charCodeAt(curIndex+3) == 100 /* Char::d */ &&
                        notPartOfIdent[src.charCodeAt(curIndex+4)]) {
                        curIndex += 4;
                        return Token::Yield;
                    }
                    break bigswitch;

                // End generated code

                case  92 /* Char::BackSlash*/:
                    // In ES3, the only way backslash can appear in the input in
                    // this position is as the first character of an identifier,
                    // represented as a character escape.  So break out.
                    break bigswitch;

                case  48 /* Char::Zero */: 
                case  49 /* Char::One */:
                case  50 /* Char::Two */:
                case  51 /* Char::Three */:
                case  52 /* Char::Four */:
                case  53 /* Char::Five */:
                case  54 /* Char::Six */:
                case  55 /* Char::Seven */:
                case  56 /* Char::Eight */:
                case  57 /* Char::Nine */:
                    markIndex = --curIndex;
                    return numberLiteral ();

                default: {
                    // Check for Unicode space characters and line
                    // terminators here in order to avoid inhibiting
                    // the mickey-mouse switch optimization in ESC.
                    // (If the optimizations were any good they would
                    // perform this transformation automatically.)

                    let c = src.charCodeAt(curIndex-1);

                    // Quick check that saves us from the tedious
                    // testing most of the time, also another hack to
                    // get around poor switch code generation.

                    if (c >= 0x1680) {
                        switch (c) {
                        case 0x1680: /* Various Zs characters */
                        case 0x180e:
                        case 0x2000:
                        case 0x2001:
                        case 0x2002:
                        case 0x2003:
                        case 0x2004:
                        case 0x2005:
                        case 0x2006:
                        case 0x2007:
                        case 0x2008:
                        case 0x2009:
                        case 0x200a:
                        case 0x202f:
                        case 0x205f:
                        case 0x3000:
                            continue;
                        case 0x2028: /* Char::LS */
                        case 0x2029: /* Char::PS */
                            lnCoord++;
                            continue;
                        }
                    }

                    break bigswitch;
                }
                }

                // end of bigswitch.

                // Identifiers are handled here (and only here).
                //
                // It is never necessary to check whether an
                // identifier not containing an escape squeuence is a
                // keyword.  In cases where the input is "breakX"
                // where X is a non-identifier character then break
                // will in fact be returned first because of how the
                // notPartOfIdentifier logic works, but since the X is
                // not an identifier constituent there will be an
                // immediate syntax error.  That's good enough for me.

                // If the scanner is used to re-check whether an identifier
                // containing a backslash sequence looks like a keyword then
                // we can stop here.

                if (keyword_or_ident)
                    return Token::Identifier;

                --curIndex;
                return identifier();
            }
        }

        // All number literals start here without having consumed any
        // input.
        //
        // It's a minor weakness of the current token representation
        // that the value it carries can only be a string.  If not, we
        // would create number values right here and would avoid
        // re-converting the strings in the parser when the AST nodes
        // are created.

        function numberLiteral() : int {
            switch (src.charCodeAt(curIndex)) {
            case  48 /* Char::0 */: 
                // Octal / hex / a single 0 / 0.<something> /0e<something>
                switch (src.charCodeAt(curIndex+1)) {
                case 120 /* Char::x */:
                case  88 /* Char::X */:
                    curIndex += 2;
                    markIndex = curIndex;
                    if (!hexDigits(-1))
                        Lex::syntaxError("Illegal hexadecimal literal: no digits");
                    return makeIntegerLiteral( parseInt(lexeme(), 16) );

                case  46 /* Char::Dot */:
                    curIndex += 2;
                    numberFraction(true);
                    return makeFloatingLiteral( lexeme() );
                        
                case 69 /* Char::E */: 
                case 101 /* Char::e */:
                    curIndex += 2;
                    numberExponent();
                    return makeFloatingLiteral( lexeme() );

                default:
                    // Octal or single '0'
                    if (src.charCodeAt(curIndex+1) === 109 /* Char::m */) {
                        curIndex++; // Consume the '0'
                        return makeFloatingLiteral( lexeme() );
                    }

                    let startIndex = curIndex;
                    octalDigits(-1);
                    switch (src.charCodeAt(curIndex) | 0) {
                    case 56:
                    case 57:
                        // Ignore the leading 0 and parse it as decimal.
                        // Firefox extension; clear error in E262-3.
                        curIndex = startIndex;
                        break;
                    default:
                        return makeIntegerLiteral( parseInt(lexeme(), 8) );
                    }
                }
                break;

            case  46 /* Char::Dot */:
                curIndex++;
                numberFraction(false);
                return makeFloatingLiteral( lexeme() );

            default:
                break;
            }

            if (numberLiteralPrime() || src.charCodeAt(curIndex) === 109 /* Char::m */)
                return makeFloatingLiteral( lexeme() );
            else
                return makeIntegerLiteral( parseInt(lexeme(), 10) );
        }

        function makeIntegerLiteral( n ) {
            checkNextCharForNumber();

            if (n >= -0x80000000 && n <= 0x7FFFFFFF)
                return Token::makeInstance( Token::IntLiteral, String(n) );
            if (n >= 0x80000000 && n <= 0xFFFFFFFF)
                return Token::makeInstance( Token::UIntLiteral, String(n) );
            return Token::makeInstance( Token::DoubleLiteral, String(n) );
        }

        function makeFloatingLiteral( s ) {
            if (src.charCodeAt(curIndex) === 109 /* Char::m */) {
                curIndex++;

                checkNextCharForNumber();
                return Token::makeInstance( Token::DecimalLiteral, s );
            }
            else {
                checkNextCharForNumber();
                return Token::makeInstance( Token::DoubleLiteral, s );
            }
        }

        function checkNextCharForNumber() {
            let c = src.charCodeAt(curIndex);
            if (c >= 48 /* Char::Zero */ && c <= 57 /* Char::Nine */ ||
                Char::isUnicodeIdentifierStart(c))
                Lex::syntaxError("Illegal character following numeric literal: " + String.fromCharCode(c));
        }

        function numberLiteralPrime() {
            if (!decimalDigits(-1))
                Lex::syntaxError("Illegal number: no digits");
            
            switch (src.charCodeAt(curIndex)) {
            case  46 /* Char::Dot */:
                curIndex++;
                numberFraction (true);
                return true;

            case 101 /* Char::e */: 
            case  69 /* Char::E */:
                curIndex++;
                numberExponent ();
                return true;

            default:
                return false;
            }
        }

        // The '.' has been consumed.
        //
        // has_leading_digits should be true if digits have been seen
        // before the '.'.

        function numberFraction(has_leading_digits) {
            if (!decimalDigits (-1) && !has_leading_digits)
                Lex::syntaxError("Illegal number: must have digits before or after decimal point");

            switch (src.charCodeAt(curIndex)) {
            case 101 /* Char::e */: 
            case  69 /* Char::E */:
                curIndex++;
                numberExponent ();
                break;
            }
        }

        // The 'e' has been consumed...

        function numberExponent () {
            switch (src.charCodeAt(curIndex)) {
            case  43 /* Char::Plus */:
            case  45 /* Char::Minus */:
                curIndex++;
                break;
            }
            if (!decimalDigits(-1))
                Lex::syntaxError("Illegal number: missing digits in exponent");
        }

        function octalDigits(k): boolean {
            let startIndex = curIndex;
            loop:
            while (k != 0) {
                switch (src.charCodeAt(curIndex)) {
                case  48 /* Char::Zero */:
                case  49 /* Char::One */:
                case  50 /* Char::Two */:
                case  51 /* Char::Three */:
                case  52 /* Char::Four */:
                case  53 /* Char::Five */:
                case  54 /* Char::Six */:
                case  55 /* Char::Seven */:
                    k--;
                    curIndex++;
                    continue;

                default:
                    break loop;
                }
            }
            return curIndex > startIndex && k <= 0;
        }

        function decimalDigits(k): boolean {
            let startIndex = curIndex;
            loop:
            while (k != 0) {
                switch (src.charCodeAt(curIndex)) {
                case  48 /* Char::Zero */:
                case  49 /* Char::One */:
                case  50 /* Char::Two */:
                case  51 /* Char::Three */:
                case  52 /* Char::Four */:
                case  53 /* Char::Five */:
                case  54 /* Char::Six */:
                case  55 /* Char::Seven */:
                case  56 /* Char::Eight */:
                case  57 /* Char::Nine */:
                    k--;
                    curIndex++;
                    continue;

                default:
                    break loop;
                }
            }
            return curIndex > startIndex && k <= 0;
        }

        function hexDigits(k): boolean {
            let startIndex = curIndex;
            loop:
            while (k != 0) {
                switch (src.charCodeAt(curIndex)) {
                case  48 /* Char::Zero */:
                case  49 /* Char::One */:
                case  50 /* Char::Two */:
                case  51 /* Char::Three */:
                case  52 /* Char::Four */:
                case  53 /* Char::Five */:
                case  54 /* Char::Six */:
                case  55 /* Char::Seven */:
                case  56 /* Char::Eight */:
                case  57 /* Char::Nine */:
                case  97 /* Char::a */: case  65 /* Char::A */:
                case  98 /* Char::b */: case  66 /* Char::B */:
                case  99 /* Char::c */: case  67 /* Char::C */:
                case 100 /* Char::d */: case  68 /* Char::D */:
                case 101 /* Char::e */: case  69 /* Char::E */:
                case 102 /* Char::f */: case  70 /* Char::F */:
                    k--;
                    curIndex++;
                    continue;

                default:
                    break loop;
                }
            }
            return curIndex > startIndex && k <= 0;
        }

        function lineComment () {
            let cachedIndex = curIndex;
            let cachedSrc = src;
            while (true) {
                switch (cachedSrc.charCodeAt(cachedIndex++) | 0) {
                case      0 /* Char::Nul */:
                case     10 /* Char::Newline */:
                case     13 /* Char::Return */:
                case 0x2028 /* Char::LS */:
                case 0x2029 /* Char::PS */:
                    cachedIndex--;
                    curIndex = cachedIndex;
                    return;
                }
            }
        }

        function blockComment () {
            let cachedIndex = curIndex;
            let cachedSrc = src;

            while (true) {
                let c;
                while ((c = cachedSrc.charCodeAt(cachedIndex++) | 0) != 42 /* Char::Asterisk */ &&
                       c != 0  /* Char::Nul */ &&
                       c != 10 /* Char::Newline */ &&
                       c != 13 /* Char::CarriageReturn */ &&
                       c != 0x2028 /* Char::LS */ &&
                       c != 0x2029 /* Char::PS */)
                    ;
                if (c == 42 /* Char::Asterisk */) {
                    if (cachedSrc.charCodeAt(cachedIndex) == 47 /* Char::Slash */) {
                        cachedIndex++;
                        curIndex = cachedIndex;
                        return;
                    }
                    continue;
                }
                if (c == 0) {
                    if (cachedIndex >= cachedSrc.length) {
                        curIndex = cachedSrc.length;
                        Lex::syntaxError("End of input in block comment");
                    }
                    continue;
                }
                if (c == 13 /* Char::CarriageReturn */) {
                    if (cachedSrc.charCodeAt(cachedIndex) == 10 /* Char::Newline */)
                        cachedIndex++;
                    c = 10;
                }
                Util::assert(c == 10 /* Char::Newline */ || c == 0x2028 /* Char::LS */ || c == 0x2029 /* Char::PS */);
                lnCoord++;
            }
        }

        function identifier () : int {
            let cachedIndex = curIndex;
            let cachedSrc = src;
            
            // The common case here is that an identifier is a
            // sequence of simple ASCII characters, followed by a
            // non-identifier-constituent ASCII character.  Optimize
            // for this.

            let c;
            let start = cachedIndex;

            while (((c = cachedSrc.charCodeAt(cachedIndex) | 0) >= 97 /* Char::a */ && c <= 122 /* Char::z */) ||
                   (c >= 65 /* Char::A */ && c <= 90 /* Char::Z */) ||
                   (c >= 48 /* Char::0 */ && c <= 57 /* Char::9 */ && cachedIndex > start) ||
                   c === 95 /* Char::_ */ ||
                   c === 36 /* Char::$ */)
                cachedIndex++;

            if (notPartOfIdent[c]) {
                if (cachedIndex == start)
                    Lex::syntaxError("Invalid character in input: " + src.charCodeAt(cachedIndex));
                curIndex = cachedIndex;
                return Token::makeInstance(Token::Identifier, src.substring(start,cachedIndex));
            }

            // Slow case.
            //
            // If at first we fail then try and try again...

            cachedIndex = curIndex;
            start = cachedIndex;

            let s = "";
            let has_backslashes = false;
            while (true) {
                c = cachedSrc.charCodeAt(cachedIndex) | 0;
                if (c == 92 /* Char::Backslash */) {
                    has_backslashes = true;
                    cachedIndex++;
                    c = cachedSrc.charCodeAt(cachedIndex) | 0;
                    if (c == 117 /* Char::u */) {
                        curIndex = cachedIndex+1;
                        c = hexEscape(4);
                        cachedIndex = curIndex;
                    }
                    else
                        Lex::internalError("Illegal escape sequence.");
                }
                if (!(cachedIndex == start ? Char::isUnicodeIdentifierStart(c) : Char::isUnicodeIdentifierPart(c)))
                    break;
                cachedIndex++;
                s += String.fromCharCode(c);
            }
                
            if (has_backslashes) {
                // The ES3 spec requires that identifiers constructed
                // with escape sequences must be checked after the
                // fact to see if they are keywords.  Here we create a
                // new lexer to do that.

                // FIXME (ticket material): It is unclear how this
                // interacts with / should interact with the more
                // liberal rules for keywords in ES4.  For example,
                // "a.if" is legal in ES4.  That being true, does it
                // make sense to prohibit \u0069f as an identifier?
                //
                // The obvious fix might be to interpret \u0069f as
                // "if" and return Token::If here.

                let subscan = new Scanner(s, filename, true);
                if (subscan.start() != Token::Identifier)
                    Lex::syntaxError("Illegal identifier: escape sequence makes it look like a keyword");
                Util::assert(subscan.start() == Token::EOS);
            }

            if (s.length == 0)
                Lex::syntaxError("Invalid character in input: " + src.charCodeAt(cachedIndex));
            curIndex = cachedIndex;
            return Token::makeInstance(Token::Identifier, s);
        }

        function stringLiteral (delimiter) : int {
            let cachedSrc = src;
            let cachedIndex = curIndex;
            let c;
            let s = "";

            if (cachedSrc.charCodeAt(cachedIndex) === delimiter && 
                cachedSrc.charCodeAt(cachedIndex+1) === delimiter) {
                curIndex = cachedIndex + 2;
                return tripleStringLiteral(delimiter);
            }

            while (true) {
                let start = cachedIndex;
                while ((c = cachedSrc.charCodeAt(cachedIndex) | 0) !== delimiter && 
                       c !== 92 /* Char::Backslash */ && 
                       c !== 0  /* Char::Nul */ &&
                       c !== 10 /* Char::Newline */ &&
                       c !== 13 /* Char::Return */ &&
                       c !== 0x2028 /* Char::LS */ &&
                       c !== 0x2029 /* Char::PS */ )
                    cachedIndex++;
                s += cachedSrc.substring(start, cachedIndex);

                switch (cachedSrc.charCodeAt(cachedIndex)) {
                case delimiter:
                    curIndex = cachedIndex + 1;
                    return Token::makeInstance (Token::StringLiteral, s);

                case  92 /* Char::Backslash */:
                    cachedIndex++;

                    switch (cachedSrc.charCodeAt(cachedIndex)) {
                    case 13 /* Char::CarriageReturn */:
                        cachedIndex++;
                        if (cachedSrc.charCodeAt(cachedIndex) === 10 /* Char::Newline */)
                            cachedIndex++;
                        lnCoord++;
                        continue;

                    case 0x2028: /* Char::LS */
                    case 0x2029: /* Char::PS */
                    case 10 /* Char::Newline */:
                        cachedIndex++;
                        lnCoord++;
                        continue;

                    default:
                        curIndex = cachedIndex;
                        s += String.fromCharCode(escapeSequence());
                        cachedIndex = curIndex;
                        continue;
                    }

                case 0 /* Char::Nul */:
                    if (cachedIndex < cachedSrc.length) {
                        s += "\0";
                        cachedIndex++;
                        continue;
                    }
                    /* FALLTHROUGH */

                default:
                    Lex::syntaxError("Unterminated string literal: " + s);
                }
            }
        }

        // The initial triplequote has been consumed.
        //
        // FIXME: A lot of overlap between this function and the
        // regular string scanner: factor or parameterize?

        function tripleStringLiteral(delimiter) {
            let cachedSrc = src;
            let cachedIndex = curIndex;
            let c;
            let s = "";

            while (true) {
                let start = cachedIndex;
                while ((c = cachedSrc.charCodeAt(cachedIndex) | 0) !== delimiter && 
                       c !== 92 /* Char::Backslash */ && 
                       c !== 13 /* Char::Return */ &&
                       c !== 0  /* Char::Nul */)
                    cachedIndex++;
                s += cachedSrc.substring(start, cachedIndex);

                switch (cachedSrc.charCodeAt(cachedIndex)) {
                case delimiter:
                    if (cachedSrc.charCodeAt(cachedIndex+1) === delimiter &&
                        cachedSrc.charCodeAt(cachedIndex+2) === delimiter &&
                        cachedSrc.charCodeAt(cachedIndex+3) !== delimiter) {
                        curIndex = cachedIndex + 3;
                        return Token::makeInstance (Token::StringLiteral, s);
                    }

                    s += String.fromCharCode(delimiter);
                    cachedIndex++;
                    continue;

                case 13 /* Char::CarriageReturn */:
                    cachedIndex++;
                    if (cachedSrc.charCodeAt(cachedIndex) === 10 /* Char::Newline */)
                        cachedIndex++;
                    s += String.fromCharCode(10);
                    continue;

                case  92 /* Char::Backslash */:
                    cachedIndex++;

                    switch (cachedSrc.charCodeAt(cachedIndex)) {
                    case 13 /* Char::CarriageReturn */:
                        cachedIndex++;
                        if (cachedSrc.charCodeAt(cachedIndex) === 10 /* Char::Newline */)
                            cachedIndex++;
                        lnCoord++;
                        continue;

                    case 0x2028: /* Char::LS */
                    case 0x2029: /* Char::PS */
                    case 10 /* Char::Newline */:
                        cachedIndex++;
                        lnCoord++;
                        continue;

                    default:
                        curIndex = cachedIndex;
                        s += String.fromCharCode(escapeSequence());
                        cachedIndex = curIndex;
                        continue;
                    }

                case 0 /* Char::Nul */:
                    if (cachedIndex < cachedSrc.length) {
                        s += "\0";
                        cachedIndex++;
                        continue;
                    }
                    else
                        Lex::syntaxError("Unterminated string literal: " + s);
                }
            }
        }

        function escapeSequence () : int {
            switch (src.charCodeAt(curIndex) | 0) {
            case  48 /* Char::Zero */:
            case  49 /* Char::One */:
            case  50 /* Char::Two */:
            case  51 /* Char::Three */:
            case  52 /* Char::Four */:
            case  53 /* Char::Five */:
            case  54 /* Char::Six */:
            case  55 /* Char::Seven */:
                return octalOrNulEscape ();

            case 120 /* Char::x */:
                curIndex++;
                return hexEscape (2);

            case 117 /* Char::u */:
                curIndex++;
                return hexEscape (4);

            case  98 /* Char::b */:
                curIndex++;
                return Char::Backspace;

            case 102 /* Char::f */:
                curIndex++;
                return Char::Formfeed;

            case 110 /* Char::n */:
                curIndex++;
                return Char::Newline;

            case 114 /* Char::r */:
                curIndex++;
                return Char::CarriageReturn;

            case 116 /* Char::t */:
                curIndex++;
                return Char::Tab;

            case 118 /* Char::v */:
                curIndex++;
                return Char::VerticalTab;

            case  39 /* Char::SingleQuote */:
            case  34 /* Char::DoubleQuote */:
            case  92 /* Char::BackSlash */:
                return src.charCodeAt(curIndex++);

            case   0:
                if (curIndex+1 == src.length)
                    Lex::syntaxError("End of input in escape sequence");
                curIndex++;
                return 0;  // NUL

            case     10: /* Char::Linefeed */
            case     13: /* Char::CarriageReturn */
            case 0x2028: /* Char::LS */
            case 0x2029: /* Char::PS */
                Lex::syntaxError("Illegal line terminator in escape sequence");

            default:
                return src.charCodeAt(curIndex++);
            }
        }

        function octalOrNulEscape () : uint {
            switch (src.charCodeAt(curIndex)) {
            case  48 /* Char::Zero */:
                curIndex++;
                switch (src.charCodeAt(curIndex)) {
                case  /* 49 */ Char::One :
                case  50 /* Char::Two */:
                case  51 /* Char::Three */:
                case  52 /* Char::Four */:
                case  53 /* Char::Five */:
                case  54 /* Char::Six */:
                case  55 /* Char::Seven */:
                    return octalEscape(3);
                default:
                    return 0;  // \0
                }

            case  49 /* Char::One */:
            case  50 /* Char::Two */:
            case  51 /* Char::Three */:
                return octalEscape(3);

            case  52 /* Char::Four */:
            case  53 /* Char::Five */:
            case  54 /* Char::Six */:
            case  55 /* Char::Seven */:
                return octalEscape(2);

            default:
                Lex::syntaxError( "Expecting octal character, got " + c);
            }
        }

        function octalEscape (n) {
            markIndex = curIndex;
            octalDigits(n);                 // ignore result
            return parseInt(lexeme(), 8);
        }

        // Any leading x has been consumed.  n is the number of digits to consume and require.

        function hexEscape (n) : uint {
            markIndex = curIndex;
            if (!hexDigits(n))
                Lex::syntaxError("Wrong number of hexadecimal digits; expected " + n);
            return parseInt(lexeme(), 16);
        }
    }
}
