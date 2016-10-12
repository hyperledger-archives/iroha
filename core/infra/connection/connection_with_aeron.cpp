#include "../../consensus/connection/connection.hpp"

#include "../../util/logger.hpp"

#include <string>
#include <functional>
#include <thread>
#include <cstdint>
#include <cstdio>
#include <memory>

#include <util/CommandOptionParser.h>
#include <Aeron.h>
#include <array>
#include <concurrent/BusySpinIdleStrategy.h>

#include <unordered_map>
#include <string>

static const std::chrono::duration<long, std::milli> IDLE_SLEEP_MS(1);

using namespace std::chrono;
using namespace aeron::util;
using namespace aeron;

std::atomic<int> counter (0);

static const std::chrono::duration<long, std::milli> IDLE_SLEEP_MS(1);
static const int FRAGMENTS_LIMIT = 10;

namespace connection {

struct Settings{
    std::string dirPrefix = "";
    std::string channel = "aeron:udp?endpoint=45.32.158.71:40123";
    std::int32_t streamId = 10;
    int numberOfMessages = 5000;
    int lingerTimeoutMs = 10;
} settings;

fragment_handler_t receiveMessage()
{
    return [&](AtomicBuffer& buffer, util::index_t offset, util::index_t length, Header& header)
    {
        std::string raw_data = std::string((char *)buffer.buffer() + offset, (unsigned long)length);
        counter  = std::atoi(raw_data.c_str()) + 1;
    };
}


std::map<std::string, std::shared_ptr<Publication>> publications;
std::shared_ptr<Aeron> aeron;
typedef std::array<std::uint8_t, 1024> buffer_t;



int exe(int argc, char** argv){

    std::string ip   = "127.0.0.1"; 
    char res_opt;
    while((res_opt = getopt(argc,argv,"i:")) != -1){
        switch(res_opt){
        case 'i':
            ip = std::string(optarg);
            break;
        }
    }

    try{
        std::cout << "Channel aeron:udp?endpoint="+ip+":40123  on Stream ID " << settings.streamId << std::endl;
        aeron::Context context;

        context.newPublicationHandler(
            [](const std::string& channel, std::int32_t streamId, std::int32_t sessionId, std::int64_t correlationId){
            std::cout << "Publication: " << channel << " " << correlationId << ":" << streamId << ":" << sessionId << std::endl;
        });
        context.newSubscriptionHandler(
            [](const std::string& channel, std::int32_t streamId, std::int64_t correlationId){
                std::cout << "Subscription: " << channel << " " << correlationId << ":" << streamId << std::endl;
        });

        aeron = Aeron::connect(context);

        std::int64_t sid = aeron->addSubscription("aeron:udp?endpoint="+ip+":40123", settings.streamId);
        std::shared_ptr<Subscription> subscription = aeron->findSubscription(sid);
        while (!subscription) {
            std::this_thread::yield();
            subscription = aeron->findSubscription(sid);
        }

        fragment_handler_t handler = receiveMessage();
        SleepingIdleStrategy idleStrategy(IDLE_SLEEP_MS);
        while (1){
            const int fragmentsRead = subscription->poll(handler, FRAGMENTS_LIMIT);
            idleStrategy.idle(fragmentsRead);
        }

    }catch (SourcedException& e) {
        std::cerr << "FAILED: " << e.what() << " : " << e.where() << std::endl;
        return -1;
    }catch (std::exception& e) {
        std::cerr << "FAILED: " << e.what() << " : " << std::endl;
        return -1;
    }
}

void addPublication(std::string ipaddr){
    std::int64_t pid = aeron->addPublication( "aeron:udp?endpoint="+ipaddr+":40123", settings.streamId);
    auto publication =  aeron->findPublication(pid);
    while (!publication) {
        std::this_thread::yield();
        publication = aeron->findPublication(pid);
    }
    publications.insert(std::pair<std::string, std::shared_ptr<Publication>>{ ipaddr, publication});
}

bool send(std::string to,std::string message) {
    if(publications.find(to) == message.end()){
        std::cout <<"Plz ip address\n";
        return false;
    }
    try{
        char* message = const_cast<char*>(message.c_str());
        AERON_DECL_ALIGNED(std::uint8_t buffer[512], 16);
        AtomicBuffer srcBuffer(&buffer[0], 512);
        srcBuffer.putBytes(0, reinterpret_cast<std::uint8_t *>(message), strlen(message));
        const std::int64_t result = publications[to]->offer(srcBuffer, 0, strlen(message));
        if (result < 0){
            if (NOT_CONNECTED == result){
                std::cout << " not connected yet." << std::endl;
            }else if (BACK_PRESSURED == result){
                std::cout << " back pressured." << std::endl;
            }else{
                std::cout << " ah?! unknown " << result << std::endl;
            }
        }else{
            std::cout <<"OK"<<std::endl;
        }
        if (!publication->isConnected()){
            std::cout << "No active subscribers detected" << std::endl;
        }
        return true;
    }catch (CommandOptionException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }catch (SourcedException& e) {
        std::cerr << "FAILED: " << e.what() << " : " << e.where() << std::endl;
        return false;
    }catch (std::exception& e) {
        std::cerr << "FAILED: " << e.what() << " : " << std::endl;
        return false;
    }
}

bool receive(std::function<void(std::string from,std::string message)> callback) {
    return true;
}
};