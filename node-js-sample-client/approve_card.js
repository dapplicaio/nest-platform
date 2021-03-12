const { Api, JsonRpc } = require('eosjs');
const { JsSignatureProvider } = require('eosjs/dist/eosjs-jssig');  // development only
const fetch = require('node-fetch'); //node only
const { TextDecoder, TextEncoder } = require('util'); //node only
const { create } = require('domain');

//TODO add key nestplatform
const privateKeys = [''];

const signatureProvider = new JsSignatureProvider(privateKeys);
const rpc = new JsonRpc('https://chain.wax.io', { fetch }); //required to read blockchain state
const WAX = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() }); //required to submit transactions

const CONTRACT = "nestplatform";
const ATOMCONTRACT = "atomicassets";

async function get_table_cards_approves() {
    try {
        const rows = await rpc.get_table_rows({
            json: true,               // Get the response as json
            code: CONTRACT,      // Contract that we target
            scope: CONTRACT,         // Account that owns the data
            table: 'apprdcards',        // Table name
            reverse: false,           // Optional: Get reversed data
            show_payer: false          // Optional: Show ram payer
        });
        console.log(rows.rows.length);
        return rows.rows;
    } catch (error) {
        console.log(error);
    }
}


async function setuph() {
    const table = await get_table_cards_approves();

    for (let index = 0; index < table.length; index++) {
        const element = table[index];

        try {
           const result = await WAX.transact(
                {
                    actions: [
                    {
                        account: ATOMCONTRACT,
                        name: 'createtempl',
                        authorization: [{
                            actor: CONTRACTT,
                            permission: 'active',
                        }],
                        data: {
                            heroname: element,
                            rarity: 3,
                            lvl1: rare_ids[0],
                            lvl2: rare_ids[1],
                            lvl3: rare_ids[2],
                            lvl4: rare_ids[3],
                        },
                    }]
                },
                {
                blocksBehind: 3,
                expireSeconds: 30,
                });
                console.log("added rare");
    
        } catch (error) {
            console.log("ERROR des status 102 user 1")
            console.log(error);
        }
    }

}


setTimeout(async () => {
    console.log('start');
  
    await setuph();
    console.log('end');
  }, 0 * 1000);