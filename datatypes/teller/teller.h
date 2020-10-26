/*******************************************************************************
   File:   teller.h                                                            *
   Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                          *
   Ass.:   CSCI203, Assignment 2                                               *
   About:  This file holds the definitions for the teller datatype used in     *
           the simulations. All datatypes are stored in the datatype           *
           namespace.                                                          *
                                                                               *
   Last Modified: 07/09/16.                                                    *
*******************************************************************************/
#ifndef _TELLER_H_
#define _TELLER_H_
#include "../customer/customer.h"        // Customer struct
#include "../../datastructures/heap/heap.h"   // Templated Heap class
#include "../../datastructures/queue/queue.h"  // Templated Queue class
using namespace datastructures;
using namespace datatypes;

/* TELLER NEEDS TO DELETE THE QUEUE!!! */
namespace datatypes
{
  /*****************************************************************************
    Teller Class.                                                              *
    This class holds all relevant data for describing a Teller.                *
  *****************************************************************************/
  class Teller {
   public:
    Teller();
    ~Teller();

    void setIdle(double time_stamp);

    bool   isIdle() { return idle_; }
    double serveCustomer(double time_stamp, Customer* cust);

    int customerCount() const { return customers_served_; }
    double timeIdle() const { return idle_time_; } 
    double serviceTime() const { return service_time_; }

   private:
    bool   idle_;              // True if the teller is not currently serving a customer.
    double idle_time_;         // Holds the time the teller has spent idle.
    double begin_idle_;        // Holds the time stamp at which the teller started being idle.
    int    customers_served_;  // Holds the total number of customers successfully served by the teller.

    double service_time_;
  };
}

#endif  // _Teller_H_
