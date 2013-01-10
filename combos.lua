-- TODO write an editor for this shit
-- Hell this will branch out
combos = {
    {
        ["text"] = "left right kick combo",
        ["steps"] = {
            {
                ["minTime"] = 0, -- reset progress tree if action occures before time passed
                ["maxTime"] = 2000, -- after this time the progress tree is reset
                ["actions"] = {"kickLeft"}, -- hotkey names
                ["hitboxes"] = {
                    {
                        ["name"] = "footLeft",
                        ["x"] = 0,
                        ["y"] = 0,
                        ["w"] = 50,
                        ["h"] = 50,
                        ["forceX"] = 45,
                        ["forceY"] = 15
                    },
                    {
                        ["name"] = "footRight",
                        ["x"] = 0,
                        ["y"] = 0,
                        ["w"] = 50,
                        ["h"] = 50,
                        ["forceX"] = -5,
                        ["forceY"] = 0
                    }
                },
            },
            {
                --"kickRight", "kickLeft"
            }
        },
        ["followUps"] = {
            "text" = "left right punch combo",
            ["steps"] = {
                
            }
        }
    }
}
