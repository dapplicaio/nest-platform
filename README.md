## Smart Contract actions
 ```c++
    ////////////////////////////////////////////////////////////////////////////////
    //        nestcontract ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION createlboard(eosio::name owner, string boardname, uint64_t gameid);
    ACTION createprize(eosio::name owner, uint64_t boardid, uint8_t mode, string value);
    ACTION resetlboard(eosio::name owner, uint64_t boardid, bool resetpool);
    ACTION removelboard(uint64_t boardid, eosio::name owner);
    ACTION update(eosio::name owner, uint64_t boardid, eosio::name username, double points, string data);


    ////////////////////////////////////////////////////////////////////////////////
    //        GAME ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION gameregister(eosio::name  owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string realisedate, std::string data);
    ACTION gameupdate(uint64_t gameid, eosio::name  owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string realisedate, std::string data);
    ACTION gameapprove(uint64_t appgameid, bool approve);



    ////////////////////////////////////////////////////////////////////////////////
    //        USER ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION usercreate(eosio::name username);
    ACTION userregister(eosio::name username,std::string avatar, std::string nickname);
    ACTION userupdate(eosio::name username,std::string avatar, std::string nickname);
    ACTION userapprove(eosio::name username, bool approve);
    ACTION addfriend(eosio::name username, eosio::name newfriend);
    ACTION deletefriend(eosio::name username, eosio::name delfriend);
    


    ////////////////////////////////////////////////////////////////////////////////
    //        ACHIEVEMENTS ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION achievcreate(eosio::name owner, uint64_t gameid, std::string achievname, std::string description, std::string image, std::string rarity);
    ACTION achievdelete(eosio::name owner, uint64_t achieveid);
    ACTION achievearn(eosio::name owner, eosio::name username, uint64_t achieveid);
    ACTION achievapprove(uint64_t achieveid, bool approve);



    ////////////////////////////////////////////////////////////////////////////////
    //        CARDS ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION cardcreate(eosio::name owner, uint64_t gameid, std::string cardname, std::string enableimg, std::string disableimg, std::string series);
    ACTION carddelete(eosio::name owner, uint64_t cardid);
    ACTION carddrop(eosio::name owner, uint64_t cardid, eosio::name username);
    ACTION cardapprove(uint64_t cardid, bool approve);



    ////////////////////////////////////////////////////////////////////////////////
    //        BADGES ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION badgecreate(eosio::name owner, uint64_t gameid, std::string badgename, std::vector<uint64_t> cardids);
    ACTION badgedelete(eosio::name owner, uint64_t badgeid);
    ACTION badgeapprove(uint64_t gameid, bool approve);
    ACTION badgeearn(eosio::name username, uint64_t badgeid);



    ////////////////////////////////////////////////////////////////////////////////
    //        LEVEL ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION lvlcreate(uint64_t level, uint64_t xp);
    ACTION lvlupdate(uint64_t level, uint64_t xp);
    ACTION lvlearn(eosio::name username);
    ACTION lvladdxp(eosio::name username, uint64_t xp);
```

## Smart Contract structures & tables

