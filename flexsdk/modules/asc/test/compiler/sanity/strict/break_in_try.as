print("break test");
for (var i=0; i<10; i++)
{
    try
    {
        if (i == 5)        
        {
            break;
        }
    }
    finally
    {
        print("i = " + i)
    }
}

print("continue test");
for (i=0; i<10; i++)
{
    try
    {
        if (i == 5)        
        {
            print("continue");
            continue;
        }    
    }
    finally
    {
        print("i = " + i)
    }
}

    
