#include "payout.hpp"

void payout::init() {
    require_auth(_self);
    auto g = _global.get_or_create( _self, global_info{});
    g.st = 1544788800; // 2018/12/14 20:00:00
    g.ed = 1544788800 + 30*24*60*60;
    _global.set(g, _self);    
}

void payout::make_profit(uint64_t delta) {
    auto g = _global.get();
    g.earnings_per_share += MAGNITUDE * delta / g.total_staked.amount;
    _global.set(g, _self);
}


void payout::stake(name from, asset delta) {
    require_auth(from);
    // eosio_assert(delta.amount > 0, "must stake a positive amount");
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{});
    auto g = _global.get();
    v.staked += delta;
    v.payout += g.earnings_per_share * delta.amount / MAGNITUDE;
    _voters.set(v, _self);
    g.total_staked += delta;
    _global.set(g, _self);
}

void payout::unstake(name from, asset delta) {
    require_auth(from);
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{});
    auto g = _global.get();
    // eosio_assert(delta <= v.staked, "don't have enough token for unstake");

    if (g.earnings_per_share * delta.amount / MAGNITUDE <= v.payout) {
        v.payout -= g.earnings_per_share * delta.amount / MAGNITUDE;
    } else {
        v.payout = 0;
    }

    v.staked -= delta;
    _voters.set(v, _self);    
}

void payout::open(name from) {
    require_auth(from);   
    claim(from);
    singleton_voters _voters(_self, from.value);    
    auto v = _voters.get_or_create(_self, voter_info{});     
    unstake(from, v.staked);
}

void payout::buy(name from, uint64_t in) {
    uint64_t out = in / 1000;
    check(out > 0, "insufficient box");
    stake(from, asset(out, TOKEN_SYMBOL));

    auto g = _global.get_or_create( _self, global_info{});
    g.last = from;
    g.ed += 30;
    _global.set(g, _self);     
}

void payout::upgrade(name from) {
    require_auth(from);
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{});
    auto g = _global.get();

    // TODO(minakokojima): unvote(v);
    auto delta = asset(0, EOS_SYMBOL);
    auto raw_payout = g.earnings_per_share * v.staked.amount / MAGNITUDE;
    if (raw_payout > v.payout) delta.amount = raw_payout - v.payout;

    v.payout = raw_payout;
    _voters.set(v, _self);

    buy(from, delta.amount > 0);
}

void payout::claim(name from) {
    require_auth(from);
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{});
    auto g = _global.get();

    // TODO(minakokojima): unvote(v);
    auto delta = asset(0, EOS_SYMBOL);
    auto raw_payout = g.earnings_per_share * v.staked.amount / MAGNITUDE;
    if (raw_payout > v.payout) delta.amount = raw_payout - v.payout;

    v.payout = raw_payout;
    _voters.set(v, _self);

    if (delta.amount > 0) {
        action(
            permission_level{_self, "active"_n},
            EOS_CONTRACT, "transfer"_n,
            make_tuple(_self, from, delta,
                string("claim dividend."))
        ).send();        
    }
}

void payout::onTransfer(name from, name to, extended_asset in, string memo) {        

    if (to != _self) return;
    require_auth(from);
    eosio::check(in.quantity.is_valid(), "invalid token transfer");
    eosio::check(in.quantity.is_valid(), "invalid token transfer");  

    check(in.contract == EOS_CONTRACT, "must use true EOS to make profit");
    check(in.quantity.symbol == EOS_SYMBOL, "must use EOS to make profit");      
    // eosio_assert(in.quantity.amount > 0, "must transfer a positive amount");

    auto params = split(memo, ' ');
    // eosio_assert(params.size() >= 1, "error memo");    

    buy(from, in.quantity.amount);
}