```c++
    struct player_s
    {
      eosio::name               account;            //user wax account
      double                    points;             //user points
      string                    data;               //user data on JSON
      double                    payout;             //user payouts
    };

    struct prize_s
    {
      uint8_t                   mode;               //prize mode: 0 - absolute numbers, 1 - percents
      vector<double>            values;             //users' prizes parts
    };

    struct series_s
    {
      std::string               series;             //series name
      std::vector<uint64_t>     cards;              //cards' ids of budge's cards
    };

    struct achiev_s
    {
      uint64_t                  gameid;             //game id table
      uint8_t                   maxachiev;          //number of achievements of current game
      std::vector<uint64_t>     achievs;            //array of achievements
    };

    TABLE global
    {
      global() {}
	    uint64_t cardid{0};
      uint64_t badgeid{0};
      uint64_t achievid{0};
      uint64_t gameid{0};
      uint64_t lboardid{0};
      EOSLIB_SERIALIZE(global, (cardid)(badgeid)(achievid)(gameid)(lboardid))
    };
    
    TABLE lboard {
      uint64_t                  id;                 //nestcontract id on table
      eosio::name               owner;              //owner wax account name
      string                    boardname;          //name of current board
      uint64_t                  gameid;             //game's id
      double                    pot;                //total pot on current nestcontract
      vector<player_s>          players;            //array of players
      prize_s                   prize;              //array and type of prizes

      uint64_t primary_key() const { return id; }
    };

    TABLE game
    {
      uint64_t                  id{0};              //game id on games table
      eosio::name               owner;              //owner WAX account
      std::string               gameurl;            //game URL
      std::vector<std::string>  keywords;           //array of game keywords
      std::string               description;        //game description
      std::vector<std::string>  screenshots;        //array of game screenshots URLs 
      std::vector<std::string>  videos;             //array of game videos URLs 
      std::vector<std::string>  promscrshots;       //array of promotional screenshots URLs 
      eosio::asset              price;              //game price on WAX
      std::vector<std::string>  customtitles;       //array of user's custom titles
      std::vector<uint64_t>     achievements;       //array of achievements' IDs on TABLE achievements
      std::vector<uint64_t>     badges;             //array of badges' IDs on TAVLE badges
      std::string               realisedate;        //realise date on string format
      std::string               data;               //game data on JSON format

      uint64_t primary_key() const {return id;}
    };

    TABLE apprgame
    {
      uint64_t                  id{0};              //game id on games table
      eosio::name               owner;              //owner WAX account
      std::string               gameurl;            //game URL
      std::vector<std::string>  keywords;           //array of game keywords
      std::string               description;        //game description
      std::vector<std::string>  screenshots;        //array of game screenshots URLs 
      std::vector<std::string>  videos;             //array of game videos URLs 
      std::vector<std::string>  promscrshots;       //array of promotional screenshots URLs 
      eosio::asset              price;              //game price on WAX
      std::vector<std::string>  customtitles;       //array of user's custom titles
      std::vector<uint64_t>     achievements;       //array of achievements' IDs on TABLE achievements
      std::vector<uint64_t>     badges;             //array of badges' IDs on TAVLE badges
      std::string               realisedate;        //realise date on string format
      std::string               data;               //game data on JSON format

      uint64_t primary_key() const {return id;}
    };

    TABLE user
    {
      eosio::name               account;            //user's WAX account
      std::string               nickname;           //user's nickname
      std::string               avatar;             //user's avatar URL
      uint64_t                  level{0};           //current user's level
      uint64_t                  xp{0};              //current user's points
      std::vector<achiev_s>     achievements;       //array of user's achievements
      std::vector<eosio::name>  friends;            //array of user's friends(WAX accounts)

      uint64_t primary_key() const {return account.value;}
    };

    TABLE appruser
    {
      eosio::name               account;            //user's WAX account
      std::string               nickname;           //user's nickname
      std::string               avatar;             //user's avatar URL

      uint64_t primary_key() const {return account.value;}
    };

    TABLE achievement
    {
      uint64_t                  id{0};              //achievement's id
      uint64_t                  gameid{0};          //card's owner game id
      std::string               achievname;         //achievement's name
      std::string               description;        //achievement's description
      std::string               image;              //achievement's image URL
      std::string               rarity;             //achievement's rarity
      bool                      candelete;          //achievement can be deleted only if nobody has it

      uint64_t primary_key() const { return id;}
    };

    TABLE appracheiv
    {
      uint64_t                  id{0};              //achievement's id on TABLE
      uint64_t                  gameid{0};          //card's owner game id
      std::string               achievname;         //achievement's name
      std::string               description;        //achievement's description
      std::string               image;              //achievement's image URL
      std::string               rarity;             //achievement's rarity

      uint64_t primary_key() const { return id;}
    };

    TABLE card
    {
      uint64_t                  id{0};              //card's TABLE id
      uint64_t                  gameid{0};          //card's owner game id
      std::string               cardname;           //card's name
      std::string               enableimg;          //card's enable image URL
      std::string               disableimg;         //card's disable image URL
      std::string               series;             //card's series name
      bool                      candelete{1};       //card can be deleted only if nobody has it

      uint64_t primary_key() const { return id;}
    };    
    
    TABLE apprcard
    {
      uint64_t                  id{0};              //card's TABLE id
      uint64_t                  gameid{0};          //card's owner id
      std::string               cardname;           //card's name
      std::string               enableimg;          //card's enable image URL
      std::string               disableimg;         //card's disable image URL
      std::string               series;             //card's series name

      uint64_t primary_key() const { return id;}
    };

    TABLE badge
    {
      uint64_t                  id{0};              //badge TABLE id
      uint64_t                  gameid{0};          //badge's owner id
      std::string               badgename;          //badge's name
      std::vector<uint64_t>     cardids;            //array of cards which includes on badge
      bool                      candelete{1};       //badge can be deleted only if nobody has it

      uint64_t primary_key() const { return id; }
    };    
    
    TABLE apprbadge
    {
      uint64_t                  id{0};              //badge TABLE id
      uint64_t                  gameid{0};          //badge's owner id
      std::string               badgename;          //badge's name
      std::vector<uint64_t>     cardids;            //array of cards which includes on badge

      uint64_t primary_key() const { return id; }
    };

    TABLE level
    {
      uint64_t                  lvl{0};             //level number
      uint64_t                  points{0};          //points to get level

      uint64_t primary_key() const {return lvl;}
    };

    TABLE gamecard
    {
      uint64_t                  gameid{0};           //game id
      std::vector<series_s>     series;              //array with card seria data

      uint64_t primary_key() const {return gameid;}
    };

    TABLE usercard
    {
      eosio::name               username;             //user's wax account
      std::vector<uint64_t>     cardids;              //array of owned cards' ids

      uint64_t primary_key() const {return username.value;}
    };
```

## Sample js calls

```JavaScript
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
            realisedate: "10 october 2123", //Release date
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
            rarity: "", //Achievement's rarity
          },
        },
      ],
    });
  } catch (error) {
    console.log(error);
  }
}
```
