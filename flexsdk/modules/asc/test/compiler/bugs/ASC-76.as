function run():void
{
    try
    {
        //trace("Try");
    }
    catch (e:TypeError)
    {
        return;
    }
    catch (e2:Error)
    {
        return;
    }
    finally
    {
        //trace("Finally!");
    }
    //trace("Done!");
}
run();