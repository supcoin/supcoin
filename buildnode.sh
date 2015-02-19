#!/bin/bash
./autogen.sh
./configure --with-incompatible-bdb
make
make check
echo "rpcuser=foo" > ~/.supcoin/supcoin.conf
echo "rpcpassword=bar" >> ~/.supcoin/supcoin.conf
cd src
./supcoind -daemon
./supcoind -daemon -testnet
./supcoin-cli getinfo
./supcoin-cli -testnet getinfo
