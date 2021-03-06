#include "simulation.h"
#include <iostream>
#include <iomanip>
/*******************************************************************************
  Constructor                                                                  *
*******************************************************************************/
Simulation::Simulation(Simulation_Type sim_type)
{
  sim_type_ = sim_type;
  system_time_ = total_wait_time_ = maximum_wait_time_ = 0.0;
  num_tellers_ = 0;
  queue_lengths_ = NULL;
  queue_data_ = previous_entry_time_ = NULL;
}

/*******************************************************************************
  Destructor                                                                   *
*******************************************************************************/
Simulation::~Simulation()
{
  if (num_tellers_ > 0)
  {
    delete [] tellers_;
  }
  if (queue_lengths_ != NULL)
  {
    delete [] queue_lengths_;
    delete [] queue_data_;
    delete [] previous_entry_time_;
  }
}

/*******************************************************************************
  Run                                                                          *
  Runs the entire simulation.                                                  *
*******************************************************************************/
void Simulation::Run()
{
  Event e;
  while (eventsRemaining())
  {
    NextEvent(e);
    if (e.event_type == CUSTOMER_ARRIVAL)
      ProccessArrival(e.customer_ref);
    else
      ProccessTellerFinish(e.teller_ref);
  }
}

/*******************************************************************************
  Initialise                                                                   *
  Creates the Heap, teller(s), and associated queue(s).                        *
  Also inserts the first customer arrival event onto the heap.                 *
  Returns false if the data file could not be found or is empty.               *
*******************************************************************************/
bool Simulation::Initialise(const char fname[])
{
  arrival_times_.open(fname);
  if (!arrival_times_)
    return false;

  arrival_times_ >> num_tellers_;
  tellers_ = new Teller[num_tellers_];

  if (sim_type_ == SINGLE_QUEUE)
  {
    teller_queues_ = new Queue<Customer*>;
    queue_lengths_ = new int;
    queue_data_ = new double;
    previous_entry_time_ = new double;

    *queue_lengths_ = 0;
    *queue_data_ = *previous_entry_time_ = 0.0;
    
  }
  else
  {
    teller_queues_ = new Queue<Customer*>[num_tellers_];
    queue_lengths_ = new int[num_tellers_];
    queue_data_ = new double[num_tellers_];
    previous_entry_time_ = new double[num_tellers_];

    for (int i = 0; i < num_tellers_; ++i)
    {
      queue_lengths_[i] = 0;
      queue_data_[i] = previous_entry_time_[i] = 0.0;
    }
  }


  Customer* cust = ReadCustomer();
  if (cust != NULL)
  {
  Event first_arrival = {CUSTOMER_ARRIVAL, (*cust).arrival, NULL, cust};
  events_.Insert(first_arrival);
  }
  else
    return false;

return true;
}

/*******************************************************************************
  Next Event                                         Time Complexity: O(log n) *
  Pulls the next event from the heap and adjusts system time to the event      *
  time.                                                                        *
*******************************************************************************/
bool Simulation::NextEvent(Event& e)
{
  if (!events_.isEmpty())
  {
    e = events_.Delete(events_.Top());
    system_time_ = e.time_stamp;
    return true;
  }
  else
    return false;

}

/*******************************************************************************
  Proccess Arrival                                                             *
  Proccesses a customer arrival event by either immediately serving the        *
  customer or enqueueing it in the shortest queue.                             *
  The shortest queue is identified by iterating over all the queues once. It   *
  would be more efficient to identify the shortest queue by storing references *
  to queues in a heap structure, however, this could not be implemented in     *
  time.                                                                        *
*******************************************************************************/
void Simulation::ProccessArrival(Customer* cust)
{
  double teller_finish_time = 0.0;
  int free_teller = NextAvailableTeller();

  if (free_teller == num_tellers_)
  {
    if (sim_type_ == SINGLE_QUEUE)
    {
      recordQueueChange(0, teller_queues_->Length());
      teller_queues_->Enqueue(cust);
    }
    else
    {
      int smallest_index = 0;
      for (int teller_num = 1; teller_num < num_tellers_; ++teller_num)
      {
        if (teller_queues_[teller_num].Length() < teller_queues_[smallest_index].Length())
          smallest_index = teller_num;
      }
      recordQueueChange(smallest_index, teller_queues_[smallest_index].Length());
      teller_queues_[smallest_index].Enqueue(cust);

    }
  }
  else
  {
    teller_finish_time = tellers_[free_teller].serveCustomer(system_time_, cust);
    Event e  = {TELLER_FINISH, teller_finish_time, (tellers_ + free_teller), NULL};
    events_.Insert(e);
  }

  Customer* next_cust = ReadCustomer();
  if (next_cust != NULL)
  {
    Event e = {CUSTOMER_ARRIVAL, next_cust->arrival, NULL, next_cust};
    events_.Insert(e);
  }
}

