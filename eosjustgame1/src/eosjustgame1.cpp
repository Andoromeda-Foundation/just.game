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
        ref = name("justgamedev1");
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

    // const auto& pool = _rexpool.begin(); /// already checked that _rexpool.begin() != _rexpool.end() in rex_loans_available()

    // idx.modify(itr, same_payer, [&](auto &loan) {
    //     loan.total_staked.amount = rented_tokens;
    //     loan.expiration         = loan.expiration  +  eosio::seconds(30) * box_num;
    //     loan.balance.amount -= loan.payment.amount;
    // });
}

// 处理开盒子逻辑
void eosjustgame1::open(name user)
{
    require_auth(user);
}

// 处理复投逻辑
void eosjustgame1::upgrade(name user)
{
    require_auth(user);
}

// 用户调用open就可以取走钱。claim应该是项目分调用取走项目分所得？
void eosjustgame1::claim(name user)
{
    require_auth(user);
}

// 30s interval
void eosjustgame1::rungame()
{
    require_auth(_self);
}

void eosjustgame1::check_game_available()
{
    const auto &pool = _pool.begin();

    check(pool != _pool.end(), "game no init");

    static const time_point_sec now{current_time_point().sec_since_epoch()};

    check(pool->start <= now, "game not start");

    check(pool->end >= now, "game is end.");
}

void eosjustgame1::init()
{
    require_auth(_self);

    const auto &pool = _pool.begin();

    check(pool == _pool.end(), "game has init");

    uint32_t EXPIRE_SEC = 60 * 60 * 12; // 订单有效时

    uint32_t start_sec = 1563019200;

    static const time_point_sec create_time{start_sec};
    static const time_point_sec expire_time{start_sec + EXPIRE_SEC};

    _pool.emplace(_self, [&](auto &c) {
        c.id = 1;
        c.start = create_time;
        c.end = expire_time;
    });
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
            EOSIO_DISPATCH_HELPER(eosjustgame1, (open)(upgrade)(claim)(rungame)(init)(clean))
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