#include <eosjustgame1.hpp>

void eosjustgame1::handle_transfer(name from, name to, asset quantity, string memo, name code)
{
    if (from == _self || to != _self)
    {
        return;
    }

    against_contract_robot();

    string action;
    name ref;

    // 格式为: buy-justgamedev1  或者 buy-
    parse_memo(memo, &action, &ref);

    if (!is_account(ref))
    {
        ref = DEV_ACCOUNT;
    }

    if (action == "buy")
    {
        buy(from, quantity, ref);
    }
}

// 处理买盒子逻辑
void eosjustgame1::buy(name user, asset quantity, name ref)
{
    check_game_available();

    check(quantity.amount >= 1000, "pay more than 0.1 EOS to buy a box");
    check(quantity.amount <= 5000000, "should pay less than 500 EOS ");

    static const time_point_sec now{current_time_point().sec_since_epoch()};

    asset contract_fee = quantity; // 10% = 5%开发+5%推广
    contract_fee.amount = quantity.amount * 10 / 100;

    asset ref_fee = quantity; // 35% 开箱储备金
    ref_fee.amount = quantity.amount * 5 / 100;

    uint64_t big_prize = quantity.amount * 5 / 100;      // 5% 最后大奖
    uint64_t last100_prize = quantity.amount * 10 / 100; // 10% 最后100位奖励

    const auto &pool = _pool.begin();
    auto u_itr = _players.find(user.value);

    auto box_num = quantity.amount / 1000;
    auto total_box = pool->box + box_num;

    auto base_profit = quantity.amount * 70 / 100;
    auto profit_per_box = base_profit * base / total_box;

    auto pool_mask = pool->mask + profit_per_box;
    check(pool_mask >= profit_per_box, "pool mask overflow");

    auto player_profit = profit_per_box * box_num / base;

    if (u_itr == _players.end())
    {
        auto umake = pool_mask * box_num / base - player_profit;
        _players.emplace(_self, [&](auto &c) {
            c.user = user;
            c.mask = umake;
            c.join = now;
            c.lastac = now;
            c.box = box_num;
        });
    }
    else
    {
        auto umake = u_itr->mask + pool_mask * box_num / base - player_profit;

        _players.modify(u_itr, _self, [&](auto &c) {
            c.lastac = now;
            c.mask = umake;
            c.box += box_num;
        });
    }

    uint32_t time_add = (uint32_t)30 * box_num;

    time_point_sec end_time{pool->end.sec_since_epoch() + time_add};


    time_point_sec end_max{current_time_point().sec_since_epoch() + MAX_GAME_SEC};

    if (end_time > end_max)
    {
        end_time = end_max;
    }

    _pool.modify(pool, same_payer, [&](auto &p) {
        p.mask = pool_mask;
        p.box = total_box;
        p.end = end_time;
        p.last = user;
        p.big_prize += big_prize;
        p.last100_prize += big_prize;

    });

    // send ref fee
    action(
        permission_level{_self, "active"_n},
        name("eosio.token"),
        name("transfer"),
        make_tuple(_self, ref, ref_fee, string("Just Game Ref Fee")))
        .send();

    // send contract fee
    // action(
    //     permission_level{_self, "active"_n},
    //     name("eosio.token"),
    //     name("transfer"),
    //     make_tuple(_self, DEV_ACCOUNT, contract_fee, string("Just Game Contract Fee")))
    //     .send();
}

// 处理开盒子逻辑
void eosjustgame1::open(name user)
{
    require_auth(user);

    const auto &pool = _pool.begin();

    auto u_itr = _players.find(user.value);

    check(u_itr != _players.end(), "player not exists");
    check(u_itr->box > 0, "you don'thave box");

    auto b = pool->mask * u_itr->box / base;

    uint64_t profit = b - u_itr->mask;

    print("profit : ", profit, "\n");

    check(b >= profit, "profit overflow");

    if (profit > 0)
    {
        action(
            permission_level{_self, "active"_n},
            "eosio.token"_n, "transfer"_n,
            make_tuple(_self, user, asset(profit, symbol("EOS", 4)), string("open box reward.")))
            .send();

        _players.modify(u_itr, _self, [&](auto &c) {
            c.mask += profit;
        });
    }
}

// 处理复投逻辑
void eosjustgame1::upgrade(name user)
{
    require_auth(user);

    const auto &pool = _pool.begin();

    auto u_itr = _players.find(user.value);

    check(u_itr != _players.end(), "player not exists");
    check(u_itr->box > 0, "you don'thave box");

    auto b = pool->mask * u_itr->box / base;

    uint64_t profit = b - u_itr->mask;

    print("profit : ", profit, "\n");

    check(b >= profit, "profit overflow");

    uint64_t box_num = profit / 1000;

    check(box_num > 0, "can not upgrade box");

    _players.modify(u_itr, _self, [&](auto &c) {
        c.mask += profit;
    });

    buy(user, asset(profit, symbol("EOS", 4)), DEV_ACCOUNT);
}

