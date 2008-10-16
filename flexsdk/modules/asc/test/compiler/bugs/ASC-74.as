function badSwitch():void{
    if (true)
        return;
    
    switch (1){
        case 1:
        return;
    }
}

badSwitch()