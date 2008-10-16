o = {a:10,b:20,c:30}
var n = 0
for(p in o) 
{
	switch(p)
	{
		case 'a':
		print('1>',o[p]==10)
		break;
		case 'b':
		print('1>',o[p]==20)
		break;
		case 'c':
		print('1>',o[p]==30)
		break;
		default:
		print('error')
		break;
	}
	++n
    for(q in o) 
    {
		switch(q)
		{
		    case 'a':
		    print('2>',o[q]==10)
		    break;
		    case 'b':
		    print('2>',o[q]==20)
		    break;
		    case 'c':
		    print('2>',o[q]==30)
		    break;
	    }
	    ++n
	} 
}
print(n)