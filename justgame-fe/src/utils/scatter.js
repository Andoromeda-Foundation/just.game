import ScatterJS from '@scatterjs/core';
import ScatterEOS from '@scatterjs/eosjs';
import Eos from 'eosjs';
import currentNetwork from './network';

ScatterJS.plugins( new ScatterEOS() );

const network = ScatterJS.Network.fromJson(currentNetwork);

export const eos = () => ScatterJS.eos(network, Eos, { expireInSeconds: 60 });

const API = {
  account() { return ScatterJS.account('eos'); },
  connect() { return ScatterJS.connect('just game', { network }); },
  loginScatterAsync() { return ScatterJS.getIdentity({ accounts: [currentNetwork] }); },
  suggestNetworkAsync() { return ScatterJS.suggestNetwork(currentNetwork); },
};

export default API;