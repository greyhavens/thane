/*
1000	Ambiguous reference to _.
*/

package test {
	public namespace rss;
	public namespace xml;
		
	public class AmbiguousReferenceErr {
		public function ErrorExamples() {
			use namespace rss;
			use namespace xml;
			trace(hello());
		}
			
		rss function hello():String {
			return "hola";
		}
		
		xml function hello():String {
			return "foo";
		}
	}
}