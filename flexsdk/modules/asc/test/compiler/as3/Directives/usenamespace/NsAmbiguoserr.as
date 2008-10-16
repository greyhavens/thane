package {
     public namespace Football
     public namespace Baseball

     public class Game{
        Football var x =  "Joe"
        Baseball var x =  "Steve"

       use namespace Football

          function A(){return x}

       use namespace Baseball

          function B() {return x}

     }
}