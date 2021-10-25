#pragma once

#define MODULE(KIND, HEADER_FILE) MODULE_(KIND, HEADER_FILE)
#define MODULE_(KIND, HEADER_FILE) TO_STRING_(KIND ## _ ## HEADER_FILE)
#define TO_STRING_(X) #X

//#include MODULE(FRONTEND, frontend)
#include MODULE(OPTIMIZER, optimizer)
#include MODULE(BACKEND, backend)
#include MODULE(TARGET, target)