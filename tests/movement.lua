require "lunit"

module("tests.movement", package.seeall, lunit.testcase)

function setup()
    eressea.free_game()
    eressea.settings.set("nmr.removenewbie", "0")
    eressea.settings.set("nmr.timeout", "0")
    eressea.settings.set("rules.ships.storms", "0")
    conf = [[{
        "races": {
            "human" : { "speed" : 1, "flags" : [ "walk" ] },
            "troll" : {}
        },
        "items" : {
            "horse" : {
                "capacity" : 7000,
                "weight" : 5000,
                "flags" : [ "big", "animal" ]
            }
        },
        "terrains" : {
            "ocean": { "flags" : [ "sea", "sail" ] },
            "plain": { "flags" : [ "land", "walk", "sail" ] },
            "glacier": { "flags" : [ "land", "walk" ] }
        },
        "directions" : {
            "de" : {
                "east" : "OSTEN",
                "west" : "WESTEN"
            }
        },
        "keywords" : {
            "de" : {
                "move" : "NACH"
            }
        }
    }]]

    eressea.config.reset()
    eressea.config.parse(conf)
end

function test_walk_to_land()
    local r1 = region.create(0, 0, "plain")
    local r2 = region.create(1, 0, "plain")
    local f = faction.create("test@example.com", "human", "de")
    local u = unit.create(f, r1, 1)
    u:add_order("NACH O")
    process_orders()
    assert_equal(r2, u.region)
end

function test_walk_to_ocean()
    local r1 = region.create(0, 0, "plain")
    local r2 = region.create(1, 0, "ocean")
    local f = faction.create("test@example.com", "human", "de")
    local u = unit.create(f, r1, 1)
    u:add_order("NACH O")
    process_orders()
    assert_equal(r1, u.region)
end

function test_walk_distance()
    local r1 = region.create(0, 0, "plain")
    local r2 = region.create(1, 0, "plain")
    local r3 = region.create(2, 0, "plain")
    local f = faction.create("test@example.com", "human", "de")
    local u = unit.create(f, r1, 1)
    u:add_order("NACH O O")
    process_orders()
    assert_equal(r2, u.region)
end

function test_ride_distance()
    local r1 = region.create(0, 0, "plain")
    local r2 = region.create(1, 0, "plain")
    local r3 = region.create(2, 0, "plain")
    local f = faction.create("test@example.com", "human", "de")
    local u = unit.create(f, r1, 1)
    u:add_item("horse", 1)
    u:set_skill("riding", 2)
    u:add_order("NACH O O")
    process_orders()
    assert_equal(r3, u.region)
end
