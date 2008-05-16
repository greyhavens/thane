/**
 * Copyright (c) 2007 Gabriel Handford
 * See LICENSE.txt for full license information.
 */
package org.httpclient {
  
  import com.adobe.net.URI;
  
  import flash.events.EventDispatcher;
  import flash.events.Event;
  
  import org.httpclient.http.Put;
  import org.httpclient.http.Post;
  import org.httpclient.http.Get;
  import org.httpclient.events.HttpListener;
  
  import org.httpclient.http.multipart.Multipart;
  import org.httpclient.http.multipart.FilePart;
    
  [Event(name=Event.CLOSE, type="flash.events.Event")]
  [Event(name=Event.COMPLETE, type="flash.events.Event")]
  [Event(name=Event.CONNECT, type="flash.events.Event")]	
  [Event(name=HttpDataEvent.DATA, type="org.httpclient.events.HttpProgressEvent")]   
  [Event(name=HttpErrorEvent.ERROR, type="org.httpclient.events.HttpErrorEvent")]  
  [Event(name=HttpStatusEvent.STATUS, type="org.httpclient.events.HttpStatusEvent")]  

  /**
   * HTTP Client.
   */
  public class HttpClient extends EventDispatcher {

    private var _socket:HttpSocket;  
    private var _listener:*;
    
    /**
     * Create HTTP client.
     */
    public function HttpClient() {
      
    }
    
    /**
     * Get listener.
     * Redirects events to callbacks.
     * You can use this listener, or use the EventDispatcher listener. Your choice.
     *  
     * @return Listener
     */
    public function get listener():HttpListener {
      if (!_listener) {
        _listener = new HttpListener();
        _listener.register(this);        
      }
      return _listener;
    }
    
    /**
     * Set the listener.
     * @para listeners { onComplete: function(..) { }, onData: function(..) { }, .... }
     */
    public function set listener(listeners:*):void {
      _listener = new HttpListener(listeners);
      _listener.register(this);
    }

    /**
     * Load a generic request.
     * 
     * @param uri URI
     * @param request HTTP request
     */
    public function request(uri:URI, request:HttpRequest, timeout:int = -1):void {
      _socket = new HttpSocket(this, timeout);
      _socket.request(uri, request);
    }
    
    /**
     * Upload file to URI. Sorry no progress events, and don't upload anything beefy since it 
     * will all buffer in memory. flash.net.Socket is crippled.
     *  
     *  var client:HttpClient = new HttpClient();
     *  
     *  client.listener.onComplete = function():void { ... };
     *  client.listener.onStatus = function(r:HttpResponse):void { ... };
     * 
     *  var uri:URI = new URI("http://http-test.s3.amazonaws.com/test_put.png");
     *  var testFile:File = new File("app:/test/assets/test.png");
     * 
     *  client.upload(uri, testFile);
     *  
     * @param uri
     * @param file (Should be flash.filesystem.File; Not typed for compatibility with Flash)
     * @param method PUT or POST
     * @param 
     */
    public function upload(uri:URI, file:*, method:String = "PUT"):void {
      var httpRequest:HttpRequest = null;
      if (method == "PUT") httpRequest = new Put();
      else if (method == "POST") httpRequest = new Post();
      else throw new ArgumentError("Method must be PUT or POST");
            
      // Comment out to compile under flash
//      httpRequest.multipart = new Multipart([ new FilePart(file) ]);    
      //throw new DefinitionError("Upload not supported; Rebuild with upload support.");
      
      request(uri, httpRequest);      
    }
    
    /**
     * Get request.
     * @param uri
     */
    public function get(uri:URI):void {
      request(uri, new Get());
    }
    
    /**
     * Get request.
     * @param uri
     * @param listeners
     */
    public static function get(uri:URI, listeners:*):void {
      var client:HttpClient = new HttpClient();
      client.listener = listeners;
      client.get(uri);
    }
    
    /**
     * Post.
     *  
     * var variables:Array = [ 
     *  { name: "fname", value: "FirstName1" }, 
     *  { name: "lname", value: "LastName1" } ];
     *  
     * post(new URI("http://foo.com/"), variables);
     *  
     * @param uri
     * @param variables
     */
    public function post(uri:URI, variables:Array):void {
      request(uri, new Post(variables));
    }
    
  }

}