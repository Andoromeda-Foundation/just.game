#include "council.hpp"

void council::make_profit(uint64_t delta) {
    auto g = _global.get();
    g.earnings_per_share += MAGNITUDE * delta / g.total_staked.amount;
    _global.set(g, _self);
}

void council::stake(name from, asset delta) {
    require_auth(from);
    eosio_assert(delta.amount > 0, "must stake a positive amount");
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{.staked = asset(0, CMU_SYMBOL)}); //CMU_SYMBOL
    auto g = _global.get();
    v.staked += delta;
    v.payout += g.earnings_per_share * delta.amount / MAGNITUDE;
    _voters.set(v, _self);
    g.total_staked += delta;
    _global.set(g, _self);
}

void council::unstake(name from, asset delta) {
    require_auth(from);
    eosio_assert(delta.is_valid(), "invalid quantity");
    eosio_assert(delta.amount > 0, "must issue positive quantity");
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{.staked = asset(0, TOKEN_SYMBOL)});
    auto g = _global.get();
    eosio_assert(delta <= v.staked, "don't have enough token for unstake");

    if (g.earnings_per_share * delta.amount / MAGNITUDE <= v.payout) {
        v.payout -= g.earnings_per_share * delta.amount / MAGNITUDE;
    } else {
        v.payout = 0;
    }

    v.staked -= delta;
    _voters.set(v, _self);   

    g.total_staked -= delta;
    _global.set(g, _self);

    singleton_council _council(_self, v.to.value);
    if (_council.exists()) {
        auto c = _council.get();
        c.total_votes -= delta.amount;
        _council.set(c, _self); 
    }

    singleton_refunds _refunds( _self, from.value );
    auto req = _refunds.get_or_create(_self, refund_request{.amount = asset(0, TOKEN_SYMBOL)});
    req.request_time = now();
    req.amount += delta;
    _refunds.set(req, _self);

    send_defer_refund_action(from);
    // council::refund(from);
}

void council::claim(name from) {
    require_auth(from);
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{.staked = asset(0, TOKEN_SYMBOL)});
    auto g = _global.get();

    // TODO(minakokojima): unvote(v);
    auto delta = asset(0, TOKEN_SYMBOL);
    auto raw_council = g.earnings_per_share * v.staked.amount / MAGNITUDE;
    if (raw_council > v.payout) delta.amount = raw_council - v.payout;

    v.payout = raw_council;
    _voters.set(v, _self);

    if (delta.amount > 0) {
        action(
            permission_level{_self, "active"_n},
            TOKEN_CONTRACT, "transfer"_n,
            make_tuple(_self, from, delta,
                string("claim dividend."))
        ).send();        
    }
}

void council::refund(name from) {
    singleton_refunds refunds_tbl(_self, from.value);
    eosio_assert( refunds_tbl.exists(), "refund request not found" );
    
    auto req = refunds_tbl.get();
    eosio_assert( req.request_time + refund_delay >= now(), "refund is not available yet" );
    // Until now() becomes NOW, the fact that now() is the timestamp of the previous block could in theory
    // allow people to get their tokens earlier than the 1 day delay if the unstake happened immediately after many
    // consecutive missed blocks.

    action(
        permission_level{_self, "active"_n},
        TOKEN_CONTRACT, "transfer"_n,
        make_tuple(_self, from, req.amount, string("unstake refund"))
    ).send();

    refunds_tbl.remove();
}

void council::vote(name from, name to) {
    require_auth(from);    
    singleton_voters _voters(_self, from.value);
    auto v = _voters.get_or_create(_self, voter_info{.staked = asset(0, TOKEN_SYMBOL)});
    singleton_council _council_old(_self, v.to.value);
    if (_council_old.exists()) {
        auto c = _council_old.get();
        c.total_votes -= v.staked.amount;
        _council_old.set(c, _self); 
    }

    v.to = to;
    _voters.set(v, _self);     
    singleton_council _council_new(_self, v.to.value);
    if (_council_new.exists()) {
        auto c = _council_new.get();
        c.total_votes += v.staked.amount;
        _council_new.set(c, _self); 
    } else {
        v.to = name(0);
    }
}

void council::unvote(name from) {
    vote(from, from);
}

void council::onTransfer(name from, name to, extended_asset in, string memo) {        

    if (to != _self) return;
    require_auth(from);
    eosio_assert(in.quantity.is_valid(), "invalid token transfer");
    eosio_assert(in.quantity.amount > 0, "must transfer a positive amount");

    auto params = split(memo, ' ');
    eosio_assert(params.size() >= 1, "error memo");    
    
    if (params[0] == "stake") {        
        eosio_assert(in.contract == TOKEN_CONTRACT, "must use true target TOKEN to stake");
        eosio_assert(in.quantity.symbol == TOKEN_SYMBOL, "must use target TOKEN to stake");
        stake(from, in.quantity);
        return;
    }    
    
    if (params[0] == "make_profit") {
        eosio_assert(in.contract == TOKEN_CONTRACT, "must use true target TOKEN to make profit");
        eosio_assert(in.quantity.symbol == TOKEN_SYMBOL, "must use target TOKEN to make profit");
        make_profit(in.quantity.amount);
        return;
    }
}
