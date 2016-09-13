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
#include <yaml-cpp/yaml.h>

static const std::chrono::duration<long, std::milli> IDLE_SLEEP_MS(1);

using namespace std::chrono;
using namespace aeron::util;
using namespace aeron;

namespace Connection {

  std::atomic<bool> running (true);

  struct PeerContext{
    bool isAvaiableSubscription;
    bool isAvaiablePublicarion;

    std::unordered_map<std::string,std::string> peers;

    std::shared_ptr<Subscription> peerSubscription;
    std::shared_ptr<Publication>  peerPublication;

  };

  struct Config {

    std::string  address;
    std::string     port;
    std::string     name;

    int publishChannel;
    int publishStreamId;
    int subscribeChannel;
    int subscribeStreamId;

  };

  std::unique_ptr<PeerContext> context;
  aeron::Context aeronContext;

  void sigIntHandler(int param){
    std::cout << "Halt peer\n";
    std::cout << param << "\n";
    running = false;
  }

  Config loadYaml(){
    Config c;
    try{
      YAML::Node config = YAML::LoadFile("config.yml");
      c.address = config["mediaDriver"]["address"].as<std::string>();
      c.port = config["mediaDriver"]["port"].as<std::string>();

      c.publishStreamId = config["mediaDriver"]["publishStreamId"].as<int>();
      c.subscribeStreamId = config["mediaDriver"]["subscribeStreamId"].as<int>();

      c.name = config["mediaDriver"]["name"].as<std::string>();
    }catch(YAML::Exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return c;
  }

  void initialize_peer(){
    signal (SIGINT, sigIntHandler);

    Config config = loadYaml();
    std::cout<< "Loaded config \n";
    std::cout<< "URL:"<<"aeron:udp?endpoint="+config.address+":"+config.port << "\n";
    try{
      aeron::Context aeronContext;
      std::int64_t subscriptionId;
      std::int64_t publicationId;

      std::cout << "Subscribing at " << config.subscribeChannel << " on Stream ID " << config.subscribeStreamId << std::endl;
      aeronContext.newSubscriptionHandler(
        [](const std::string& channel, std::int32_t streamId, std::int64_t correlationId)
        {
            std::cout << "Subscription: " << channel << " " << correlationId << ":" << streamId << std::endl;
        });

      std::cout << "Publishing at " << config.publishChannel << " on Stream ID " << config.publishStreamId << std::endl;
      aeronContext.newPublicationHandler(
        [](const std::string& channel, std::int32_t streamId, std::int32_t sessionId, std::int64_t correlationId)
        {
            std::cout << "Publication: " << channel << " " << correlationId << ":" << streamId << ":" << sessionId << std::endl;
        });

      Aeron aeron(aeronContext);
      std::cout<< "Initialized aeron \n";
      std::shared_ptr<Subscription> pongSubscription;
      std::shared_ptr<Publication> pingPublication;

      subscriptionId = aeron.addSubscription("aeron:udp?endpoint="+config.address+":"+config.port, config.subscribeStreamId);
      pongSubscription = aeron.findSubscription(subscriptionId);

      while (!pongSubscription){
          std::this_thread::yield();
          pongSubscription = aeron.findSubscription(subscriptionId);
      }

      publicationId = aeron.addPublication("aeron:udp?endpoint="+config.address+":"+config.port, config.publishStreamId);

      pingPublication = aeron.findPublication(publicationId);
      while (!pingPublication){
          std::this_thread::yield();
          pingPublication = aeron.findPublication(publicationId);
      }

    }catch (CommandOptionException& e){
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        return;
    }catch (SourcedException& e){
        std::cerr << "FAILED: " << e.what() << " : " << e.where() << std::endl;
        return;
    }catch (std::exception& e){
        std::cerr << "FAILED: " << e.what() << " : " << std::endl;
        return;
    }
  }

  bool sendAll(std::string message){
    if(context->peerPublication != nullptr){
      AERON_DECL_ALIGNED(std::uint8_t buffer[256], 16);
      AtomicBuffer srcBuffer(&buffer[0], 256);
      do{
        srcBuffer.putBytes(0, reinterpret_cast<const std::uint8_t *>(message.c_str()), message.size());
      }while (context->peerPublication->offer(srcBuffer, 0, message.size()) < 0L);
      return true;
    }
    return false;
  }

  bool send(std::string to,std::string message){
    if(context->peerPublication != nullptr){
      AERON_DECL_ALIGNED(std::uint8_t buffer[256], 16);
      AtomicBuffer srcBuffer(&buffer[0], 256);
      do{
        srcBuffer.putBytes(0, reinterpret_cast<const std::uint8_t *>(message.c_str()), message.size());
      }while (context->peerPublication->offer(srcBuffer, 0, message.size()) < 0L);
      return true;
    }
    return false;
  }

  bool receive(std::function<void(std::string from,std::string message)> callback){
    fragment_handler_t handler = [&](AtomicBuffer& buffer, util::index_t offset, util::index_t length, Header& header){
      // ToDo validate string from, message
        callback(std::string((char *)buffer.buffer() + offset, (unsigned long)length),std::string((char *)buffer.buffer() + offset, (unsigned long)length));
    };
    if(context->peerSubscription){
      SleepingIdleStrategy idleStrategy(IDLE_SLEEP_MS);
      while (context->peerSubscription->poll(handler, 10) <= 0){
        idleStrategy.idle(0);
      }
      return true;
    }
    return false;
  }

};
