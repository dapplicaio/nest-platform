#include <blends.hpp>
#include "OwnersAction.cpp"

void blends::receive_asset_transfer
(
    const name& from,
    const name& to,
    std::vector <uint64_t>& asset_ids,
    const std::string& memo
)
{
    if (to != CONTRACT_ACCOUNT) 
    {
        return;
    }
    atomicassets::assets_t assets (atomicassets::ATOMICASSETS_ACCOUNT,CONTRACT_ACCOUNT.value);
    
    const name& collection_name = name(memo.substr(0,12));
    check_contract_auth(collection_name);
    check(check_number(memo.substr(13,memo.size())) == true, "Invalid blend id");
    uint64_t blend_id = std::stoll(memo.substr(13,memo.size()));

    blends_t blends_table(CONTRACT_ACCOUNT,collection_name.value);
    auto blends_table_itr = blends_table.require_find(blend_id, "Could not find blend id");
    check(blends_table_itr->available() == true, "the maximum craft limit of this element has been exhausted");
    check(blends_table_itr->requiring_components.size() == asset_ids.size(), "You have to transfer exact number of assets");

    std::vector<uint32_t> temp_vector = {};
    for(const uint64_t& asset_id : asset_ids)
    {
        auto assets_itr = assets.find(asset_id);
        check(assets_itr->collection_name == collection_name, "Asset id[" + std::to_string(asset_id) + "] is not from " + collection_name.to_string() + " collection");
        temp_vector.push_back(assets_itr->template_id);

        action
        (
            permission_level{CONTRACT_ACCOUNT,"active"_n},
            atomicassets::ATOMICASSETS_ACCOUNT,
            "burnasset"_n,
            std::make_tuple
            (
                CONTRACT_ACCOUNT,
                asset_id
            )
        ).send();
    }
    std::sort(temp_vector.begin(), temp_vector.end());
    check(std::equal(temp_vector.begin(), temp_vector.end(), blends_table_itr->requiring_components.begin()) == true, "Invalid components");

    const name& schema_name = ((atomicassets::get_templates(collection_name)).find(blends_table_itr->result))->schema_name;
    action
    (
        permission_level{CONTRACT_ACCOUNT,"active"_n},
        atomicassets::ATOMICASSETS_ACCOUNT,
        "mintasset"_n,
        std::make_tuple
        (
            CONTRACT_ACCOUNT,
            collection_name,
            schema_name,
            blends_table_itr->result,  
            from,
            (atomicassets::ATTRIBUTE_MAP){},
            (atomicassets::ATTRIBUTE_MAP){},
            (std::vector <asset>){}
        )
    ).send();

    blends_table.modify(blends_table_itr,CONTRACT_ACCOUNT,[&](auto &new_row)
    {
        ++new_row.crafted;
    });
}

const name blends::check_collection_auth(const name& collection_name)
{
    auto collection_itr = atomicassets::collections.require_find(collection_name.value, ("Collection" + collection_name.to_string() + " does not exist").c_str());
    check(has_auth(collection_itr->author), "You are is not authorized within the collection");
    return collection_itr->author;
}

void blends::check_contract_auth(const name& collection_name)
{
    auto collection_itr = atomicassets::collections.require_find(collection_name.value, ("Collection" + collection_name.to_string() + " does not exist").c_str());
    check(std::find(
        collection_itr->authorized_accounts.begin(),
        collection_itr->authorized_accounts.end(),
        CONTRACT_ACCOUNT
        ) != collection_itr->authorized_accounts.end(),
        CONTRACT_ACCOUNT.to_string() + " is not authorized within the collection");
}

void blends::check_repeated(std::vector<uint32_t>& array)
{
    std::sort(array.begin(), array.end());
    check(std::adjacent_find(array.begin(), array.end()) == array.end(), "Can't use the same asset multiple times");
}

const bool blends::check_number(const std::string& number)
{
    for(uint8_t i = 0; i < number.size(); ++i)
    {
        if(!isdigit(number[i]))
            return false;
    }
    return true;
}


