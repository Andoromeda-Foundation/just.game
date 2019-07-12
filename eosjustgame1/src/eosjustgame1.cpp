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

void eosjustgame1::buy(name user, asset quantity, name ref)
{
    // 处理买盒子逻辑
}

void eosjustgame1::open(name user)
{
    // 处理开盒子逻辑
}

void eosjustgame1::upgrade(name user)
{
    // 处理复投逻辑
}

void eosjustgame1::claim(name user)
{
    // 用户调用open就可以取走钱。claim应该是项目分调用取走项目分所得？
}

extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
    auto self = receiver;

    if (code == self)
    {
        switch (action)
        {
            EOSIO_DISPATCH_HELPER(eosjustgame1, (open)(upgrade)(claim))
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