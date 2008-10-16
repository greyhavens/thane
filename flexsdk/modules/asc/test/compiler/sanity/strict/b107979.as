var x = 
<rectangle>
<a>30</a>
<b>50</b>
</rectangle>;

var x1 = <rectangle>{x.b}{x.a}</rectangle>;
print(x1 == x);
