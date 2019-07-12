/**
 *  @dev minakokojima, yukiexe
 *  @env cdt v1.2.x
 */
#pragma once
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>

#include "config.hpp"
#include "utils.hpp"

using namespace eosio;
using namespace std;

struct st_transfer {
    name   from;
    name   to;
    asset  quantity;
    string memo;
};

CONTRACT payout : public contract {
public:
    payout(name receiver, name code, datastream<const char*> ds): 
        contract(receiver, code, ds),
        _global(receiver, receiver.value) {
    }

    TABLE voter_info {
        name     to;
        asset    staked;
        int64_t  payout;        
    };

    TABLE global_info {
        uint64_t defer_id;
        asset    total_staked;
        int128_t earnings_per_share;
        name last;
        time st, ed;
        uint64_t pool;
    };

    typedef singleton<"voters"_n, voter_info> singleton_voters;
    typedef singleton<"global"_n, global_info> singleton_global;

    singleton_global _global;

    uint64_t get_next_defer_id() {
        auto g = _global.get();    
        g.defer_id += 1;
        _global.set(g,_self);
        return g.defer_id;
    }

    template <typename... Args>
    void send_defer_action(Args&&... args) {
        transaction trx;
        trx.actions.emplace_back(std::forward<Args>(args)...);
        trx.send(get_next_defer_id(), _self, false);
    }    

    
    ACTION init();
    ACTION unstake(name from, asset delta);
    ACTION claim(name from);  
    ACTION upgrade(name from);
    ACTION open(name from);    

    ACTION transfer(name from, name to, asset quantity, string memo) {        
    }
    void onTransfer(name from, name to, extended_asset in, string memo);

    void buy(name from, uint64_t in);        
    void stake(name from, asset delta);
    void make_profit(uint64_t delta);

    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        auto &thiscontract = *this;
        if (action == name("transfer").value) {
            auto transfer_data = unpack_action_data<st_transfer>();
            onTransfer(transfer_data.from, transfer_data.to, extended_asset(transfer_data.quantity, name(code)), transfer_data.memo);
            return;
        }

        switch (action) {
             EOSIO_DISPATCH_HELPER(payout, 
                (init)
                (upgrade)
                (open)
                (transfer)
            )
        }
    }
};

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        payout p( name(receiver), name(code), datastream<const char*>(nullptr, 0) );
        p.apply(receiver, code, action);
        eosio_exit(0);
    }
}