#pragma once
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include <eosio/singleton.hpp>

using namespace std;

#define CONTRACTN eosio::name("nestplatform")
#define SIMPLEASSETSCONTRACT eosio::name("simpleassets")

CONTRACT nestplatform : public eosio::contract {
  public:
    using contract::contract;

    ////////////////////////////////////////////////////////////////////////////////
    //        nestplatform ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION createlboard(eosio::name owner, string boardname, uint64_t gameid);
    ACTION createprize(uint64_t boardid, uint8_t mode, string value);
    ACTION resetlboard(uint64_t boardid, bool resetpool);
    ACTION removelboard(uint64_t boardid);
    ACTION update(uint64_t boardid, eosio::name username, double points, string data);


    ////////////////////////////////////////////////////////////////////////////////
    //        GAME ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION gameregister(eosio::name  owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string releasedate, std::string data);
    ACTION gameupdate(uint64_t gameid, eosio::name  new_owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string releasedate, std::string data);
    ACTION gameapprove(uint64_t appgameid, bool approve);
    //ACTION gamerate(eosio::name username, uint64_t gameid, uint8_t rate, std::string comment);


    ////////////////////////////////////////////////////////////////////////////////
    //        USER ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION usercreate(eosio::name author,eosio::name username);
    ACTION userregister(eosio::name username,std::string avatar, std::string nickname);
    ACTION userupdate(eosio::name username,std::string avatar, std::string nickname);
    ACTION userapprove(eosio::name username, bool approve);
    ACTION addfriend(eosio::name username, eosio::name newfriend);
    ACTION deletefriend(eosio::name username, eosio::name delfriend);
    ACTION applyfriend(eosio::name username, eosio::name newfriend);
    


    ////////////////////////////////////////////////////////////////////////////////
    //        ACHIEVEMENTS ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION achievcreate(eosio::name owner, uint64_t gameid, std::string achievname, uint64_t xp, std::string description, std::string image, std::string rarity);
    ACTION achievxp(uint64_t id, uint64_t xp);
    ACTION achievdelete(uint64_t achieveid);
    ACTION achievearn(eosio::name username, uint64_t achieveid);
    ACTION achievapprove(uint64_t achieveid, bool approve);



    ////////////////////////////////////////////////////////////////////////////////
    //        CARDS ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION cardcreate(eosio::name owner, uint64_t gameid, std::string cardname, std::string enableimg, std::string disableimg, std::string series);
    ACTION carddelete( uint64_t cardid);
    ACTION carddrop(uint64_t cardid, eosio::name username);
    ACTION cardapprove(uint64_t cardid, bool approve);
    ACTION setupdrop(uint8_t maxdrops, uint8_t maxonseries);



    ////////////////////////////////////////////////////////////////////////////////
    //        BADGES ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION badgecreate(eosio::name owner, uint64_t gameid, std::string badgename, std::vector<uint64_t> cardids);
    ACTION badgedelete(uint64_t badgeid);
    ACTION badgeapprove(uint64_t gameid, bool approve);
    ACTION badgeearn(eosio::name username, uint64_t badgeid);



    ////////////////////////////////////////////////////////////////////////////////
    //        LEVEL ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION lvlcreate(uint64_t level, uint64_t xp);
    ACTION lvlupdate(uint64_t level, uint64_t xp);
    ACTION lvlearn(eosio::name username);
    ACTION lvladdxp(eosio::name username, uint64_t xp);


    ////////////////////////////////////////////////////////////////////////////////
    //        INGAME ACTIONS
    ////////////////////////////////////////////////////////////////////////////////
    ACTION startround(eosio::name username, uint64_t gameid);


  private:

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
      std::vector<uint64_t>     cards;              //cards' ides of budge's cards
    };

    struct achiev_s
    {
      uint64_t                  gameid;             //game id on table
      uint8_t                   maxachiev;          //number of achievements on current game
      std::vector<uint64_t>     achievs;            //array of achievements
    };

    struct card_s
    {
      uint64_t                  card_id;            //card id on table
      uint32_t                  amount;             //number of cards
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
    typedef eosio::singleton< "global"_n, global> conf;
    global _cstate;
    

    TABLE lboard {
      uint64_t                  id;                 //nestplatform id on table
      eosio::name               owner;              //owner wax account name
      string                    boardname;          //name of current board
      uint64_t                  gameid;             //game's id
      double                    pot;                //total pot on current nestplatform
      vector<player_s>          players;            //array of players
      prize_s                   prize;              //array and type of prizes

      uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<eosio::name("lboards"), lboard> lboards;

    TABLE gametbl
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
      std::string               releasedate;        //realise date on string format
      std::string               data;               //game data on JSON format

      uint64_t primary_key() const {return id;}
    };

    typedef eosio::multi_index<eosio::name("gametbls"), gametbl> games;

    TABLE apprgametbl
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
      std::string               releasedate;        //realise date on string format
      std::string               data;               //game data on JSON format

      uint64_t primary_key() const {return id;}
    };

    typedef eosio::multi_index<eosio::name("apprgametbls"), apprgametbl> apprgames;

    TABLE rategame
    {
      uint64_t                  gameid{0};          //game id on games table
      uint64_t                  totalstars{0};      //total stars on game
      uint64_t                  totalvotes{0};      //total votes on game
      double                    rate{0.0};          //game's rate
      std::vector<std::string>  coments;            //coments on game

      uint64_t primary_key() const {return gameid;}
    };

    typedef eosio::multi_index<eosio::name("rategames"), rategame> rategames;

    TABLE user
    {
      eosio::name               account;            //user's WAX account
      std::string               nickname;           //user's nickname
      std::string               avatar;             //user's avarat URL
      uint64_t                  level{0};           //current user's level
      uint64_t                  xp{0};              //current user's points
      std::vector<achiev_s>     achievements;       //array of user's achievements
      std::vector<eosio::name>  friends;            //array of user's friends(WAX accounts)

      uint64_t primary_key() const {return account.value;}
    };

    typedef eosio::multi_index<eosio::name("users"), user> users;

    TABLE appruser
    {
      eosio::name               account;            //user's WAX account
      std::string               nickname;           //user's nickname
      std::string               avatar;             //user's avarat URL

      uint64_t primary_key() const {return account.value;}
    };

    typedef eosio::multi_index<eosio::name("apprusers"), appruser> apprusers;

    TABLE friendapply
    {
      eosio::name               account;            //user's WAX account
      std::vector<eosio::name>  friends;            //user's friends to apply

      uint64_t primary_key() const {return account.value;}
    };

    typedef eosio::multi_index<eosio::name("friendapplys"), friendapply> friendapplys;

    TABLE achiev
    {
      uint64_t                  id{0};              //achievement's id on TABLE
      uint64_t                  gameid{0};          //card's owner game id
      std::string               achievname;         //achievement's name
      std::string               description;        //achievement's description
      std::string               image;              //achievement's image URL
      uint64_t                  xp{0};              //achievement's xp
      std::string               rarity;             //achievement's rarity
      bool                      candelete;          //achievement can be deleted only if nobody has it

      uint64_t primary_key() const { return id;}
    };

    typedef eosio::multi_index<eosio::name("achievs"), achiev> achievements;

    TABLE appracheiv
    {
      uint64_t                  id{0};              //achievement's id on TABLE
      uint64_t                  gameid{0};          //card's owner game id
      std::string               achievname;         //achievement's name
      std::string               description;        //achievement's description
      std::string               image;              //achievement's image URL
      uint64_t                  xp{0};              //achievement's xp
      std::string               rarity;             //achievement's rarity

      uint64_t primary_key() const { return id;}
    };

    typedef eosio::multi_index<eosio::name("appracheivs"), appracheiv> appracheivs;

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

    typedef eosio::multi_index<eosio::name("cards"), card> cards;
    
    
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

    typedef eosio::multi_index<eosio::name("apprcards"), apprcard> apprcards;

    TABLE badge
    {
      uint64_t                  id{0};              //badge TABLE id
      uint64_t                  gameid{0};          //badge's owner id
      std::string               badgename;          //badge's name
      std::vector<uint64_t>     cardids;              //array of cards which includes on badge
      bool                      candelete{1};       //badge can be deleted only if nobody has it

      uint64_t primary_key() const { return id; }
    };

    typedef eosio::multi_index<eosio::name("badges"), badge> badges;
    
    
    TABLE apprbadge
    {
      uint64_t                  id{0};              //badge TABLE id
      uint64_t                  gameid{0};          //badge's owner id
      std::string               badgename;          //badge's name
      std::vector<uint64_t>     cardids;              //array of cards which includes on badge

      uint64_t primary_key() const { return id; }
    };

    typedef eosio::multi_index<eosio::name("apprbadges"), apprbadge> apprbadges;

    TABLE level
    {
      uint64_t                  lvl{0};             //level number
      uint64_t                  points{0};          //points to get level

      uint64_t primary_key() const {return lvl;}
    };

    typedef eosio::multi_index<eosio::name("levels"), level> levels;

    TABLE gamecard
    {
      uint64_t                  gameid{0};           //game id
      std::vector<series_s>     series;              //array with card series data

      uint64_t primary_key() const {return gameid;}
    };

    typedef eosio::multi_index<eosio::name("gamecards"), gamecard> gamecards;

    TABLE usercardd
    {
      eosio::name               username;             //user's wax account
      std::vector<card_s>       cardids;              //array of owned cards' ids

      uint64_t primary_key() const {return username.value;}
    };

    typedef eosio::multi_index<eosio::name("usercardds"), usercardd> usercards;

    TABLE userbadge
    {
      eosio::name               username;             //user's wax account
      std::vector<uint64_t>     badge_id;             //array of owned cards' ids

      uint64_t primary_key() const {return username.value;}
    };

    typedef eosio::multi_index<eosio::name("userbadges"), userbadge> userbadges;

    TABLE carddropset
    {
      carddropset() {}
	    uint8_t maxdrop{0};
      uint8_t maxonseries{0};
      EOSLIB_SERIALIZE(carddropset, (maxdrop)(maxonseries))
    };
    typedef eosio::singleton< eosio::name("carddropset"), carddropset> carddrset;
    carddropset _cdrops;

  public:
  
  nestplatform(eosio::name receiver, eosio::name code, eosio::datastream<const char *> ds) : contract(receiver, code, ds) {}
  int finder(std::vector<player_s> players, eosio::name username);
  int finder(std::vector<eosio::name> friends, eosio::name username);
  int finder(std::vector<achiev_s> achieves, uint64_t gameid);
  int finder(std::vector<series_s> series, std::string ser_name);
  int finder(std::vector<card_s> cards, uint64_t card_id);
  int finder(std::vector<uint64_t> ids, uint64_t id);
  uint64_t get_achiev_points(std::string rarity);
  uint64_t getcardid();
  uint64_t getbadgeid();
  uint64_t getachievid();
  uint64_t getgameid();
  uint64_t getlboardid();
   
};
