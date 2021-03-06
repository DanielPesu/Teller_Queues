#include "teller.h"
Teller::Teller()
{
  idle_ = true;
  idle_time_ = 0.0;
  begin_idle_ = 0.0;
  customers_served_ = 0;
  service_time_ = 0.0;
}

/*******************************************************************************
  Destructor
  Teller is responsible for closing its queue.
*******************************************************************************/
Teller::~Teller()
{

}

void Teller::setIdle(double time_stamp)
{
  idle_ = true;
  begin_idle_ = time_stamp;
}

/*******************************************************************************
  Serve Customer
  The teller serves a passed customer.
  Since customers are dynamically allocated, and are no longer needed beyond
  this point, they are deleted once the teller's finish time has been
  calculated.
  Returns the time at which the teller will finish serving the customer.
*******************************************************************************/
double Teller::serveCustomer(double time_stamp, Customer* cust)
{
//std::cout << "SERVING A CUSTOMER" << std::endl;
  double finish_time = time_stamp + (*cust).service_time;
//std::cout << "A" << std::endl;
  idle_ = false;

  idle_time_ += time_stamp - begin_idle_;  // <-- record time spent being idle.
  service_time_ += cust->service_time;

  customers_served_++;
  
  delete cust;
//std::cout << "FINISH TIME " << finish_time << std::endl;
  return finish_time;  // Return teller's finish time.
}
