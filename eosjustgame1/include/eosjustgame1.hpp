#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <types.hpp>

using namespace eosio;
using namespace std;

CONTRACT eosjustgame1 : public contract
{
    public:
      uint32_t MAX_GAME_SEC = 60 * 60 * 12;
      // uint32_t MAX_GAME_SEC = 90;

      name DEV_ACCOUNT = name("justgamedevl");

      const uint64_t base = 10000ll;

      eosjustgame1(name receiver, name code, datastream<const char *> ds)
          : contract(receiver, code, ds),
            _globals(_self, _self.value),
            _players(_self, _self.value),
            _pool(_self, _self.value)
      {
            auto oid_itr = _globals.find(name("oid").value);
            if (oid_itr == _globals.end())
            {
                  _globals.emplace(_self, [&](auto &s) {
                        s.key = name("oid");
                        s.val = 1;
                  });
            }
      }

      void handle_transfer(name from, name to, asset quantity, string memo, name code);

      void buy(name user, asset quantity, name ref);

      void check_game_available();

      ACTION rungame();

      ACTION payout();

      ACTION init(uint32_t start);

      ACTION clean();

      ACTION open(name user);

      ACTION upgrade(name user);

    private:
      TABLE global_var
      {
            name key;
            uint64_t val;

            uint64_t primary_key() const { return key.value; }
            EOSLIB_SERIALIZE(global_var, (key)(val))
      };

      typedef multi_index<"globals"_n, global_var> globals;
      globals _globals;

      TABLE pool
      {
            uint64_t id;
            uint64_t mask;
            uint64_t box;

            name last;
            uint64_t pay_count;        // 记录奖励发放次数

            uint64_t big_prize;        // 5% 最后大奖
            uint64_t last100_prize;    // 10% 最后100位奖励

            eosio::time_point_sec start;
            eosio::time_point_sec end;

            uint64_t primary_key() const { return id; }
      };

      typedef multi_index<"pool"_n, pool> pool_index;
      pool_index _pool;

      TABLE player
      {
            name user;
            uint64_t box;
            uint64_t mask;
            bool pay;

            eosio::time_point_sec join;
            eosio::time_point_sec lastac;

            uint64_t primary_key() const { return user.value; }
            uint64_t by_lastac() const { return lastac.sec_since_epoch(); }
      };

      typedef multi_index<"players"_n, player,
                          indexed_by<"bylastac"_n, const_mem_fun<player, uint64_t, &player::by_lastac>>>
          player_table;

      player_table _players;

      uint64_t next_oid()
      {
            auto oid_itr = _globals.find(name("oid").value);

            uint64_t oid = oid_itr->val;

            _globals.modify(oid_itr, _self, [&](auto &s) {
                  s.val++;
            });

            return oid;
      }

      void parse_memo(string memo, string * action, name * ref)
      {
            size_t sep_count = count(memo.begin(), memo.end(), '-');

            if (sep_count == 0)
            {
                  memo.erase(remove_if(memo.begin(), memo.end(), [](unsigned char x) { return isspace(x); }), memo.end());
                  *action = memo;
            }
            else if (sep_count == 1)
            {
                  size_t pos;
                  string container;
                  pos = sub2sep(memo, &container, '-', 0, true);
                  *action = container;
                  *ref = name(memo.substr(++pos));
            }
      }

      size_t sub2sep(const string &input,
                     string *output,
                     const char &separator,
                     const size_t &first_pos = 0,
                     const bool &required = false)
      {
            check(first_pos != string::npos, "invalid first pos");
            auto pos = input.find(separator, first_pos);
            if (pos == string::npos)
            {
                  check(!required, "parse memo error");
                  return string::npos;
            }
            *output = input.substr(first_pos, pos - first_pos);
            return pos;
      }

      void against_contract_robot()
      {
            auto tx_size = transaction_size();
            char tx[tx_size];
            auto read_size = read_transaction(tx, tx_size);
            check(tx_size == read_size, "read_transaction failed");
            auto trx = unpack<transaction>(tx, read_size);
            action first_action = trx.actions.front();
            check(first_action.name.value == name("transfer").value, "wrong action");
            check(first_action.account.value == name("eosio.token").value, "wrong action");
      }
};