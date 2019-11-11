/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
   
   Demonstrates the minimal amount of code to take a PAR measurement.
 */

#include <uFire_PAR.h>

uFire_PAR par;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println((String)par.measurePAR() + " μmol/m²/s");
  delay(1000);
}
