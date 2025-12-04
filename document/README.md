Using the Waveshare library, there are... some things to address:
There is a fair amount of slightly amiguous or inaccurate documentation, but overall, the local pindef.md file contains the valid pin placements.

While I know it is against standard practice, it appears to be necessary to directly include the library's .cpp implementation files. Something about Arduino IDE's ^
project scope resolution refuses to compile them when only the .h files are referenced, maybe something to do with subdirectory traversal.

There is a persistent compiler warning about a method of the RGBmatrixPanel class, where it attempts to incorrectly evaluate a constant character array; I have yet to see this cause genuine issues.

The Adafruit library that this draws from defines multiple methods like start/endWrite(), updateDisplay(), some simple drawing tools, etc., that either failed to be overridden in the Waveshare ^
RGBmatrixPanel subclass (startWrite and endWrite have no implementation anywhere that I can see) or are handled largely internally (updateDisplay is called on clock cycles, DO NOT manually call it)

The matrix's qualified name is "RGB LED Matrix Panel 64x32 dots pixels 3 mm pitch"