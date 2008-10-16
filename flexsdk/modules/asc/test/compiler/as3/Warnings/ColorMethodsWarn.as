/*******************************************************************************************************************Coach Warning for Color Class Methods ******************************
*******************************************************************************************/

package Warnings{
    import flash.geom.ColorTransform;
    import flash.display.MovieClip;
    import flash.util.trace;
    
    var my_mc:MovieClip = new MovieClip();
    var my_color:ColorTransform = new ColorTransform(my_mc);

    my_color.setRGB(0xff9933);
    var my_value:String = my_color.getRGB().toString(16);
    trace(my_value);

    var myTransform:Object = my_color.getTransform();
    myTransform = {ra:"50",ba:"50",aa:"30"}
    my_color.setTransform(my_Transform);
    
    
}