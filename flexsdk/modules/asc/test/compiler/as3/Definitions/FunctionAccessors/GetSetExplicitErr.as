/**
 *  Class GetSetExplicit
 *	
 *
 *  Author: Vera Fleischer (vfleisch@macromedia.com)
 *  Date: 07/19/2005
 *  Moving to Errors folder as it should throw compiler error since explicit attribute is
 *  allowed only for package top level definitions
 *  Modified:  Subha Subramanian(ssubrama@macromedia.com)	
 *  Date: 10/14/2005
 *  
 */

package Errors {

	public class GetSetExplicit {

		private var _nt = "no type";
		private var _x:Array = [1,2,3];
		private var _y:int = -10;
		private var _b:Boolean = true;
		private var _u:uint = 1;
		private var _s:String = "myString";
		private var _n:Number = 5;
		
		explicit function get x():Array {
			return _x;
		}
		explicit function set x( a:Array ) {
			_x=a;
		}
		explicit function get y():int {
			return _y;
		}
		explicit function set y( i:int ) {
			_y=i;
		}
	        explicit function get boolean():Boolean { return _b; }
		explicit function set boolean(b:Boolean) { _b=b; }
		explicit function get u():uint{ return _u; }
		explicit function set u(ui:uint) { _u=ui; }
		explicit function get string():String{ return _s; }
		explicit function set string(s:String) { _s=s; }
		explicit function get noType():String{ return _nt; }
		explicit function set noType(nt) { _nt=nt; }

		// call setter from setter
		private var _sfs1:Number = 99;
		private var _sfs2:int = 0; 
		explicit function get sfs1():Number{ return _sfs1; }
		explicit function get sfs2():int{ return _sfs2; }
		explicit function set sfs1(n:Number){
			_sfs1 = n;
		}
		explicit function set sfs2(i:int){
			this.sfs1 = i;
			_sfs2 = i;
		}

		// call setter from getter
		private var _sfg1:String = "FAILED";
		private var _sfg2:uint = 0; 
		explicit function get sfg1():String{ return _sfg1; }
		explicit function get sfg2():uint{
			sfg1 = "PASSED";
			return _sfg2;
		}
		explicit function set sfg1(s:String){ _sfg1 = s; }
		explicit function set sfg2(ui:uint){ _sfg2 = ui; }
		
		// call getter from setter
		private var _gfs1:String = "FAILED";
		private var _gfs2:String = "PASSED"; 
		explicit function get gfs1():String{
			return _gfs1;
		}
		explicit function get gfs2():String{
			return _gfs2;
		}
		explicit function set gfs1(s:String){
			_gfs1=gfs2;
		}
		explicit function set gfs2(s:String){
			_gfs2=s;
		}

		// call getter from getter
		private var _gfg1:String = "PASSED";
		private var _gfg2:String = "FAILED"; 
		explicit function get gfg1():String{
			return _gfg1;
		}
		explicit function get gfg2():String{
			return gfg1;
		}

		// define a getter for a property and call the undefined setter
		private var _nosetter = "FAILED";
		explicit function get noSetter(){ return _nosetter; }

		// define a setter for a property and call the undefined getter
		private var _nogetter = "FAILED";
		explicit function set noGetter(s){ _nogetter = s; }

	}

}
