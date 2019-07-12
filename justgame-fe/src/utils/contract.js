import { eos } from './scatter'

// https://github.com/EOSIO/eosjs/tree/v16.0.9

export const CONTRACT_ACCOUNT = 'just game'

export const transfer = ({ account = null, amount = 0, memo = '' }) => {
  if (!account) throw new Error('no account')
  return eos().transaction({
    actions: [
      {
        account: 'eosio.token',
        name: 'transfer',
        authorization: [
          {
            actor: account.name,
            permission: account.authority
          }
        ],
        data: {
          from: account.name,
          to: CONTRACT_ACCOUNT,
          quantity: `${amount.toFixed(4).toString()} EOS`,
          memo
        }
      }
    ]
  })
}

export const open = ({ account = null }) => {
  if (!account) throw new Error('no account')
  return eos().transaction({
    actions: [
      {
        account: CONTRACT_ACCOUNT,
        name: 'claim',
        authorization: [
          {
            actor: account.name,
            permission: account.authority
          }
        ],
        data: { from: account.name }
      }
    ]
  })
}