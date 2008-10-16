// This tests back references within zero width positive lookaheads and zero width negative lookaheads
//  See 15.10.2.8 of the ECMAScript ed 3 spec, specifically the part entitled "Informative comments"

print(/(.*?)a(?!(a+)b\2c)\2(.*)/.exec("baaabaac")); // negative lookahead, \2 matches "aa" within the (?!), and undefined outside of it.  result is ["baaabaac", "ba", undefined, "abaac"], not ["baaabaac", "ba", "aa", "abaac"]
print(/(?=(a+))/.exec("baaabac"))    // positive lookahead, matches empty space after the first b, result ["","aaa"]
print(/(?=(a+))a*b\1/.exec("baaabac")); // positive lookahead, result should be ["aba", "a"], not ["aaab","aaa"]

