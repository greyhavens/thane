import flash.utils.*


function basics(d)
{
	var k1 = {}
	var k2 = {}

	// test get/set
	d[k1] = "bomb"
	d[k2] = "tom"
	trace("b1:" + (d[k1] == "bomb"))
	trace("b2:" + (d[k2] == "tom"))

	// test in 
	trace("b3:" + (k1 in d))
	trace("b4:" + (k2 in d))

	// iterate values
	for each(var x in d) {
		trace("b5:" + (x == "tom" || x == "bomb"))
	}

	// iterate keys
	for (var x in d) {
		trace("b6:" + (d[x] == "tom" || d[x] == "bomb"))
	}

	// test delete 
	delete d[k1]
	trace("b7:" + (d[k1] == undefined))
}

function dict_test()
{
	basics(new Dictionary())
}

function reap_drc()
{
	for(var i:int=0;i<1000;i++) new Object
}


function createAndPopulate()
{
	var d = new Dictionary(true)
	basics(d)
	return d
}

function weak_test()
{
	var d = createAndPopulate()

	// add a bunch of objects that should go away
	for(var i:int=0;i<40;i++) d[new Object] = "asdf"

	reap_drc()

	// now add a bunch that won't go away
	var a=[]
	for(var i:int=0;i<666;i++)  {
		new Object
		var o = new Object
		a.push(o)
		d[o]="adsf"
	}

	var count:int;
	for(var x in d) count++
	trace(count)
}

dynamic class A extends Dictionary
{
	function toString() { return function() { return "A" }()  }
}

function inheritance_test()
{
	var d = new A
	d[d] = d
	trace("inher1:" + (d[d] == d))
	trace("inher2:" + (d["A"] != d)) // makes sure we're not going through toString
}

function method_closure_test()
{
	var d = new Dictionary()
	var a = new A
	d[a.toString] = "test"
	trace("mct:" + (d[a.toString] == "test"))
}

inheritance_test()
dict_test()
weak_test()
method_closure_test()