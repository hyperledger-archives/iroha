# Using ansible for local Iroha deployment
A side note: these scripts were taken and modified from D3. The reasoning: to get rid of `iroha-cli`.
To deploy several Iroha peers on your localhost:
1. Install ansible
2. Install docker
3. Install ed25519
```
pip install git+https://github.com/Warchant/pyed25519-sha3
```
4. Create docker network
```
docker network create iroha-network-local
```
5. Change/add peers in your inventory file. An example:
```
[local]
peer1 ansible_connection=local ansible_host=d3-iroha-0
peer2 ansible_connection=local ansible_host=d3-iroha-1
peer3 ansible_connection=local ansible_host=d3-iroha-2
peer4 ansible_connection=local ansible_host=d3-iroha-3

[local:vars]
ansible_python_interpreter=/usr/local/bin/python2.7
```
6. `cd` into `ansible` folder
7. Run the command:
```
ansible-playbook -i inventory_local.list main.yml -l local
```
8. ???
9. PROFIT
