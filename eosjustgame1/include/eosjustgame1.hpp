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
      uint32_t ORDER_EXPIRE_SEC = 3600;

      eosjustgame1(name receiver, name code, datastream<const char *> ds)
          : contract(receiver, code, ds),
            _globals(_self, _self.value)
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

      ACTION open(name user);

      ACTION claim(name user);

      ACTION upgrade(name user);

    private:
      TABLE player
      {
            name user;
            uint64_t box;
            uint64_t eos;
            uint64_t mask;
      };

      TABLE global_var
      {
            name key;
            uint64_t val;

            uint64_t primary_key() const { return key.value; }
            EOSLIB_SERIALIZE(global_var, (key)(val))
      };

      typedef multi_index<"globals"_n, global_var> globals;
      globals _globals;

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
            check(sep_count == 1, "invalid memo");

            size_t pos;
            string container;

            pos = sub2sep(memo, &container, '-', 0, true);
            
            *action = container;

            *ref = name(memo.substr(++pos));
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