
#include <repository/transaction_repository.hpp>

namespace repository{
    namespace transaction {

        bool add(const std::string &hash,const Transaction& tx){
            return world_state_repository::add("transaction_" + hash, tx.SerializeAsString());
        }

        std::vector<Transaction> findAll(){
            std::vector<Transaction> res;
            auto txstr = world_state_repository::findByPrefix("transaction_");
            for(auto txs: txstr){
                Transaction tx;
                tx.ParseFromString(txs);
                res.push_back(tx);
            }
            return res;
        }

        Transaction find(std::string hash){
            std::vector<Transaction> res;
            Transaction tx;
            if(world_state_repository::exists("transaction_" + hash)){
                tx.ParseFromString(world_state_repository::find("transaction_" + hash));
            }
            return tx;
        }

    }
}