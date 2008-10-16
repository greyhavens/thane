

L1: 
do { 
   L1: while(false) { continue L1; break; }; break L1; continue; 
} while ( true )

L2:
{
   break;
}

L3:
{
   continue L3;
   continue;
}