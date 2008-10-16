package {
    public class bug  {
      private var notSent:Boolean= true;
      public function _bug() {
        try {
        } catch (e:Error) {
        }
      }
    }
    new bug()._bug()
}
