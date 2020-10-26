/*******************************************************************************
   File:   customer.h                                                          *
   Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                          *
   Ass.:   CSCI203, Assignment 2                                               *
   About:  This file holds the definitions for the customer datatype used in   *
           the simulations. All datatypes are stored in the datatype           *
           namespace.                                                          *
                                                                               *
   Last Modified: 22/08/16.                                                    *
*******************************************************************************/
#ifndef _DATATYPES_H_
#define _DATATYPES_H_

namespace datatypes {
  /*****************************************************************************
    Customer Struct.                                                           *
    This class holds all relevant data for describing a Customer.              *
    If a customer is to be enqueued, then their time spent waiting is stored.  *
  *****************************************************************************/
  struct Customer {
    double arrival;       // time arrived.
    double service_time;  // time to serve customer.
  };
}

#endif  // _DATATYPES_H_
