# nest-blending

##### To start using blend contract you have to provide auth for our contract to your collection. Than you have to call:

###### "setblend" action with parameters:
- collection_name - name of your collection
- requiring_components - array of templates(components) for creating new one(result)  
- result - template id of resulting NFT(which will be minted after blending)
- max_supply - maximum value of minting results (0 - infinity)
###### In table "blends" you can find your created blends(scope: collection_name)

##### If you want to blend your NFTs you have to transfer requiring components to our contract with memo : "collection_name:blend_id".
##### A little magic and you can see the result in your inventory. Good luck)

