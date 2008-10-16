

L1: 
while(true) { 
L2: L1: while(false) { continue L1; break L2; } break L1; continue; 
}
L1: 
while(true) { 
L2: while(false) { continue L1; break L2; } break L1; continue; 
}
L1:
{
    break L1
}
print(true)