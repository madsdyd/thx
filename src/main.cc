#include <stdlib.h>
#include <unistd.h>

#include "client.hh"

/* Our client main */
int main(int argc, char** argv) {
  /* Init random */
  srand(getpid()*time(NULL));
      
  Client = new TClient(argc, argv);

  /* This never returns */
  Client->Run();

  return 0;
}
