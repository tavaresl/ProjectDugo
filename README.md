# Conway's Container

> Made for OLC Codejam 2020

Fitting the theme "The Great Machine", in this game you take control of the rules of John Conway's Game of Life.
Your goal is to save as many populations as possible before the countdown ends.

Even though we did not have any previous experience with C++, we decided to use it to develop the game so we could try the `olcPixelGameEngine` as well.

We hope you like it :)

## How it works

We change some rules of the game on the run, but first remembering the original rules of Conway's Game of Life:

> Any live cell with two or three live neighbours survives.<br />
> Any dead cell with three live neighbours becomes a live cell.<br />
> All other live cells die in the next generation. Similarly, all other dead cells stay dead.<br />

So, we changed some rules to make it Controllable.

## Movementation

Let's interpret the cell and interactions as this following visual

OOO<br />
O@O<br />
OOO<br />

@ = Alive Cell<br />
O = Identifying neighbours

In the case above, all neighbours is interpreted, that means the cell will grow to all 4 sides (if possible). So to make it goes Right for example, we removed the Left collumn identification, being like this:

&OO<br />
&@O<br />
&OO<br />


& = Not Identifying neighbours

In this way, will only grow to the right. But this brings an outcome. The cells will die easier.

## Less Deaths

In the original rules, you will need 3 alive neighbours to create a life. <br />
So, to create more lifes, we thought that 2 neighbours to create a life should be enough to create more life than before.


## Changing Neighbours N values (More Deaths)

In the rule *Any live cell with 2 or 3 live neighbours survives*, and you can change that rule by adding the minimum value to survive to 3 or lowering the maximum value to 2, causing more deaths.

## Identifying more Neighbours

The neighbours are identified as the next of the cell, like this:

OOO<br />
O@O<br />
OOO<br />

But if you whant to identify more neighbours, you can use to be in this way:

OOOOO<br />
OOOOO<br />
OO@OO<br />
OOOOO<br />
OOOOO<br />

You can imagine that the rules of "Need 3 neighbours to be alive" can be easily made or disturbed by this change of rule. But what it causes is that life is more spaced and occupies more of the screen.
