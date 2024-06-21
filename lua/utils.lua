---@class Utils
---@field colors ColorTools
---@field runners RunnerTools
---@field get_appearance fun(): "light"|"dark" See platform specific implmentations
---@field inspect fun(val, indent)
local utils = {}

function utils.inspect(val, indent)
  if not indent then
    indent = 0
  end
  for k, v in pairs(val) do
    local formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      utils.inspect(v, indent + 1)
    elseif type(v) == "boolean" then
      print(formatting .. tostring(v))
    else
      print(formatting .. v)
    end
  end
end

---@class RunnerTools
---@field day_night_toggle fun(cb: fun(is_day))
utils.runners = {}

function utils.runners.day_night_toggle(cb)
  local cur_appearence = utils.get_appearance()
end

---@class ColorTools
utils.colors = {}

return utils
