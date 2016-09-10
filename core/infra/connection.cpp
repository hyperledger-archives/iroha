#include <string>
#include <functional>
#include <cstdint>
#include <cstdio>
#include <signal.h>

#include <util/CommandOptionParser.h>
#include <thread>
#include <Aeron.h>
#include <array>
#include <concurrent/BusySpinIdleStrategy.h>

#include <unordered_map>
#include <string>
#include <iostream>

static const std::chrono::duration<long, std::milli> IDLE_SLEEP_MS(1);

using namespace std::chrono;
using namespace aeron::util;
using namespace aeron;

namespace Connection{

}

