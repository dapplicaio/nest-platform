
void blends::setblend(const name& collection_name, std::vector<uint32_t> requiring_components, const uint32_t& result, const uint32_t& max_supply)
{
    const name& author = check_collection_auth(collection_name);
    check_contract_auth(collection_name);
    check_repeated(requiring_components);
    check(requiring_components.size() >= 2, "Field \"requiring_components\" can not be empty and requiring at least 2 components");

    blends_t blends_table = get_blends(collection_name);
    auto templates_table = atomicassets::get_templates(collection_name);
    templates_table.require_find(result, ("Template id[" + std::to_string(result) + "] does not exist in " + collection_name.to_string()).c_str());

    for(const uint32_t& requiring_component : requiring_components)
    {
        check(result != requiring_component, "The resulting NFT cannot be among the components");
        templates_table.require_find(requiring_component, ("Template id[" + std::to_string(requiring_component) + "] does not exist in " + collection_name.to_string()).c_str());
    }
    
    uint64_t blend_id = blends_table.available_primary_key();
    if (blend_id == 0) 
    {
        blend_id = 1;
    }

    std::sort(requiring_components.begin(),requiring_components.end());

    blends_table.emplace(author,[&](auto &new_row)
    {
        new_row.blend_id = blend_id;
        new_row.requiring_components = requiring_components;
        new_row.result = result;
        new_row.max_supply = max_supply;
        new_row.crafted = 0;
    });
}

void blends::deleteblend(const name& collection_name, uint64_t blend_id)
{
    check_collection_auth(collection_name);

    blends_t blends_table = get_blends(collection_name);
    blends_table.erase(blends_table.require_find(blend_id, "Could not find blend id"));
}






