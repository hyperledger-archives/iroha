#include "../../connection/connection.hpp"

#include "../../util/logger.hpp"

#include <string>
#include <functional>
#include <thread>
#include <cstdint>
#include <cstdio>


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

namespace connection {

  std::atomic<bool> running (true);

  struct PeerContext {
    bool isAvaiableSubscription;
    bool isAvaiablePublicarion;

    std::unordered_map<std::string,std::string> peers;

    std::shared_ptr<Subscription> peerSubscription;
    std::shared_ptr<Publication>  peerPublication;
  };

  std::unique_ptr<PeerContext> context;
  aeron::Context aeronContext;

  void initialize_peer(const
      std::unordered_map<
        std::string, std::string
      > &config) {

    logger::info("connection", "URL:aeron:udp?endpoint="+config.at("address")+":"+config.at("port"));
    try{
      aeron::Context aeronContext;
      std::int64_t subscriptionId;
      std::int64_t publicationId;

      logger::info("connection", "Subscribing at "+
        config.at("subscribeChannel") +" on Stream ID "+ config.at("subscribeStreamId"));
      aeronContext.newSubscriptionHandler(
        [](const std::string& channel, std::int32_t streamId, std::int64_t correlationId)
        {
            logger::info(
              "connection",
              "Subscription: "+ channel +" "+ std::to_string(correlationId)+
                ":"+ std::to_string(streamId)
            );
        });

      logger::info("connection", "Publishing at "+config.at("publishChannel")+" on Stream ID "+config.at("publishStreamId"));
      aeronContext.newPublicationHandler(
        [](const std::string& channel, std::int32_t streamId, std::int32_t sessionId, std::int64_t correlationId)
        {
            logger::info(
              "connection",
              "Publication: "+ channel +" "+ std::to_string(correlationId)+
                ":"+ std::to_string(streamId) +":"+ std::to_string(sessionId)
            );
        });

      Aeron aeron(aeronContext);
      logger::info("connection", "Initialized aeron");
      std::shared_ptr<Subscription> pongSubscription;
      std::shared_ptr<Publication>  pingPublication;

      subscriptionId = aeron.addSubscription(
        "aeron:udp?endpoint="+config.at("address")+":"+config.at("port"),
        std::atoi(config.at("subscribeStreamId").c_str())
      );
      pongSubscription = aeron.findSubscription(subscriptionId);

      while (!pongSubscription) {
          std::this_thread::yield();
          pongSubscription = aeron.findSubscription(subscriptionId);
      }

      publicationId = aeron.addPublication(
        "aeron:udp?endpoint="+config.at("address")+":"+config.at("port"),
        std::atoi(config.at("publishStreamId").c_str())
      );

      pingPublication = aeron.findPublication(publicationId);
      while (!pingPublication) {
          std::this_thread::yield();
          pingPublication = aeron.findPublication(publicationId);
      }

    } catch (CommandOptionException& e) {
      logger::error("connection","ERROR: "+std::string(e.what()));
      return;
    } catch (SourcedException& e) {
      logger::error("connection","FAILED: "+std::string(e.what()));
      return;
    } catch (std::exception& e) {
      logger::error("connection","FAILED: "+std::string(e.what()));
      return;
    }
  }

  bool sendAll(std::string message) {
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

  bool send(std::string to,std::string message) {
    if (context->peerPublication != nullptr){
      AERON_DECL_ALIGNED(std::uint8_t buffer[256], 16);
      AtomicBuffer srcBuffer(&buffer[0], 256);
      do {
        srcBuffer.putBytes(0, reinterpret_cast<const std::uint8_t *>(message.c_str()), message.size());
      } while (context->peerPublication->offer(srcBuffer, 0, message.size()) < 0L);
      return true;
    }
    return false;
  }

  bool receive(std::function<void(std::string from,std::string message)> callback) {
    fragment_handler_t handler = [&](AtomicBuffer& buffer, util::index_t offset, util::index_t length, Header& header){
      // ToDo validate string from, message
        callback(std::string((char *)buffer.buffer() + offset, (unsigned long)length),std::string((char *)buffer.buffer() + offset, (unsigned long)length));
    };
    if (context->peerSubscription) {
      SleepingIdleStrategy idleStrategy(IDLE_SLEEP_MS);
      while (context->peerSubscription->poll(handler, 10) <= 0) {
        idleStrategy.idle(0);
      }
      return true;
    }
    return false;
  }
};  // namespace connection
