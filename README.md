# Teller_Queues
A repository showcasing a program written to simulate a single- and multi-queue system. Formally, this program simulates an M/D/1 and M/D/k queue, where k is selected by the user. All algorithms and data structures used were implemented 'by-hand' and involve the use of templated classes.

## About

The program runs a discrete event simulation of customers arriving and being serviced by tellers. The program runs two simulations:
 1. A single-queue system where customers who arrive while a teller is busy are placed into a queue.
 2. A multi-queue system where a fixed number of queues (each with their own teller) are defined and customers are enqueued into the shortest queue.

At the end of the simulation, analysis is performed and outputted including total simulation time, average customer wait time, and maximum queue length.

## Data File

The input to the program is a file of arrival and service times. The first line in the file denotes the number of tellers to be used (in the multi-teller simulation).

**TODO:** The original data files for the program (except for one "stress-test" file `big`) were lost. Instead, a companion program which simulates data using techniques from [Queueing Theory](https://en.wikipedia.org/wiki/Queueing_theory) should be developed. Then simulation results can be compared with theoretical ones.

## Usage

Set the working directory to the root folder.

The simulation is built by using the assigned make file.

To construct the simulation, enter the following in the command window:

```
$ make Simulation
```

(note the upper-case 'S').

This will create the application file 'Simulation', which can be run by entering the command:

```
$ ./Simulation
```

(note the upper-case 'S').

Once the application has begun, enter the name of the source file, which should have been placed in the current working directory, and press enter. This will run both simulations and output the desired results.
