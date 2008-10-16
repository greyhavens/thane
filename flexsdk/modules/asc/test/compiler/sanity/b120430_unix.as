var soap = new Namespace("http://schemas.xmlsoap.org/soap/envelope/");
var stock = new Namespace("http://mycompany.com/stocks");
default xml namespace = soap;

// Create an XML initializer in the default (i.e., soap) namespace
message =
<Envelope 
xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
<Body>
<stock:GetLastTradePrice xmlns:stock="http://mycompany.com/stocks">
<stock:symbol>DIS</stock:symbol>
</stock:GetLastTradePrice>
</Body>
</Envelope>;
print(message.toXMLString())

