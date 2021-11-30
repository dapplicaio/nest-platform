#include <eosio/eosio.hpp>
#include "atomicassets.hpp"

static constexpr name CONTRACT_ACCOUNT = "blendingcont"_n;    

class [[eosio::contract]] blends : public contract 
{
  public:
    using contract::contract;

    [[eosio::on_notify("atomicassets::transfer")]] 
    void receive_asset_transfer
    (
      const name& from,
      const name& to,
      std::vector <uint64_t>& asset_ids,
      const std::string& memo
    );


    // ------------------------------------------- OWNER ACTIONS ----------------------------------------------------------------------
    // require auth owner of collection;
    [[eosio::action]]
    void setblend(const name& collection_name, std::vector<uint32_t> requiring_components, const uint32_t& result, const uint32_t& max_supply);

    [[eosio::action]]
    void deleteblend(const name& collection_name, uint64_t blend_id);

    // --------------------------------------------------------------------------------------------------------------------------------
    
  private:

  struct [[eosio::table]] blends_j
  {
    uint64_t                blend_id;
    std::vector<uint32_t>   requiring_components;
    uint32_t                result;
    uint32_t                max_supply; // 0 - inf
    uint32_t                crafted;

    const bool available()const
    {
      if(this->max_supply == 0)
        return true;

      return (max_supply != crafted);
    }

    uint64_t primary_key()const { return blend_id; }
  };
  typedef multi_index< "blends"_n, blends_j > blends_t;

  blends_t get_blends(const name& collection_name)const
  {
    return blends_t(CONTRACT_ACCOUNT,collection_name.value);
  }

  const name check_collection_auth(const name& collection_name);
  void check_contract_auth(const name& collection_name);
  void check_repeated(std::vector<uint32_t>& array);
  const bool check_number(const std::string& number);
    
};
