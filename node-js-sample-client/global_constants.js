//add eosjs library
const Eos = require("eosjs");

//initialize WAX
WAX = Eos({
  keyProvider: "", // private key of your WAX account
  httpEndpoint: "https://chain.wax.io",
  chainId: "1064487b3cd1a897ce03ae5b6a865651747e2e152090f99c1d19d44e01aea5a4",
});

const NEST_CONTRACT = "nestcontract";
const WAX_ACCOUNT = ""; //your WAX account's name

module.exports = {
  Eos: Eos,
  WAX: WAX,
  WAX_ACCOUNT: WAX_ACCOUNT,
  NEST_CONTRACT: NEST_CONTRACT
};
