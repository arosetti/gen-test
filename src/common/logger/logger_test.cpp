#include <iostream>
#include <string>

#include "logger.h"

using namespace std;

logger *logger::l_singleton = NULL;

int main ()
{
  for(int i = 0 ; i<10; i++)
      LOG("logger.log", "***log***", true);

  delete LOG_PTR();

  return 0;
}
