/* 

   Error: 1138 Required parameters not permitted after optional parameters 

*/

package
{
    public class OptParamAfterReqParamError
    {
	public function OptionalParametersErr( myString:String='string',myint:int )
	{
	}
		
        OptionalParametersErr();
     }
}