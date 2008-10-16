/**
 *
 *    Error 1168: Illegal assignment to function _.
 *
 */

function topLevel()
{
	var int=1;
}

topLevel = function() 
	{ 
		var int=2;
	}

