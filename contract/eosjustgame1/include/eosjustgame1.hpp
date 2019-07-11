/**
 *  @dev minakokojima, yukiexe
 *  env: EOSIO.CDT v1.3.2
 *  @copyright Andoromeda
 */
#pragma once
#include "council.cpp"
#include "kyubey.hpp"
 
CONTRACT eosjustgame1 : public council {
public:
    eosjustgame1( name receiver, name code, datastream<const char*> ds ) :
        council( receiver, code, ds ),
        _market(receiver, receiver.value) {}
    
    TABLE global : public global_info {};
    TABLE voters : public voter_info {};
    TABLE market : public kyubey::_market {};

    
    TABLE players {
        uint64_t game_profit;   // 游戏收入(卖land/portal两种情况)以及exceed的 EOS
        uint64_t ref_profit;    // 拉人收入(land/portal两种情况) CMU
        uint64_t fee_profit;    // creator创建地标收入以及地标在哪个land的owner收入(仅portal) CMU
        void withdraw() {
        }
    };

    typedef singleton<"players"_n, players> singleton_players;
    typedef eosio::multi_index<"market"_n, market> market_t;
    market_t _market;
    
    /* 
    ACTION init();
    ACTION clear();
    // ACTION test() { require_auth(_self); }
    
    ACTION claim(name from) {
        eosio_assert(false, "not start yet.");
        require_auth(from);
        council::claim(from);

        singleton_players _player(_self, from.value);
        auto p = _player.get_or_create(_self, players{});

        if (p.ref_profit > 0) {
            action(
                permission_level{_self, "active"_n},
                EOS_CONTRACT, "transfer"_n,
                make_tuple(_self, from, asset(p.ref_profit, EOS_SYMBOL),
                    std::string("refer profit CMU."))
            ).send();
            p.ref_profit = 0;
        }
        _player.set(p,_self);
    }
    
    ACTION upgrade(name from) {
        require_auth(from);
    }
    
    
    ACTION open(name from) {
        require_auth(from);
        claim(from);
    }
    
    ACTION refund(name from) {
        council::refund(from);
    }      


    ACTION transfer(name from, name to, asset quantity, string memo);    
    void onTransfer(name from, name to, extended_asset in, string memo);
    void buy(name from, extended_asset in, const vector<string>& params);
    void sell(name from, extended_asset in, const vector<string>& params);

    */

    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        /* auto &thiscontract = *this;
        if (action == name("transfer").value) {
            auto transfer_data = unpack_action_data<st_transfer>();
            onTransfer(transfer_data.from, transfer_data.to, extended_asset(transfer_data.quantity, name(code)), transfer_data.memo);
            return;
        }

        switch (action) {
            EOSIO_DISPATCH_HELPER(eosjustgame1,
                (init)
                (clear)
                (claim)
                (refund)
            )
        }*/
    } 
};

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        eosjustgame1 p( name(receiver), name(code), datastream<const char*>(nullptr, 0) );
        p.apply(receiver, code, action);
        eosio_exit(0);
    }
}
