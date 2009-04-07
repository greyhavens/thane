import avmplus.*;

		/****  inner class Node  ***/
		class Node 
		{
			var left:Node, right:Node;
			var i:int, j:int;
			function Node(l:Node = null, r:Node = null) { left = l; right = r; }
			
		}
		/**** end Node *******/

class GCBench
{
		var rootNode:Node;
		
		var tempTree:Node;
		var tStart:Number, tFinish:Number;
		var tElapsed:Number;

		static const kStretchTreeDepth:int    = 18;	// about 16Mb
		static const kLongLivedTreeDepth:int  = 16;  // about 4Mb
		static const kArraySize:int  = 500000;  // about 4Mb
		static const kMinTreeDepth:int = 4;
		static const kMaxTreeDepth:int = 16;

		static const bTrace:Boolean = false;

		function GCBench()
		{
//			msgBox = new TextField();
	//		msgBox.width = stage.width
		//	msgBox.height = stage.height
			if(bTrace) {
				trace("Garbage Collector Test");
				trace(" Stretching memory with a binary tree of depth " + kStretchTreeDepth);
			}
			tStart = System.getTimer();

			// Stretch the memory space quickly
			tempTree = MakeTree(kStretchTreeDepth);
			tempTree = null;

			// Create a long lived object
			if(bTrace) trace(" Creating a long-lived binary tree of depth " + kLongLivedTreeDepth);
			var longLivedTree:Node = new Node();
			Populate(kLongLivedTreeDepth, longLivedTree);

			// Create long-lived array, filling half of it
			if(bTrace) ttrace(" Creating a long-lived array of " + kArraySize + " doubles");
			var bcArray:Array = new Array();
			for (var bci:int = 0; bci < kArraySize/2; ++bci) 
			{
				bcArray[bci] = 1.0/bci;
			}

			for (var d:int = kMinTreeDepth; d <= kMaxTreeDepth; d += 2) 
			{
				TimeConstruction(d);
			}

			if (longLivedTree == null || bcArray[1000] != .001)
				trace("Failed");
						// fake reference to LongLivedTree
						// and array
						// to keep them from being optimized away

			tFinish = System.getTimer();
			tElapsed = tFinish-tStart;
			trace("metric time "+tElapsed);

		}

//	function trace(str)
	//{
		//msgBox.text += ( str + "\n");
//	}

	// Nodes used by a tree of a given size
	function TreeSize(ts:int):int
	{
	    	return ((1 << (ts + 1)) - 1);
	}

	// Number of iterations to use for a given tree depth
	function NumIters(ni:int):int
	{
			return Math.round(2 * TreeSize(kStretchTreeDepth) / TreeSize(ni));
	}

	// Build tree top down, assigning to older objects. 
	function Populate(iDepth:int, thisNode:Node):void
	{
		if (iDepth<=0) {
			return;
		} else {
			iDepth--;
			thisNode.left  = new Node();
			thisNode.right = new Node();
			Populate (iDepth, thisNode.left);
			Populate (iDepth, thisNode.right);
		}
	}

	// Build tree bottom-up
	function MakeTree(iDepth:int):Node
	{
		if (iDepth<=0) {
			return new Node();
		} else {
			return new Node(MakeTree(iDepth-1),
					MakeTree(iDepth-1));
		}
	}

	function TimeConstruction(depth:int):void 
	{
		var rootNode:Node;
		var tStart:Number, tFinish:Number;
		var iNumIters:int = NumIters(depth);
		var tempTree:Node;

		if(bTrace) trace("Creating " + iNumIters +
				   " trees of depth " + depth);
		tStart = System.getTimer();
		for (var i:int = 0; i < iNumIters; ++i) {
			tempTree = new Node();
			Populate(depth, tempTree);
			tempTree = null;
		}
		tFinish = System.getTimer();
		if(bTrace) trace("\tTop down construction took "
				   + (tFinish - tStart) + "msecs");
		tStart = System.getTimer();
		for (var itr:int = 0; itr < iNumIters; ++itr) {
				tempTree = MakeTree(depth);
				tempTree = null;
		}
		tFinish = System.getTimer();
		if(bTrace) trace("\tBottom up construction took " + (tFinish - tStart) + "msecs");
		
	}
}

startTest();
new GCBench();
AddTestCase("ran heap_stress", true, true);
test();
