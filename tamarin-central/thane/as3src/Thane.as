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
import avmplus.Yard;
import avmplus.System;

public class Thane
{
    Thanette.systemSetup(httpClientFactory);

    public static function spawnYard (
        yardId :String, userCode :ByteArray, consoleTracePrefix :String,
        bridge :EventDispatcher) :Yard
    {
        if (!Thanette.isSystemYard()) {
            throw new Error("Non-system yard spawning");
        }

        if (yardId == null || yardId.length == 0) {
            throw new Error ("Yard must be spawned with an identifier");
        }
        if (_spawnedYards[yardId] != null) {
            throw new Error ("Yard identifier not unique");
        }

        var env :Yard = new Yard(userCode);

        var thanette :Class = env.domain.getClass("Thanette");
        if (thanette == null) {
            throw new Error ("Could not locate Thanette in new Yard");
        }
        var initFun :Function = thanette["initializeYard"];
        if (initFun == null) {
            throw new Error("Could not locate initializeYard() on foreign Thanette class");
        }
        _spawnedYards[yardId] = new SpawnedYard();
        _spawnedYards[yardId].yard = env;
        _spawningYard = yardId;
        try {
            initFun(yardId, consoleTracePrefix, bridge, requestSpawnedHeartbeat, registerTrace,
                    httpClientFactory);

        } catch (e :Error) {
            trace("Yard initialization error: " + e.getStackTrace());
            delete _spawnedYards[yardId];
            return null;

        } finally {
            _spawningYard = null;
        }
        
        return env;
    }

    public static function unspawnYard (yardId :String) :void
    {
        delete _spawnedYards[yardId];
    }

    /**
     * Outputs a message and optional error to a domain's trace function.
     */
    public static function outputToTrace (yard :Yard, msg :String, err :Error) :void
    {
        for each (var spawned :SpawnedYard in _spawnedYards) {
            if (spawned.yard == yard) {
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
                if (Thanette.isSystemYard()) {
                    // each top-level script gets its own 15 second timeout
                    System.resetTimeout();
                }

                heart();
            } catch (err :Error) {
                trace("Heartbeat error: " + err.getStackTrace());
            }
        }
        for each (var spawned :SpawnedYard in _spawnedYards) {
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
        if (!Thanette.isSystemYard() || _spawningYard == null) {
            throw new Error("Master heartbeat inaccessible: " +
                            Thanette.getYardId() + ", " + _spawningYard);
        }
        var spawned :SpawnedYard = _spawnedYards[_spawningYard];
        if (spawned == null) {
            throw new Error("Yard not spawned");
        }
        if (spawned.heartbeat != null) {
            throw new Error("Yard heartbeat already allocated");
        }
        spawned.heartbeat = heart;
    }

    protected static function registerTrace (traceFn :Function) :void
    {
        var spawned :SpawnedYard = _spawnedYards[_spawningYard];
        if (spawned == null) {
            throw new Error("Yard not spawned");
        }
        spawned.traceFn = traceFn;
    }
    
    protected static function httpClientFactory () :CachingHttpClient
    {
        return new CachingHttpClient();
    }

    /** The SpawnedYard objects created by spawnYard. */
    private static var _spawnedYards :Dictionary = new Dictionary();

    /** Heartbeats requested in this yard. */
    private static var _hearts :Array = new Array();

    /** Only set when spawning a yard. */
    private static var _spawningYard :String = null;
}
}

import avmplus.Yard;

/** Tracks the things needed for a spawned yard */
class SpawnedYard
{
    /** The yard object. */
    public var yard :Yard;

    /** The heartbeat function within the yard to call as often as possible. */
    public var heartbeat :Function;

    /** The trace function within the yard. */
    public var traceFn :Function;
}
