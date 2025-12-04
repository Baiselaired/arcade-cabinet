# arcade-cabinet
This is the current state of the game program I wrote for the University of Michigan's CIT-100 course final project.

Be it a fault in the Adafruit library or the wrapper around it provided with the matrix product or the Arduino IDE itself, I've not found a way to keep dependencies in a subdirectory, they will only work if every necessary file is in the local directory despite direct references with relative pathing. So, while ugly, this seems to be a necessary evil.

I am by no means intending to be finished with this project. A list of the current intended features to be added:
- Enemies (multiple types, maybe, a boss at every score interval)
- Varied bullet velocities -- faster/slower and directional dependent on player volecity
- Powerups of some sorts
- A sprint activated by the joystick button, with an associated sprint bar
- A score display on screen
- Limits to player fire rate
- Implementations of random generation
- Some variety to sounds -- spawns, deaths, etc.
- Actual music playback, not sure if that would require only a speaker or another module
