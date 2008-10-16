var o = {a:1}
with(o) { hasOwnProperty("a") }

var x = <x><y a="10"><z/></y></x>
x.y.(hasOwnProperty("@a"))