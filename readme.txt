knightt is a spaceship search program by Tim Coe.  It is designed to
investigate the complete spaceship tree for spaceships of a given speed,
period, width, and symmetry.  For any (2*PERIOD + 1) phases of a row that
is part of a ship of the given type the search outputs the shortest ship
containing those phases.  A complete search gives a set of spaceships that
could be used to construct any possible spaceship of that type and width.

This collection contains the output of completed knightt searches.  A small
number of results from incomplete searches are also included.

The files are presented in gzipped RLE format and can be read directly by
the latest version of Golly.  The collection is sorted by speed.  File
names indicate the speed, width, and symmetry of the search.  The symmetry
codes are as follows:

n = asymmetry
o = odd-width bilateral symmetry
e = even-width bilateral symmetry
g = gutter symmetry (odd bilateral symmetry with an empty central column)

knightt currently supports orthogonal spaceships of speeds c/N and
Nc/(2N + 1).  The table below contains the largest width that has been
completely searched for types up to period 9.  The table gives the largest
"logical" width rather than the full width.  For example, a 9 in the odd
column represents bilaterally symmetric ships with a total width of 17.

 Speed |          Symmetry
       | asym |  odd | even | gutter 
-------+------+------+------+--------
  c/3  |  13  |  10  |  10  |   10
  c/4  |  11  |   9  |   9  |    9
  c/5  |  10  |   9  |   9  |    9
 2c/5  |  12  |   9  |  10  |   10
  c/6  |  11  |   9  |   9  |    9
  c/7  |   9  |   9  |  10  |    9
 3c/7  |  14  |  14  |  14  |   14
  c/8  |  10  |   9  |   9  |    9
  c/9  |   8  |   8  |   8  |    8
 4c/9  |  11  |  11  |  11  |   11

--
Matthias Merzenich