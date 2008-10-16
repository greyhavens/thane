LabeledContinue( 5, 5 );

function LabeledContinue( limit, expect ) {
i = 0;
j=0;
woohoo:
while(j<4){
j++;
print("J is :"+j);

while(i<limit) {
i++;
continue woohoo;
}
print("i is :"+i);
}

}