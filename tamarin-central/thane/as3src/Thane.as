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

import flash.events.EventDispatcher;
import flash.net.CachingHttpClient;
import flash.utils.ByteArray;
import flash.utils.Dictionary;

import com.adobe.utils.ArrayUtil;

import avmplus.Puddle;
import avmplus.System;
import avmplus.Yard;

public class Thane
{
    Thanette.systemSetup(httpClientFactory);

    public static function spawnPuddle (
        puddleId :String, yard :Yard, consoleTracePrefix :String,
        bridge :EventDispatcher) :Puddle
    {
        if (!Thanette.isSystemPuddle()) {
            throw new Error("Non-system puddle spawning");
        }

        if (puddleId == null || puddleId.length == 0) {
            throw new Error ("Puddle must be spawned with an identifier");
        }
        if (_spawnedPuddles[puddleId] != null) {
            throw new Error ("Puddle identifier not unique");
        }

        var env :Puddle = new Puddle(yard);

        var thanette :Class = env.domain.getClass("Thanette");
        if (thanette == null) {
            throw new Error ("Could not locate Thanette in new Puddle");
        }
        var initFun :Function = thanette["initializePuddle"];
        if (initFun == null) {
            throw new Error("Could not locate initializePuddle() on foreign Thanette class");
        }
        _spawnedPuddles[puddleId] = new SpawnedPuddle();
        _spawnedPuddles[puddleId].puddle = env;
        _spawningPuddle = puddleId;
        try {
            initFun(puddleId, consoleTracePrefix, bridge, requestSpawnedHeartbeat, registerTrace,
                    httpClientFactory);

        } catch (e :Error) {
            trace("Puddle initialization error: " + e.getStackTrace());
            delete _spawnedPuddles[puddleId];
            return null;

        } finally {
            _spawningPuddle = null;
        }
        
        return env;
    }

    public static function unspawnPuddle (puddleId :String) :void
    {
        var puddle :SpawnedPuddle = _spawnedPuddles[puddleId];
        if (puddle != null) {
            ArrayUtil.removeValueFromArray(_hearts, puddle.heartbeat);
        }
        delete _spawnedPuddles[puddleId];
    }

    /**
     * Outputs a message and optional error to a domain's trace function.
     */
    public static function outputToTrace (puddle :Puddle, msg :String, err :Error) :void
    {
        for each (var spawned :SpawnedPuddle in _spawnedPuddles) {
            if (spawned.puddle == puddle) {
                spawned.traceFn(msg);
                if (err != null) {
                    spawned.traceFn(err.getStackTrace());
                }
                return;
            }
        }
    }

    public static function requestHeartbeat (heart :Function) :void
    {
        if (-1 == _hearts.indexOf(heart)) {
            _hearts.push(heart);
        }
    }

    public static function heartbeat () :void
    {
        for each (var heart :Function in _hearts) {
            try {
                if (Thanette.isSystemPuddle()) {
                    // each top-level script gets its own 15 second timeout
                    System.resetTimeout();
                }

                heart();
            } catch (err :Error) {
                trace("Heartbeat error: " + err.getStackTrace());
            }
        }
        for each (var spawned :SpawnedPuddle in _spawnedPuddles) {
            if (spawned.heartbeat == null) {
                continue;
            }
            try {
                spawned.heartbeat();
            } catch (err :Error) {
                trace("Heartbeat error: " + err.getStackTrace());
            }
        }
    }

    protected static function requestSpawnedHeartbeat (heart :Function) :void
    {
        if (!Thanette.isSystemPuddle() || _spawningPuddle == null) {
            throw new Error("Master heartbeat inaccessible: " +
                            Thanette.getPuddleId() + ", " + _spawningPuddle);
        }
        var spawned :SpawnedPuddle = _spawnedPuddles[_spawningPuddle];
        if (spawned == null) {
            throw new Error("Puddle not spawned");
        }
        if (spawned.heartbeat != null) {
            throw new Error("Puddle heartbeat already allocated");
        }
        spawned.heartbeat = heart;
    }

    protected static function registerTrace (traceFn :Function) :void
    {
        var spawned :SpawnedPuddle = _spawnedPuddles[_spawningPuddle];
        if (spawned == null) {
            throw new Error("Puddle not spawned");
        }
        spawned.traceFn = traceFn;
    }
    
    protected static function httpClientFactory () :CachingHttpClient
    {
        return new CachingHttpClient();
    }

    /** The SpawnedPuddle objects created by spawnPuddle. */
    private static var _spawnedPuddles :Dictionary = new Dictionary();

    /** Heartbeats requested in this puddle. */
    private static var _hearts :Array = new Array();

    /** Only set when spawning a puddle. */
    private static var _spawningPuddle :String = null;
}
}

import avmplus.Puddle;

/** Tracks the things needed for a spawned puddle */
class SpawnedPuddle
{
    /** The puddle object. */
    public var puddle :Puddle;

    /** The heartbeat function within the puddle to call as often as possible. */
    public var heartbeat :Function;

    /** The trace function within the puddle. */
    public var traceFn :Function;
}
