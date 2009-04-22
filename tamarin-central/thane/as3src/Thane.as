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
import flash.utils.Dictionary;
import avmplus.Domain;
import avmplus.System;

public class Thane
{
    public static function spawnDomain (
        domainId :String, consoleTracePrefix :String, bridge :EventDispatcher) :Domain
    {
        if (!Thanette.isSystemDomain()) {
            throw new Error("Non-system domain spawning");
        }

        var dom :Domain = new Domain();

        if (domainId == null || domainId.length == 0) {
            throw new Error ("Domain must be spawned with an identifier");
        }
        if (_spawnedDomains[domainId] != null) {
            throw new Error ("Domain identifier not unique");
        }

        var thane :Class = dom.getClass("Thanette");
        if (thane == null) {
            throw new Error ("Could not locate Thanette in new Domain");
        }
        var initFun :Function = thane["initializeDomain"];
        if (initFun == null) {
            throw new Error("Could not locate initializeDomain() on foreign Thanette class");
        }
        _spawnedDomains[domainId] = new SpawnedDomain();
        _spawnedDomains[domainId].domain = dom;
        _spawningDomain = domainId;
        try {
            initFun(domainId, consoleTracePrefix, bridge, requestSpawnedHeartbeat, registerTrace);

        } catch (e :Error) {
            unspawnDomain(dom);
            return null;

        } finally {
            _spawningDomain = null;
        }
        
        return dom;
    }

    public static function unspawnDomain (domain :Domain) :void
    {
        for (var domainId :String in _spawnedDomains) {
            if (_spawnedDomains[domainId].domain == domain) {
                delete _spawnedDomains[domainId];
                return;
            }
        }
        throw new Error("Domain not spawned");
    }

    /**
     * Outputs a message and optional error to a domain's trace function.
     */
    public static function outputToTrace (domain :Domain, msg :String, err :Error) :void
    {
        for each (var spawned :SpawnedDomain in _spawnedDomains) {
            if (spawned.domain == domain) {
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
                // each top-level script gets its own 15 second timeout
                System.resetTimeout();

                heart();
            } catch (err :Error) {
                trace("Heartbeat error: " + err.getStackTrace());
            }
        }
        for each (var spawned :SpawnedDomain in _spawnedDomains) {
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
        if (!Thanette.isSystemDomain() || _spawningDomain == null) {
            throw new Error("Master heartbeat inaccessible");
        }
        var spawned :SpawnedDomain = _spawnedDomains[_spawningDomain];
        if (spawned == null) {
            throw new Error("Domain not spawned");
        }
        if (spawned.heartbeat != null) {
            throw new Error("Domain heartbeat already allocated");
        }
        spawned.heartbeat = heart;
    }

    protected static function registerTrace (traceFn :Function) :void
    {
        var spawned :SpawnedDomain = _spawnedDomains[_spawningDomain];
        if (spawned == null) {
            throw new Error("Domain not spawned");
        }
        spawned.traceFn = traceFn;
    }
    
    /** The SpawnedDomain objects created by spawnDomain. */
    private static var _spawnedDomains :Dictionary = new Dictionary();

    /** Heartbeats requested in this domain. */
    private static var _hearts :Array = new Array();

    /** Only set when spawning a domain. */
    private static var _spawningDomain :String = null;
}
}

import avmplus.Domain;

/** Tracks the things needed for a spawned domain. */
class SpawnedDomain
{
    /** The domain object. */
    public var domain :Domain;

    /** The heartbeat function within the domain to call as often as possible. */
    public var heartbeat :Function;

    /** The trace function within the domain. */
    public var traceFn :Function;
}
