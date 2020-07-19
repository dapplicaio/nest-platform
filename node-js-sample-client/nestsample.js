const globals = require("./global_constants");

//####################################################################################
//##      Game functions
//####################################################################################

//register game
async function register_game() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "gameregister",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //your WAX account
            gameurl: "", //URL to your game site
            keywords: ["", ""], //array of keywords
            description: "", //game's description
            screenshots: [""], //array of URLs with game screenshots(at least 1)
            videos: [""], //array of URLs with game videos(can be empty)
            promscrshots: [""], //array of URLs with game promotional(can be empty)
            price: "0 WAX", //Game's price
            realisedate: "10 october 2123", //Realise date
            data: "{}", //Another data on JSON-format
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//update game
async function update_game() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "gameupdate",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            gameid: 0, //Your game's id
            owner: globals.WAX_ACCOUNT, //your WAX account
            gameurl: "", //URL to your game site
            keywords: ["", ""], //array of keywords
            description: "", //game's description
            screenshots: [""], //array of URLs with game screenshots(at least 1)
            videos: [""], //array of URLs with game videos(can be empty)
            promscrshots: [""], //array of URLs with game promotional(can be empty)
            price: "0 WAX", //Game's price
            realisedate: "10 october 2123", //Realise date
            data: "{}", //Another data on JSON-format
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//####################################################################################
//##      Achievements functions
//####################################################################################

//create achievement
async function create_achievement() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "achievcreate",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            gameid: 0, //Your game's id
            achievname: "", //Achievement name
            description: "", //Description of achievement
            image: "", //URL to achievement's image
            rarity: "", //Achievement's rarity(check rarities on our documentation)
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//delete achievement(can be deleted only if nobody has it)
async function delete_achievement() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "achievdelete",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            achieveid: 0, //Achievement's id on TABLE achievements
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//issuance of achievements
//add xp of this achievement to user's account and update level
async function issue_achievement() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "achievearn",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            username: "", //WAX account of user to earn achievement
            achieveid: 0, //Achievement's id on TABLE achievements
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//####################################################################################
//##      Cards functions
//####################################################################################

//create card
async function create_card() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "cardcreate",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            gameid: 0, //Your game's id
            cardname: "", //Card's name
            enableimg: "", //Card's image
            disableimg: "", //Card's image when user hasn't it(BW image)
            series: "", //Card series' name
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//delete card(can be deleted only if nobody has it)
async function delete_card() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "carddelete",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            cardid: 0, //Card's id on TABLE cards
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//Drop card to user
//Get NFT card to user and create it on user account
async function drop_card() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "carddrop",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            cardid: 0, //Card's id on TABLE cards
            username: "", //User WAX account
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//####################################################################################
//##      Badges functions
//####################################################################################

//create badge
async function create_badge() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "badgecreate",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            gameid: 0, //Your game's id
            badgename: "", //Badge's name
            cardids: [], //Array with cards' ids, witch included on badge, from TABLE cards
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//delete badge(can be deleted only if nobody has it)
async function delete_badge() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "badgedelete",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            badgeid: 0, //Card's id on TABLE cards
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//####################################################################################
//##      Leader board functions
//####################################################################################

//create leader board
async function create_leader_board() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "createlboard",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            boardname: "", //Leader board's name
            gameid: 0, //Your game's id
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//create leader board's prize
async function create_prize() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "createprize",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            boardid: 0, //Leader board's id from lboards TABLE
            mode: 0, //Prize's mode: 0 - absolute values, 1 - percents
            value: "", // String of values like "100,50,10,"
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//reset leader board
async function reset_leader_board() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "resetlboard",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //Your WAX account
            boardid: 0, //Leader board's id from lboards TABLE
            resetpool: 0, //1 - reset prize pool, 0 - don't reset prize pool
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//remove leader board
async function reset_leader_board() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "removelboard",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            boardid: 0, //Leader board's id from lboards TABLE
            owner: globals.WAX_ACCOUNT, //Your WAX account
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}

//update leader board
async function update_leader_board() {
  try {
    await globals.WAX.transaction({
      actions: [
        {
          account: globals.NEST_CONTRACT,
          name: "update",
          authorization: [
            {
              actor: globals.WAX_ACCOUNT,
              permission: "active",
            },
          ],
          data: {
            owner: globals.WAX_ACCOUNT, //owner wax account
            boardid: 0, //board id on table lboards
            username: "", //user's wax account, whose record update
            point: 0, //number of points to add
            data: "{}", //user data on JSON format
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}
