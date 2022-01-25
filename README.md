# C++ Calculator

This calculator uses all 4 operators and pi to solve basic mathematical
problems.

### How to run

Download the project and either compile the calculator-main.cpp file using
the following command:

```gcc -std=c++17 -o calc calculator-main.cpp```

Then you can run the program using:

```./calc```

### Improvements I made
- I added division to the calculator
- I added pi, so that the input can take in pi and in the code it is treated
as 3.1415
- I refactored the FindLHS and FindRHS functions so that it loops through
each symbol in the array, instead of using 4 if statements.