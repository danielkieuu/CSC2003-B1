## Members

| Github Handle                                             | Student ID |
| --------------------------------------------------------- | ---------- |
| <a href="https://github.com/zyferis/">Derick Kwok</a>     | 2100689    |
| <a href="https://github.com/Matahhhhh/">Reagan Chia</a>   | 2102539    |
| <a href="https://github.com/NgChaoHong/">Ng Chao Hong</a> | 2100997    |

## Mapping Files

There is 2 files under mapping called 
1. `mapping.c`
2. `mappingSimulation.c`

`mapping.c` is fully integrated with PID and Ultrasonic for the Movement as well as setup for Communications to pass `x` and `y` coordinates for the car to traverse.

`mappingSimulation.c` removes some functionality of PID and ultrasonic sensor and changes the movement logic such that it will work with testing data without needing to actually move any hardware. 

## Running The Code

### mapping.c

Simply call `Map()` and and the code will automatically start moving to unexplored neighbours and generating new nodes to traverse until the maze is completely mapped. 

### mappingSimulation.c 

Running the file will print out your `current location` as well as the test data as a `map`
Key in the `x` and `y` values you want to traverse to one after another
The console will print out the steps taken to traverse to the node and repeat the loop

> Note that there is a wall that 
> exists between nodes 0 and 3 
> that is printed out on the map

## Mapping Algorithm 

Our final implementation of the code uses an A* algorithm with heuristics being calculated by a modified manhattan method. 
In an effort to save on processing power the `(heurestic * 10)` was removed from the manhattan method.
We have chosen A* to find the shortest path to our goal node in an attempt to reduce the amount of time needed for traveling and mapping out the entire maze. 

Nodes are not generated at compile time to save on memory.
The node generation of the map is fully dynamic and can be scaled up by changing the variable `NUMOFNODESTODECLARE` to increase the array to store the dynamically generated nodes. 


## Memory Used

Each `Node` object takes up `72 Bytes`
Each `Pointer` takes up `8 Bytes`

In an ideal scenario the total amount of memory used by the map for this specific arena will be 
`20 Nodes` + `30 pointer addresses` = `1680 Bytes`

An array of size 30 is declared to store the `Node` addresses which is more than the maximum needed for this scenario as a failsafe and to make the program more dynamic in the event that the arena is widened. 

## Mapping Flowchart

Flowchart for the mapping [Click Here](https://viewer.diagrams.net/?tags=%7B%7D&highlight=0000ff&edit=_blank&layers=1&nav=1&title=Flowchart#Uhttps%3A%2F%2Fdrive.google.com%2Fuc%3Fid%3D1_q26IFHSNsQXYZy0HzrzaDjJYhZwIG8D%26export%3Ddownload)

## Mapping Video

Video explanation for Mapping [Click Here](https://youtu.be/E1R4I-0ntPA)

## Limitations 

The Mapping assumes that it starts with the back of the car facing a wall. The mapping will run normally if it does not start with the back facing a wall. However, the node behind the car will not be mapped unless there is a seperate node connecting to it. 