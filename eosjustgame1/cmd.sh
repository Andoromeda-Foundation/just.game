
## 准备
cleos system newaccount eosio eosjustgame1 EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 2
cleos system newaccount eosio justgamedevl EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 2
cleos system newaccount eosio testuser1111 EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 2
cleos system newaccount eosio testuser2222 EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 2
cleos system newaccount eosio 1kgj1love24n EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 1024000


cleos transfer eosio.token eosjustgame1 "5000 EOS" "test"
cleos transfer eosio.token joetothemoon "5000 EOS" "test"
cleos transfer eosio.token testuser1111 "5000 EOS" "test"
cleos transfer eosio.token testuser2222 "5000 EOS" "test"
cleos transfer eosio.token 1kgj1love24n "5000 EOS" "test"



cleos set account permission 1kgj1love24n active '{"threshold": 1,"keys": [{"key": "EOS6tkSGWHB5m9cvpKhYJwCcoQnQMnoek3xbNoWEnHYHcsvXsSTdR","weight": 1}],"accounts":[{"permission":{"actor":"1kgj1love24n","permission":"eosio.code"},"weight":1}]}' owner -p 1kgj1love24n@owner
cleos set contract 1kgj1love24n /Users/joe/Workspace/eos-project/ayana/ayana -p 1kgj1love24n


## 部署
cleos system buyram eosjustgame1 eosjustgame1  1024000 --bytes

cleos set account permission eosjustgame1 active '{"threshold": 1,"keys": [{"key": "EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q","weight":1}],"accounts":[{"permission":{"actor":"eosjustgame1","permission":"eosio.code"},"weight":1}]}' owner -p eosjustgame1

cleos set contract eosjustgame1 /Users/joe/Workspace/eos-project/ite-fe/just.game/eosjustgame1/build/eosjustgame1 -p eosjustgame1

cleos push action 1kgj1love24n send '["testuser2222", "1.0000 EOS"]' -p 1kgj1love24n
cleos push action 1kgj1love24n send '["eosjustgame1", "1.0000 EOS"]' -p 1kgj1love24n


cleos transfer 1kgj1love24n eosjustgame1 "0.1 EOS" "buy"
cleos transfer joetothemoon eosjustgame1 "0.1 EOS" "buy"


cleos transfer 1kgj1love24n eosjustgame1 "100.1 EOS" "buy"
cleos transfer joetothemoon eosjustgame1 "100.1 EOS" "buy"



# 游戏初始化: 开局时间写在代码里
cleos push action eosjustgame1 init '[1563206744]' -p eosjustgame1
# 合约清理
cleos push action eosjustgame1 clean '[]' -p eosjustgame1

# 查看游戏状态
cleos get table eosjustgame1 eosjustgame1 pool
cleos get table eosjustgame1 eosjustgame1 players



# 开箱子
cleos push action eosjustgame1 open '["1kgj1love24n"]' -p 1kgj1love24n
cleos push action eosjustgame1 open '["joetothemoon"]' -p joetothemoon

cleos push action eosjustgame1 upgrade '["1kgj1love24n"]' -p 1kgj1love24n
cleos push action eosjustgame1 upgrade '["joetothemoon"]' -p joetothemoon




cleos system newaccount eosio testtoken111 EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 102400
cleos system newaccount eosio testtoken222 EOS6Fhx9xMx5eNoTQjVSvNVMarT5QpPj9kddJmNbzpCbXMvtbgL9q  --stake-net "0.01 EOS" --stake-cpu "0.1 EOS" --buy-ram-kbytes 102400


cleos set contract testtoken111 /Users/joe/Workspace/eos-source/eosio.contracts/build/contracts/eosio.token -p testtoken111
cleos set contract testtoken222 /Users/joe/Workspace/eos-source/eosio.contracts/build/contracts/eosio.token -p testtoken222

cleos push action testtoken111 create '[ "testtoken111", "1000000000.0000 DICE"]' -p testtoken111
cleos push action testtoken111 create '[ "testtoken111", "1000000000.0000 BTC"]' -p testtoken111
cleos push action testtoken111 create '[ "testtoken111", "1000000000.0000 EOS"]' -p testtoken111
cleos push action testtoken222 create '[ "testtoken222", "1000000000.0000 BTC"]' -p testtoken222

cleos push action testtoken111 issue '[ "testtoken111", "1000000000.0000 DICE", "initial supply" ]' -p testtoken111
cleos push action testtoken111 issue '[ "testtoken111", "1000000000.0000 BTC", "initial supply" ]' -p testtoken111
cleos push action testtoken111 issue '[ "testtoken111", "1000000000.0000 EOS", "initial supply" ]' -p testtoken111
cleos push action testtoken222 issue '[ "testtoken222", "1000000000.0000 BTC", "initial supply" ]' -p testtoken222


cleos transfer -c testtoken111 testtoken111 fastecoboxme "1000.0000 EOS" "test"

