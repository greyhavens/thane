class A
{
        function b(n:Number):Number
        {
                switch(n)
                {
                        case 1: return 10;
                        case 2: return 20;
                }

                trace("do something");
                return 0;
        }

        function c():void
        {
                trace("calling b");
                b(1);
                trace("called b");
        }
}

var a:A = new A();
a.c();

