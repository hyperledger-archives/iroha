# Info

First of all, you have to get the latest iroha binaries. For this, refer to [iroha-build](../README.md). Basically, you need to run `../build.sh`. It creates `iroha.tar` in this folder.

This directory will be mounted to `iroha-dev` container. After successful build, `iroha.tar` will be copied to host computer to this folder.

Then, to build iroha container, run:
```
docker build -t hyperledger/iroha-docker .
```


# Default config

Default `sumeragi.json` is 

```json
{
  "group": [
    {
      "ip": "127.0.0.1",
      "name": "da77880a3da4",
      "publicKey": "u7X/zQ/Dq21WW7YH4rbkpiCYJXjPxk5t3qNDKiVwBx8="
    }
  ],
  "me": {
    "ip": "127.0.0.1",
    "name": "da77880a3da4",
    "privateKey": "cPY84e0BXGUHBjT4QdlPI0LI3BPIfUfSZjB8jdWURkNQ+pEagT/ysrewbt2YUo/Qbfd5vczW5oDooGSNUBTj9g==",
    "publicKey": "u7X/zQ/Dq21WW7YH4rbkpiCYJXjPxk5t3qNDKiVwBx8="
  }
}
```