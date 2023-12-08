# joby-simulation-project

Simulates running 20 eVTOL simultaneously over a 3 hour period, with 3 chargers available. Can be run from the repository path with the command:
`make all && make run`

The simulation will randomize the 20 vehicle instances between 5 different companies (Alpha, Bravo, Charlie, Delta, Echo), each with different properties.

Here is an example of a printout:
```
./build\main.exe
Running eVTOL simulation with the following parameters: 
        Num eVTOLs:              20
        Num chargers:            3
        Simulation time (hours): 3
For 20 eVTOLs instantiated, the breakdown is as follows:
        Alpha:   4
        Bravo:   3
        Charlie: 4
        Delta:   4
        Echo:    5
Beginning Flight Sim, simulated duration: 3 hours

Completed 3 hour simulation!
Company Statistics:

Alpha Statistics:
        Avg. Flight Time:      1.695 hours
        Avg. Flight Distance:  207.398 miles
        Avg. Charging Time:    0.317083 hours
        Total Faults:          4
        Total Passenger Miles: 3318.37
        Avg. Waiting Time:     0.987917 hours

Bravo Statistics:
        Avg. Flight Time:      1.33333 hours
        Avg. Flight Distance:  137.498 miles
        Avg. Charging Time:    0.2 hours
        Total Faults:          0
        Total Passenger Miles: 2062.47
        Avg. Waiting Time:     1.46667 hours

Charlie Statistics:
        Avg. Flight Time:      1.25 hours
        Avg. Flight Distance:  211.996 miles
        Avg. Charging Time:    0.800001 hours
        Total Faults:          0
        Total Passenger Miles: 2543.95
        Avg. Waiting Time:     0.949999 hours

Delta Statistics:
        Avg. Flight Time:      1.98375 hours
        Avg. Flight Distance:  181.536 miles
        Avg. Charging Time:    0.34875 hours
        Total Faults:          4
        Total Passenger Miles: 1452.29
        Avg. Waiting Time:     0.6675 hours

Echo Statistics:
        Avg. Flight Time:      1.69767 hours
        Avg. Flight Distance:  155.788 miles
        Avg. Charging Time:    0.124 hours
        Total Faults:          5
        Total Passenger Miles: 1557.88
        Avg. Waiting Time:     1.17833 hours

Executing run: all complete!
```
