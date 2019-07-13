#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

using namespace eosio;
using namespace std;

struct transfer_args
{
      name from;
      name to;
      asset quantity;
      string memo;
};

struct token
{
      token(name tkn) : _self(tkn) {}

      struct account
      {
            asset balance;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
      };

      struct currency_stats
      {
            asset supply;
            asset max_supply;
            name issuer;

            uint64_t primary_key() const { return supply.symbol.code().raw(); }
      };

      typedef eosio::multi_index<"accounts"_n, account> accounts;
      typedef eosio::multi_index<"stat"_n, currency_stats> stats;
      name _self;

      asset get_supply(symbol_code sym_code)
      {
            stats statstable(_self, sym_code.raw());
            const auto &st = statstable.get(sym_code.raw());
            return st.supply;
      }

      asset get_balance(name owner, symbol_code sym_code)
      {
            accounts accountstable(_self, owner.value);
            const auto &ac = accountstable.get(sym_code.raw());
            return ac.balance;
      }
};