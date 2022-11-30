local npc = event.me
player = event.activator -- we need this global for npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- group (category) definitions, currently use hardcoded type and subtype to id content
-- TODO remove hardcoded definitions
groups =
{
  {name = "1 handed pierce weapons", type = 15, subtype = 2},
  {name = "2 handed pierce weapons", type = 15, subtype = 6},
  {name = "polearm pierce weapons", type = 15, subtype = 10},
  {name = "1 handed slash weapons", type = 15, subtype = 1},
  {name = "2 handed slash weapons", type = 15, subtype = 5},
  {name = "polearm slash weapons", type = 15, subtype = 9},
  {name = "1 handed cleave weapons", type = 15, subtype = 3},
  {name = "2 handed cleave weapons", type = 15, subtype = 7},
  {name = "polearm cleave weapons", type = 15, subtype = 11},
  {name = "1 handed impact weapons", type = 15, subtype = 0},
  {name = "2 handed impact weapons", type = 15, subtype = 4},
  {name = "polearm impact weapons", type = 15, subtype = 8}
}

-- these must be the correct arch names. we need this global ...
for_sale =
{
  -- 1 handed pierce
  {arch = "dagger_small"},
  {arch = "dagger"},
  {arch = "dagger_large"},
  {arch = "athame"},
  {arch = "tanto"},
  {arch = "cutlass"},
  {arch = "sai"},
  {arch = "saber"},
  {arch = "pallasch"},
  {arch = "degen"},
  {arch = "rapier"},
  {arch = "shiavona"},
  {arch = "flamberge"},
  {arch = "spatha"},
  {arch = "saber_willow"},
  -- 2 handed pierce
  {arch = "xuqiang"},
  {arch = "spear"},
  {arch = "forc"},
  {arch = "forc_military"},
  {arch = "dory"},
  {arch = "trident"},
  -- polearm pierce
  {arch = "pike"},
  {arch = "guisarme"},
  {arch = "bill_guisarme"},
  {arch = "ranseur"},
  {arch = "partisan"},	
  -- 1 handed slash
  {arch = "tulwar"},
  {arch = "baselard"},
  {arch = "shortsword"},
  {arch = "ninjato"},
  {arch = "wakizashi"},
  {arch = "gladius"},
  {arch = "sword"},
  {arch = "sword_curved"},
  {arch = "kopis"},
  {arch = "bastardsword"},
  {arch = "tsurugi"},
  {arch = "katana"},
  {arch = "longsword"},
  {arch = "scimitar"},
  {arch = "scythe_military"},
  {arch = "falchion"},
  {arch = "muleijian"},
  {arch = "zuijian"},
  {arch = "broadsword"},
  -- 2 handed slash
  {arch = "claymore"},
  {arch = "scythe_war"},
  {arch = "sword2h"},
  {arch = "sword_double"},
  {arch = "sword_great"},
  {arch = "no_dachi"},
  {arch = "fujian"},
  {arch = "panzerstecher"},
  -- polearm slash
  {arch = "fauchard"},
  {arch = "glaive"},
  {arch = "zuidao"},
  {arch = "zuipudao"},
  {arch = "naginata"},
  -- 1 handed cleave
  {arch = "whip"},
  {arch = "axe_small"},
  {arch = "pick_axe"},
  {arch = "cleaver_war"},
  {arch = "axe_hand"},
  {arch = "axe_war"},
  {arch = "axe"},
  {arch = "whip_war"},
  {arch = "axe_double"},
  {arch = "axe_battle"},
  {arch = "axe_broad"},
  {arch = "axe_great"},
  {arch = "cleaver_battle"},
  -- 2 handed cleave
  {arch = "axe_2h"},
  {arch = "doubleji"},
  {arch = "axe_2h_battle"},
  {arch = "axe_2h_great"},
  {arch = "mattock"},
  {arch = "axe_2h_war"},
  -- polearm cleave
  {arch = "taiji"},
  {arch = "bardiche"},
  {arch = "spetum"},
  {arch = "halberd"},
  {arch = "fuji"},
  {arch = "voulge"},
  -- 1 handed impact
  {arch = "hammer_small"},
  {arch = "nunchacu"},
  {arch = "club"},
  {arch = "mstar_small"},
  {arch = "nunchacu_war"},
  {arch = "mace"},
  {arch = "club_rough"},
  {arch = "hammer_military"},
  {arch = "hammer"},
  {arch = "club_big"},
  {arch = "mace_spiked"},
  {arch = "manrikigusari"},
  {arch = "mstar"},
  {arch = "club_spiked"},
  {arch = "hammer_war"},
  {arch = "mace_battle"},
  {arch = "mstar_large"},
  {arch = "shootingstar"},
  {arch = "mace_war"},
  {arch = "mace_spiked_war"},
  {arch = "flail_war"},
  {arch = "ancus"},
  {arch = "hammer_battle"},
  -- 2 handed impact
  {arch = "mstar_2h"},
  {arch = "club_giant"},
  {arch = "club_giant_spiked"},
  {arch = "bo"},
  {arch = "hammer_giant"},
  {arch = "hammer_swar_giant"},
  {arch = "hammer_war_giant"},
  {arch = "quaterstaff"},
  -- polearm impact
  {arch = "bec_de_corbin"},
  {arch = "lucern_hammer"}
}

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("Welcome to my small shop.")
  ib:AddMsg("You want to see my stuff?")
  ib:AddLink("Sure.","shop")
  ib:AddLink("No, not now.","")
  ib:SetLHSButton("Shop")
end

require("/scripts/npc_shop.lua")

require("topic_list")
tl = TopicList()
tl:SetDefault(topic_greeting)
tl = shopAddTopics(tl)
tl:AddServices(nil, topic_shop)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
