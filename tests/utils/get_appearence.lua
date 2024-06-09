-- local utils = require("utils")
print("hello, from get_appearance")
-- print(require("utils"))
local cur_appearence = "dark"

if utils.get_appearance() ~= cur_appearence then
	error("get_appearance did not return the correct value")
end
