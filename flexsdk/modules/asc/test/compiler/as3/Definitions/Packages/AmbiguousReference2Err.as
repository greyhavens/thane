package P
{
	public class C
	{
		public function C() { trace("P.C") }
	}
}

import P.*;

class C
{
public function C() { trace("Q.C") }
}

c = new C();
