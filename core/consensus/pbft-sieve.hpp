
namespace pbft_sieve{
  void initialize_pbft_sieve(int myNumber,int aNumberOfPeer, int leaderNumber);
  void loop();
  void command(std::string command,std::vector<std::string> args);
  void execute();
};
