/**
 *  @dev minakokojima, yukiexe
 *  @copyright Andoromeda
 */
#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>
#include "utils.hpp"
 
using namespace eosio;
using namespace std;

// https://github.com/EOSIO/eos/search?q=_voters&unscoped_q=_voters

using std::string;
// TODO
// 还没有办法让council, proxy退出。
// 还没实现72小时后取token。

CONTRACT council : public contract {
public:
    council( name receiver, name code, datastream<const char*> ds ) :
        contract(receiver, code, ds),        
        _global(receiver, receiver.value) {
    }

    TABLE global_info {
        uint64_t defer_id;
        asset    total_staked;
        uint128_t earnings_per_share;

        name last;
        time st, ed;
        uint64_t pool;
    };    

    TABLE voter_info {
        name     to;
        asset    staked;
        int64_t  payout;        
    };

    TABLE refund_request {
        time request_time;
        asset    amount;
    };    

    /*
    TABLE proxy_info {
        name owner;
        name to;
        uint64_t     delegated_staked ;

        auto primary_key()const { return owner; }
    };*/

    TABLE council_info {
        int64_t     total_votes;
        int64_t     unpaid;
    };  

    typedef singleton<"global"_n, global_info> singleton_global_t;
    typedef singleton<"voters"_n, voter_info> singleton_voters;
    typedef singleton<"refunds"_n, refund_request> singleton_refunds;
    // typedef singleton<"proxies"_n, proxy_info> singleton_proxies;
    typedef singleton<"council"_n, council_info> singleton_council;

    singleton_global_t _global;

    uint64_t get_next_defer_id() {
        auto g = _global.get();    
        g.defer_id += 1;
        _global.set(g,_self);
        return g.defer_id;
    }

    void send_defer_refund_action(name from) {
        transaction out;        
        out.actions.emplace_back(
            permission_level{_self, "active"_n},
            _self, "refund"_n, 
            from
        );
        out.delay_sec = refund_delay;
        cancel_deferred(from.value); // TODO: Remove this line when replacing deferred trxs is fixed
        out.send(from.value, _self, true);
    }

    template <typename... Args>
    void send_defer_action(Args&&... args) {
        transaction trx;
        trx.actions.emplace_back(std::forward<Args>(args)...);
        trx.send(get_next_defer_id(), _self, false);
    }    


    // typedef eosio::multi_index<"proxies"_n, proxy_info>  proxies_t;
    // typedef eosio::multi_index<"council"_n, council_info>  council_t;    
//    proxies_t _proxies;
 //   council_t _council;

        /*
        require_auth(from);
        eosio_assert(delta == 0, "must stake a positive amount");

        auto itr = _voters.find(from);
        if (itr != _voters.end()) {    
            // voter already exist, update the staked information.
            unvote(itr);
            _voters.modify(itr, _code, [&](auto &v) {
                v.staked += delta;
            });
            vote(itr); 
        } else {
            // new voter.    
            
            _voters.emplace(_code, [&](auto &v) {
                v.owner = from;
                v.staked += delta;
            });            
        }*/
  //  }

    /*
    void unvote( voters_t::const_iterator itr  ) {
        
        auto p = _proxies.find(itr->to);
        if (p != _proxies.end()) { 
            _proxies.modify(p, _code, [&](auto &pp) {
                pp.delegated_staked -= itr->staked;
            });
            auto c = _council.find(p->to);
            if (c != _council.end()) {                 
                _council.modify(c, _code, [&](auto &cc) {
                    cc.total_votes -= itr->staked;
                });     
            }
            return;
        }        
        auto c = _council.find(itr->to);
        if (c != _council.end()) {             
            _council.modify(c, _code, [&](auto &cc) {
                cc.total_votes -= itr->staked;
            });
        }
        _voters.modify(itr, _code, [&](auto &v) {

        });
    }*/

    void unvote( /*proxies_t::const_iterator itr*/ ) {
        /*
        auto c = _council.find(itr->to);
        if (c != _council.end()) { 
            _council.modify(c, _code, [&](auto &cc) {
                cc.total_votes -= itr->delegated_staked;
            });
        }
        */
    }
/*
    void vote(voters_t::const_iterator itr) {
        unvote(itr);
        auto p = _proxies.find(itr->to);
        if (p != _proxies.end()) {             
            _proxies.modify(p, _code, [&](auto &pp) {
                pp.delegated_staked += itr->staked;
            });
            auto c = _council.find(p->to);
            if (c != _council.end()) { 
                _council.modify(c, _code, [&](auto &cc) {
                    cc.total_votes += itr->staked;
                });            
            }
            return;
        }        
        auto c = _council.find(itr->to);
        if (c != _council.end()) {             
            _council.modify(c, _code, [&](auto &cc) {
                cc.total_votes += itr->staked;
            });
            return;          
        }
    }
    */

    /*
    void vote(proxies_t::const_iterator itr) {   
        
        unvote(itr);     
        auto c = _council.find(itr->to);
        if (c != _council.end()) { 
            _council.modify(c, _code, [&](auto &cc) {
                cc.total_votes += itr->delegated_staked;
            });
        }
        
    }    
*/
    // 申明自己参与委员会
    void runcouncil(name from) {
        require_auth(from);

        // warning!!!
        // 申明成为委员会需要哪些条件？
        // warning!!!
        
        singleton_council _council(_self, from.value);
        auto c = _council.get_or_create(_self, council_info{});
        _council.set(c, _self);     
    }

    // ACTION init() { require_auth(_self); }
    ACTION unstake(name from, asset delta);
    ACTION claim(name from);         
    ACTION refund(name from);    
    ACTION vote(name from, name to);
    ACTION unvote(name from);
    ACTION transfer(name from, name to, asset quantity, string memo);
    void onTransfer(name from, name to, extended_asset in, string memo);
    void stake(name from, asset delta);
    void make_profit(uint64_t delta);
};