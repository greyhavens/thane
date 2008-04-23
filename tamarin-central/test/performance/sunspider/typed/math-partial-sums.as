/*
 Copyright (C) 2007 Apple Inc.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

// The Computer Language Shootout
// http://shootout.alioth.debian.org/
// contributed by Isaac Gouy

function partial(n:Number):void{
    var a1:Number,a2:Number,a3:Number,a4:Number,a5:Number,a6:Number,a7:Number,a8:Number,a9:Number;
    a1 = a2 = a3 = a4 = a5 = a6 = a7 = a8 = a9 = 0.0;
    var twothirds:Number = 2.0/3.0;
    var alt:Number = -1.0;
    var k2:Number = k3 = sk = ck= 0.0;
    
    for (var k:int = 1; k <= n; k++){
        k2 = k*k;
        k3 = k2*k;
        sk = Math.sin(k);
        ck = Math.cos(k);
        alt = -alt;
        
        a1 += Math.pow(twothirds,k-1);
        a2 += Math.pow(k,-0.5);
        a3 += 1.0/(k*(k+1.0));
        a4 += 1.0/(k3 * sk*sk);
        a5 += 1.0/(k3 * ck*ck);
        a6 += 1.0/k;
        a7 += 1.0/k2;
        a8 += alt/k;
        a9 += alt/(2*k -1);
    }
}

function runMathPartialSums():int {
var _sunSpiderStartDate:Date = new Date();
for (var i:int = 1024; i <= 16384; i *= 2) {
    partial(i);
}
var _sunSpiderInterval:int = new Date() - _sunSpiderStartDate;
return _sunSpiderInterval;
}

print("metric math-partial-sums-typed " + runMathPartialSums());