// 30s interval
void eosjustgame1::rungame()
{
    require_auth(_self);

    const auto &pool = _pool.begin();

    static const time_point_sec now{current_time_point().sec_since_epoch()};

    if (pool->end <= now)
    {
        // 游戏结束，分发大奖和100名奖励
        // pay last winner
        name winner = pool->last;

        asset reward = asset(pool->big_prize, symbol("EOS", 4));

        action(
            permission_level{_self, "active"_n},
            "eosio.token"_n, "transfer"_n,
            make_tuple(_self, winner, reward, string("last winner reward.")))
            .send();

        transaction trx{};
        trx.actions.emplace_back(
            permission_level{_self, "active"_n},
            _self,
            name("payout"),
            std::make_tuple());
        trx.send(next_oid(), _self);
    }
    else
    {
        transaction trx{};
        trx.actions.emplace_back(
            permission_level{_self, "active"_n},
            _self,
            name("rungame"),
            std::make_tuple());
        trx.delay_sec = 5;
        trx.send(next_oid(), _self);
    }
}

void eosjustgame1::payout()
{
    require_auth(_self);

    const auto &pool = _pool.begin();

    static const time_point_sec now{current_time_point().sec_since_epoch()};

    auto player_idx = _players.get_index<"bylastac"_n>();

    int pay_count = pool->pay_count;

    for (uint16_t i = 0; i < 2; ++i)
    {
        auto itr = player_idx.begin();

        if (itr == player_idx.end())
        {
            break;
        }

        if (pay_count < 100)
        {
            pay_count++;
            // 100名大奖
            asset reward = asset(pool->last100_prize / 100, symbol("EOS", 4));

            action(
                permission_level{_self, "active"_n},
                "eosio.token"_n, "transfer"_n,
                make_tuple(_self, itr->user, reward, string("100 winner reward.")))
                .send();
        }

        // 单纯开箱奖励, 减半
        auto b = pool->mask * itr->box / base;

        uint64_t profit = b - itr->mask;

        if (b >= profit && profit > 0)
        {
            uint64_t reward = profit / 2;
            if (reward > 0)
            {
                action(
                    permission_level{_self, "active"_n},
                    "eosio.token"_n, "transfer"_n,
                    make_tuple(_self, itr->user, asset(reward, symbol("EOS", 4)), string("open box reward.")))
                    .send();
            }
        }

        player_idx.erase(itr);
    }

    _pool.modify(pool, same_payer, [&](auto &p) {
        p.pay_count = pay_count;
    });

    auto itr = player_idx.begin();
    if (itr != player_idx.end())
    {
        transaction trx{};
        trx.actions.emplace_back(
            permission_level{_self, "active"_n},
            _self,
            name("payout"),
            std::make_tuple());
        trx.send(next_oid(), _self);
    }
}

void eosjustgame1::check_game_available()
{
    const auto &pool = _pool.begin();
    check(pool != _pool.end(), "game no init");
    static const time_point_sec now{current_time_point().sec_since_epoch()};
    check(pool->start <= now, "game not start");
    check(pool->end >= now, "game is end.");
}

void eosjustgame1::init(uint32_t start)
{
    require_auth(_self);

    const auto &pool = _pool.begin();

    check(pool == _pool.end(), "game has init");

    check(current_time_point().sec_since_epoch()  < start, "invalid start time");

    uint32_t start_sec = start;

    static const time_point_sec create_time{start_sec};
    static const time_point_sec expire_time{start_sec + MAX_GAME_SEC};

    _pool.emplace(_self, [&](auto &c) {
        c.id = 1;
        c.start = create_time;
        c.end = expire_time;
    });

    transaction trx{};
    trx.actions.emplace_back(
        permission_level{_self, "active"_n},
        _self,
        name("rungame"),
        std::make_tuple());
    trx.delay_sec = 5;
    trx.send(next_oid(), _self);
}

void eosjustgame1::clean()
{
    require_auth(_self);

    const auto &pool = _pool.begin();

    for (auto i = 0; i < 10; i++)
    {
        auto pool_itr = _pool.begin();
        if (pool_itr != _pool.end())
        {
            _pool.erase(pool_itr);
        }

        auto p_itr = _players.begin();
        if (p_itr != _players.end())
        {
            _players.erase(p_itr);
        }
    }
}

extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
    auto self = receiver;

    if (code == self)
    {
        switch (action)
        {
            EOSIO_DISPATCH_HELPER(eosjustgame1, (open)(upgrade)(rungame)(payout)(init)(clean))
        }
    }
    else
    {
        if (action == name("transfer").value && code == name("eosio.token").value)
        {
            eosjustgame1 ptop(name(receiver), name(code), datastream<const char *>(nullptr, 0));
            const auto t = unpack_action_data<transfer_args>();
            ptop.handle_transfer(t.from, t.to, t.quantity, t.memo, name(code));
        }
    }
}
}