/**
 * Copyright (c) 2007 Gabriel Handford
 * See LICENSE.txt for full license information.
 */
package org.httpclient {
  
  import com.adobe.net.URI;
  
  import flash.net.Socket;
  import flash.utils.ByteArray;
  import flash.utils.Timer;
  
  import flash.events.Event;
  import flash.events.EventDispatcher;
  import flash.events.IOErrorEvent;
  import flash.events.SecurityErrorEvent;
  import flash.events.ProgressEvent;  
  import flash.errors.EOFError;
  import flash.events.TimerEvent;
  
  import org.httpclient.io.HttpRequestBuffer;
  import org.httpclient.io.HttpResponseBuffer;
  import org.httpclient.io.HttpBuffer;
  
  import org.httpclient.events.*;
      
  /**
   * HTTP Socket.
   *  
   * Follow the HTTP 1.1 spec: http://www.w3.org/Protocols/rfc2616/rfc2616.html
   */
  public class HttpSocket {
    
    public static const DEFAULT_HTTP_PORT:uint = 80;   
    public static const DEFAULT_HTTPS_PORT:uint = 443; 
    public static const HTTP_VERSION:String = "1.1";
    
    // Socket or TLSSocket
    private var _socket:*; 
    
    // Event dispatcher
    private var _dispatcher:EventDispatcher;

    // Timer
    private var _timer:HttpTimer;
    
    // Internal callbacks
    private var _onConnect:Function;
    
    // Buffers
    private var _requestBuffer:HttpRequestBuffer;
    private var _responseBuffer:HttpResponseBuffer;    
    
    /**
     * Create HTTP socket.
     *  
     * @param dispatcher Event dispatcher
     * @param timeout Timeout in millis
     */
    public function HttpSocket(dispatcher:EventDispatcher, timeout:Number = -1) {
      _dispatcher = dispatcher;
      if (timeout < 1000) timeout = 60 * 1000;
      _timer = new HttpTimer(timeout, onTimeout);
    }
    
    /**
     * Create the socket.
     * Create Socket or TLSSocket depending on URI scheme (http or https).
     */
    protected function createSocket(secure:Boolean = false):void {      
        if (secure) {
            throw new Error("secure sockets not supported");
        }
        _socket = new Socket();
      
      _socket.addEventListener(Event.CONNECT, onConnect);       
      _socket.addEventListener(Event.CLOSE, onClose);
      _socket.addEventListener(IOErrorEvent.IO_ERROR, onIOError);
      _socket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, onSecurityError);
      _socket.addEventListener(ProgressEvent.SOCKET_DATA, onSocketData);      
    }
    
    /**
     * Default port.
     */
    public function getDefaultPort(secure:Boolean = false):uint {
      if (secure) return DEFAULT_HTTPS_PORT;
      return DEFAULT_HTTP_PORT;
    }
    
    /**
     * Close socket.
     */
    public function close():void {
      _timer.stop();
      if (_socket.connected) {
        _socket.flush();
        _socket.close();
      }
    }
    
    /**
     * Initiate the connection (if not connected) and send the request.
     * @param request HTTP request
     */
    public function request(uri:URI, request:HttpRequest):void {
      var onConnect:Function = function():void {
        sendRequest(uri, request);
      };
      
      // Connect
      connect(uri, onConnect);
    }
   
    /**
     * Connect (to URI).
     * @param uri Resource to connect to
     * @param onConnect On connect callback
     */
    protected function connect(uri:URI, onConnect:Function = null):void {
      _onConnect = onConnect;

      // Create the socket
      var secure:Boolean = (uri.scheme == "https");
      createSocket(secure);

      // Start timer
      _timer.start();
      
      // Connect
      var port:int = Number(uri.port);
      if (!port) port = getDefaultPort(secure);
      
      var host:String = uri.authority;      
      Log.debug("Connecting: host: " + host + ", port: " + port);
      _socket.connect(host, port);              
    }
    
    /**
     * Send request.
     * @param uri URI
     * @param request Request to write
     */
    protected function sendRequest(uri:URI, request:HttpRequest):void {               
      // Prepare response buffer
      _responseBuffer = new HttpResponseBuffer(onResponseHeader, onResponseData);
      
      var header:ByteArray = request.getHeader(uri, HTTP_VERSION);
      Log.debug("Sending request: " + header + "--");
      _socket.writeBytes(header);
      _socket.flush();
      _timer.reset();
      
      if (request.hasRequestBody) {
        
        _requestBuffer = new HttpRequestBuffer(request.body);
        
        while (_requestBuffer.hasData) {
          var bytes:ByteArray = _requestBuffer.read();
          Log.debug("<" + bytes.length + ">");
          if (bytes.length > 0) {
            _socket.writeBytes(bytes);
            _timer.reset();
             
            // Flush doesn't work. We are totally fucked.
            // http://tech.groups.yahoo.com/group/flexcoders/message/72197
            _socket.flush();
          }
        }
      }
      Log.debug("Send request done");
    }
    
    
    /**
     * Socket data available.
     */
    private function onSocketData(event:ProgressEvent):void {
      while (_socket.bytesAvailable) {        
        _timer.reset();
        try {           
          
          // Load data from socket
          var bytes:ByteArray = new ByteArray();
          _socket.readBytes(bytes, 0, _socket.bytesAvailable);                       
          bytes.position = 0;
          
          // Write to response buffer
          _responseBuffer.writeBytes(bytes);
          
        } catch(e:EOFError) {
          // TODO: handle EOF
          Log.debug("EOF");
          break;
        }                           
      }
    }
    
    //
    // Events (Custom listeners)
    //
    
    private function onResponseHeader(response:HttpResponse):void {
      Log.debug("Response: " + response.code);
      _dispatcher.dispatchEvent(new HttpStatusEvent(response));
    }
    
    private function onResponseData(bytes:ByteArray):void {
      _dispatcher.dispatchEvent(new HttpDataEvent(bytes));
    }
    
    private function onComplete():void {
      Log.debug("Complete");
      _dispatcher.dispatchEvent(new Event(Event.COMPLETE));
    }
    
    private function onTimeout(idleTime:Number):void {
      close();
      _dispatcher.dispatchEvent(new HttpErrorEvent("error", false, false, "Timeout", 0));
      
    }
    
    //
    // Events (Socket listeners)
    //

    private function onConnect(event:Event):void {    
      _dispatcher.dispatchEvent(event.clone());
      
      // Internal callback
      if (_onConnect != null) _onConnect(event);            
    }
  
    private function onClose(event:Event):void {
      onComplete(); // TODO: earlier?
      close();
      _dispatcher.dispatchEvent(event.clone());                
    }
    
    private function onIOError(event:IOErrorEvent):void { 
      close();
      _dispatcher.dispatchEvent(event.clone());
    }

    private function onSecurityError(event:SecurityErrorEvent):void {
      close();
      _dispatcher.dispatchEvent(event.clone());
    }
    
    
    
  }
}