#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

#include <string>

namespace signature{
  std::string readPublicKey(std::string publickeyname);

  bool verify(std::string signature,std::string message, std::string publicKeyName);
  std::string sign(std::string message, std::string privateKeyName,std::string publicKeyName);
  bool createKeyPair(std::string filenamePrefix,std::string keyPath);

  bool test();
};

#endif
