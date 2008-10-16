
namespace Baseball;

class C
{

function getTeam() { return "Public"; }
Baseball function getTeam() { return "Giants"; } 

}

use namespace Baseball;
var s = new C();
print(s.Baseball::getTeam());

