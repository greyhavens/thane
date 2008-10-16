// Should be no verify errors with these methods.
function run()
{
    try
    {
        trace("Try");
    }
    catch (e)
    {
        return;
    }
    finally
    {
        trace("Finally!");
    }
}

function run2():void
{
    try
    {
        trace("Try");
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
        trace("Finally!");
    }
    trace("Done!");
}
run();
run2();