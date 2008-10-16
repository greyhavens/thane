// switch statement should get removed as unreachable code,
// otherwise we get a VerifyError
function badSwitch():void
{
    if(true)
        return;
    
    switch (1)
    {
        case 1:
            break;
    }
}

badSwitch();