# OOP-Lab-Project (L2-3)
<br>
At the time of editing this file, we are facing <b>2 tasks</b>:<br>
<b>1.</b> (L2) Creation of <b>10 classes</b> (taking into account <b>encapsulation</b>) that will <u>represent actors in a simulation</u>;<br>
<b>2.</b> (L3) Create relatively <b>complex inheritance chains<b> that will contain examples of <b>two-level inheritance</b> and <b>multiple inheritance</b>. 
<br>
I chose a simulation somewhat similar to <b>a tower defense</b>, but with some more complex mechanics.<br>
<b>The purpose of the game</b>: <b>to prevent opponents</b> (within the lore: wolves, werewolves, etc.) <b>from<br> 
reaching the goal located in any place on the map (9 * 9 at the moment)</b> (within the lore - a house). Unlike a traditional tower<br> 
defenses in this simulation: the opponents themselves find the route based on the structure of the playing field;<br> 
most defensive units are designed to be installed directly in the path of movement and not on the side of the road.
<br>
<b>Realisation</b>:
<br>
![Class Diagram](class_diagram.jpg)
<br>
I have created <b>14 classes</b> organized in the above structure.<br>
All classes can be defined in <b>3 groups (+ superclass)</b>:
<b>1.</b><i>'Actors'</i>: entities inside the simulation in which <b>the parameters</b> of the creatures and <b>the methods</b><br>
by which they interact with the game and with each other are declared.
<b>2.</b><i>'Properties'</i>: These are abstract classes in which <b>parameters and some methods are embedded</b>,<br>
which will be used by 'actors' as a result of <b>inheritance</b>.
<b>3.</b><i>'Game functionality'</i>: These classes declare methods for working with the console, <b>game constants</b>,<br>
<b>statistics</b>, and so on.
