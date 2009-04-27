//
// $Id$
//
// Thane - an enhancement of Mozilla/Adobe's Tamarin project with partial Flash Player compatibility
// Copyright (C) 2008-2009 Three Rings Design, Inc.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package {

import avmplus.Domain;

import flash.events.EventDispatcher;
import flash.net.CachingHttpClient;

public class Thanette
{
    public static function isSystemYard () :Boolean
    {
        return _yardId == null;
    }

    public static function getYardId () :String
    {
        return _yardId;
    }

    public static function getBridgeHome () :EventDispatcher
    {
        return _bridge;
    }

    public static function getConsoleTracePrefix () :String
    {
        return _consoleTracePrefix;
    }

    // some parts of Thanette we want to work in the system yard too
    public static function systemSetup (cacheFactory :Function) :void
    {
        if (isSystemYard()) {
            _cacheFactory = cacheFactory;
        }
    }

    public static function initializeYard (
        yardId :String, consoleTracePrefix :String, bridge :EventDispatcher,
        foreignHeart :Function, registerTrace :Function, cacheFactory :Function) :void
    {
        if (yardId == null || yardId.length == 0) {
            throw new Error ("Yard must be spawned with an identifier");
        }
        if (_initialized) {
            throw new Error("This yard has already been initialized");
        }
        _initialized = true;
        _yardId = yardId;
        _bridge = bridge;
        _consoleTracePrefix = consoleTracePrefix;
        _cacheFactory = cacheFactory;

        if (foreignHeart != null) {
            foreignHeart(Thane.heartbeat);
        }

        if (registerTrace != null) {
            registerTrace(trace);
        }
    }

    public static function getCachingHttpClient () :CachingHttpClient
    {
        return _cacheFactory() as CachingHttpClient;
    }

    public static function traceToBridge (s :Array) :void
    {
        if (_bridge != null) {
            _bridge.dispatchEvent(
                new TraceEvent(TraceEvent.TRACE, false, false, Domain.currentDomain, s));
        }
    }

    private static var _initialized :Boolean;
    private static var _yardId :String;
    private static var _bridge :EventDispatcher;
    private static var _consoleTracePrefix :String;
    private static var _cacheFactory :Function;
}
}
