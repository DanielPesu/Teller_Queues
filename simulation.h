/*******************************************************************************
   File:   customer.h                                                          *
   Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                          *
   Ass.:   CSCI203, Assignment 2                                               *
   About:  This file holds the definition of the Simulation class. This class  *
           implements the degned CircularBuffer, Queue, and Heap classes.      *
                                                                               *
   Last Modified: 09/09/16.                                                    *
*******************************************************************************/
#ifndef _SIMUATION_H_
#define _SIMULATION_H_
#include "./datastructures/heap/heap.h"     // Templated Heap class
#include "./datastructures/queue/queue.h"   // Templated Queue class
#include "./datatypes/teller/teller.h"      // Teller class
#include "./datatypes/customer/customer.h"  // Customer struct
#include <fstream>                          // ifstream.
using namespace std;
using namespace datatypes;
using namespace datastructures;

// Identifies the type of simulation being run.
enum Simulation_Type { SINGLE_QUEUE,
                       INDEPENDENT_QUEUES
};

// Identifier for the type event is being processed.
enum Event_Type { CUSTOMER_ARRIVAL,  // Indicates a customer has arrived.
                  TELLER_FINISH      // Indicates a teller has finished serving a customer.
};

/*******************************************************************************
  Event                                                                        *
  Stores key data about an event.                                              *
  An event is considered '<' another event if it occurrs sooner.               *
*******************************************************************************/
struct Event {
  Event_Type event_type;      // The type of the event which has occured.
  double     time_stamp;      // The time at which the event occurs.

  Teller*    teller_ref;      // Refers to the Teller associated with the FINISH event.
  Customer*  customer_ref;    // Refers to the Customer assocciated with the ARRIVAL event.

  friend bool operator<(const Event& lhs, const Event& rhs) // Determines which event occurs sooner.
  {
    return lhs.time_stamp < rhs.time_stamp;
  }

  friend bool operator>(const Event& lhs, const Event& rhs)
  {
    return lhs.time_stamp > rhs.time_stamp;
  }

  friend std::ostream& operator<<(std::ostream& out, const Event& e)
  {
    out << "Event(t=" << e.time_stamp << ", event_type=";
    if (e.event_type == CUSTOMER_ARRIVAL)
      out << "CUSTOMER_ARRIVAL";
    else
      out << "TELLER_FINISH";
    out << ")" << std::endl;
    return out;
  }
};

/*******************************************************************************
  Simulation Class                                                             *
  This class handles all operations with the simulation.                       *
*******************************************************************************/
class Simulation {
 public:
  Simulation(Simulation_Type sim_type);
  ~Simulation();

  void Run();

  bool Initialise(const char fname[]);
  bool NextEvent(Event& e);
  void ProccessArrival(Customer* cust);
  void ProccessTellerFinish(Teller* tell);
  int NextAvailableTeller();

  bool eventsRemaining();
  void Analyse(std::ostream& out);
  Customer* ReadCustomer();

 private:
  Simulation_Type sim_type_;
  double system_time_;
  ifstream arrival_times_; // Links to the file of customer arrivals.  TODO change to sim_file_

  // The tellers_ array and teller_queues_ array are stored in parallel for
  // simulations with multiple queues.
  int num_tellers_;
  Teller* tellers_;                 // Array of tellers
  Queue<Customer*>* teller_queues_; // Array of queues to tellers
  Heap<Event> events_;              // Stores the order of events.

  int* queue_lengths_;        // Stores the maximum queue lengths for each queue.
  double total_wait_time_;    // Stores the total time customers spend waiting in the queue.
  double maximum_wait_time_;  // Stores the maximum time a customer spends waiting.

  double* queue_data_;  // Stores the running average of queue lengths for each queue.
  double* previous_entry_time_;  // Stores the time the queue previously changed.

  void recordQueueChange(int queue_index, int queue_length);
};
#endif
