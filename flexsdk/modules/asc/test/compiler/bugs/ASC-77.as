package 
{

public class Perkins
{
	private var _pages : Array = new Array();
	
	public function enumerateEvents( _events : Array, func : Function ) : void
	{
		for ( var x : int = 0, numItems : int = _events.length; x < _events.length; x++)
		{
			var eventObj : Object = Object( _events[x] );
			if (func.call( this, x, numItems, eventObj ) == false)
				break;
		}
	}
	
	public function enumerateAnimations( _anim : Array, func : Function ) : void
	{
		for ( var x : int = 0, numItems : int = _anim.length; x < _anim.length; x++)
		{
			var animObj : Object = Object( _anim[x] );
			if (func.call( this, x, numItems, animObj ) == false)
				break;
		}
	}
	
	public function enumerateItems( _items : Array, func : Function ) : void
	{
		for ( var x : int = 0, numItems : int = _items.length; x < numItems; x++)
		{
			var itemObj : Object = _items[x];
			if (func.call( this, x, numItems, itemObj ) == false)
				break;
		}
	}
		
	public function setup() : void
	{
		_pages = new Array();
		for ( var pageNo : int = 0; pageNo < 1; pageNo++)
		{
			var _items : Array = new Array();
			for ( var itemNo : int = 0; itemNo < 2; itemNo++)
			{
				var _anim : Array = new Array();
				for ( var aItemNo : int = 0; aItemNo < 2; aItemNo++)
				{ 
					var _events : Array = new Array();
					for ( var eItemNo : int = 0; eItemNo < 2; eItemNo++)
					{ 
						_events.push( "[v:" + pageNo + " i:" + itemNo + " a: " + aItemNo + " e: " + eItemNo + "]")
					}
					_anim.push( _events );
				}
				_items.push( _anim );
			}
			_pages.push( _items );
		}
	}
	
	public function verifyContainer( container : Array ) : void
	{
		enumerateItems(container,
			function myContainerFunction( itemNo : int, numItems : int, items : Array ) : Boolean
			{
				var itemCount : int = 500; 
				enumerateAnimations(items,
					function myAnimationFunction( animNo : int, numItems : int, anim : Array ) : Boolean
					{
						var	animationDuration : int = 200; 
						enumerateEvents(anim,
							function myEventFunction( eventNo : int, numItems : int, event : Object ) : Boolean
							{
								trace( itemCount + " " + animationDuration + " " + event);
								return true;
							}
						);
						return true;
					}
				);
				return true;
			}
		);
	}
	
	public function goodResults() : void
	{
		for ( var pageNo : int = 0; pageNo < _pages.length; pageNo++)
		{
			var _items : Array = _pages[ pageNo ] as Array;
			for ( var itemNo : int = 0; itemNo < _items.length; itemNo++)
			{
				var itemCount : int = 500; 
				
				var _anim : Array = _items[ itemNo ] as Array;
				for ( var aItemNo : int = 0; aItemNo < _anim.length; aItemNo++)
				{ 
					var animationDuration : int = 200; 
		
					var _events : Array = _anim[ aItemNo ] as Array;
					for ( var eItemNo : int = 0; eItemNo < _events.length; eItemNo++)
					{ 
						trace( itemCount + " " + animationDuration + " " + _events[eItemNo] );
					}
				}
			}
		}
	}
		
	public function badResults() : void
	{
		for ( var index : int = 0; index < _pages.length; index++)
		{
			var container : Array = _pages[index] as Array;
			verifyContainer( container );
		}
	}
	
	public function Perkins()
	{
		setup(); 
	
		trace("Good Results" );
		// this path will print good results
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		goodResults();
	
		trace("Bad Results - first iteration - first event loop good, second loop bad" );
		// this path will print bad results
		// 500 200
		// 500 200
		// 500 200
		// 500 200
		// 500 59435417
		// 500 59435417
		// 500 59435417
		// 500 59435417
		badResults();
	
		trace("Bad Results - second iteration - both loops bad" );
		// this path will print worse results traversing the same structure
		// 2 500
		// 2 500
		// 2 500
		// 2 500
		// 2 500
		// 2 500
		// 2 500
		// 2 500
		badResults();
	}
}

}


var foo:Perkins = new Perkins();