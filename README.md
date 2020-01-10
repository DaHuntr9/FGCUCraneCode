# FGCUCraneCode
This is the code supporting the mock crane in the networking lab as of Fall 2019. 
<br>
The following is the Arduino code for FGCU's mock crane in the networking lab. This crane has had some edits from the previous iterations.
<br>
This project is in relation to a repository by carayes17. His software is for the server that hosts a webpage that sends commands to the serial listener in this repository. 
<br>
https://github.com/careyes17/RemoteCTRL

## Notes 
-The crane now uses continous servos so the arduino code sets the speed of the servos.
<br>
-The winch is not bound to a solid attachment so it will fall off over time. 
<br>
-The cranes wires will bind up after a specific number of rotations and will break its connections ripping the wires out of its connections. 

## Future Ideas
Create limitations for the cranes range of motion so that it will not break itself.
