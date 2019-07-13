import { Api, JsonRpc } from 'eosjs' // https://github.com/EOSIO/eosjs
import JsSignatureProvider from 'eosjs/dist/eosjs-jssig'

export default class EOSIOClient {
  constructor (contractAccount) {
    const rpc = new JsonRpc(process.env.REACT_APP_EOSIO_HTTP_URL)
    const signatureProvider = new JsSignatureProvider([process.env.REACT_APP_EOSIO_PRIVATE_KEY])
    this.contractAccount = contractAccount
    this.eos = new Api({ rpc, signatureProvider })
  }

  transaction = (actor, action, data) => {
    return this.eos.transact({
      actions: [
        {
          account: this.contractAccount,
          name: action,
          authorization: [
            {
              actor,
              permission: 'active'
            }
          ],
          data: {
            ...data
          }
        }
      ]
    }, {
      blocksBehind: 3,
      expireSeconds: 30
    })
  }
}