/*******************************************************************************
  Proccess Teller Finish                                                       *
  Proccesses a teller finish event. If there are no more customers for the     *
  teller to serve then it is switched to and idle state.                       *
*******************************************************************************/
void Simulation::ProccessTellerFinish(Teller* tell)
{
  int queue_index;
  if (sim_type_ == SINGLE_QUEUE)
   queue_index = 0;
  else
    queue_index = tell-tellers_;

  if (teller_queues_[queue_index].isEmpty())
    tell->setIdle(system_time_);
  else
  {
    double finish_time = 0.0;
    recordQueueChange(queue_index, teller_queues_[queue_index].Length());
    Customer* cust = teller_queues_[queue_index].Dequeue();

    total_wait_time_ += system_time_ - cust->arrival;
    if (maximum_wait_time_ < (system_time_ - cust->arrival))
    {
      maximum_wait_time_ = (system_time_ - cust->arrival);
    }
    finish_time = tell->serveCustomer(system_time_, cust);

    Event e = {TELLER_FINISH, finish_time, tell, NULL};
    events_.Insert(e);    
  }
}

/*******************************************************************************
  Next Available Teller                                  Time Complexity: O(n) *
  Returns the index of the last teller + 1 if there are no free tellers.       *
  Otherwise, returns the index of the first available teller.                  *
*******************************************************************************/
int Simulation::NextAvailableTeller()
{
  int teller_index = 0;
  while (tellers_[teller_index].isIdle() == false && teller_index != num_tellers_)
   ++teller_index;

  return teller_index;
}

/*******************************************************************************
  Events Remaining                                       Time Complexity: O(1) *
  Returns true if there are still events to be proccessed.                     *
*******************************************************************************/
bool Simulation::eventsRemaining()
{
  if (events_.isEmpty())
    return false;
  else
    return true;
}

/*******************************************************************************
  Analyse                                                                      *
  Outputs the gathered statistics from the simulation to a given output.       *
*******************************************************************************/
void Simulation::Analyse(std::ostream& out)
{
  out << "\n\tANALYSIS:\t\t";
  if (sim_type_ == SINGLE_QUEUE)
    out << "Single Queue" << std::endl;
  else
    out << "Multiple Queues" << std::endl;

  out << "-----------------------------------------------------" << std::endl;
  out << "  Simulation Terminated:\t\tt = " << setprecision(2) << fixed << system_time_ << std::endl;

  int total_customers = 0;
  double total_idle_time = 0.0, total_service_time = 0.0;

  for (int i = 0; i < num_tellers_; ++i)
  {
    total_customers += tellers_[i].customerCount();
    total_idle_time += tellers_[i].timeIdle();
    total_service_time += tellers_[i].serviceTime();
  }

  out << "  Total Customers Served:\t\t" << total_customers << std::endl;
  out << "  Total Teller Idle Time:\t\t" << total_idle_time << std::endl;
  out << "  Average Service Time:\t\t\t" << total_service_time / total_customers << std::endl;
  out << "  Average Wait Time:\t\t\t" << total_wait_time_ / total_customers << std::endl;
  out << "  Maxiumum Wait Time:\t\t\t" << maximum_wait_time_ << std::endl;
  if (sim_type_ == SINGLE_QUEUE)
  {
    out << "  Maximum Queue Length:\t\t\t" << *queue_lengths_ << std::endl;
    out << "  Average Queue Length:\t\t\t" << *queue_data_/system_time_ << std::endl;
  }
  else
  {
    int max_length = 0;
    double grand_average = 0.0;
    out << "  Average & Maximum Queue Lengths:" << std::endl;
    for (int i = 0; i < num_tellers_; ++i)
    {
      out << "    Teller " << i+1 << "\t\t\t\t" << queue_data_[i]/system_time_ <<  "  (" << queue_lengths_[i] << ")" << std::endl;
      if (queue_lengths_[i] > max_length)
        max_length = queue_lengths_[i];
        grand_average += queue_data_[i]/system_time_;
    }
    out << "    Overall:\t\t\t\t" << grand_average/num_tellers_ << "  (" << max_length << ")" << std::endl;
  }

  out << "-----------------------------------------------------" << std::endl;
}

/*******************************************************************************
  Read Customer                                                                *
  Reads the next customer from the file.                                       *
  Customers are created dynamically with their pointers being passed around    *
  the simulation.                                                              *
  If there are no more customers in the file, the function returns NULL.       *
  A Customer is deleted when a teller begins serving it.                       *
    See: Teller.serveCustomer().                                               *
*******************************************************************************/
Customer* Simulation::ReadCustomer()
{
  double time = 0.0;
  Customer* next_cust = new Customer;

  arrival_times_ >> time;

  if (!arrival_times_.eof())
  {
    next_cust->arrival = time;
    arrival_times_ >> next_cust->service_time;
  }
  else
  {
    delete next_cust;
    arrival_times_.close();
    next_cust = NULL;
  }

  return next_cust;
}

/*******************************************************************************
  Record Queue Change                                                          *
  This function is called whenever a change in queue length occurs and is used *
  to record queue-based statistics such as average queue length and maximum    *
  queue length.                                                                *
*******************************************************************************/
void Simulation::recordQueueChange(int queue_index, int queue_length)
{
  if (queue_lengths_[queue_index] < queue_length)
    queue_lengths_[queue_index] = queue_length;

  queue_data_[queue_index] += (system_time_ - previous_entry_time_[queue_index]) * queue_length;
  previous_entry_time_[queue_index] = system_time_;
}
