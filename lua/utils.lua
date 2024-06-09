---@class Utils
---@field colors ColorTools
---@field runners RunnerTools
---@field get_appearance fun(): "light"|"dark" See platform specific implmentations
local utils = {}

---@class RunnerTools
---@field day_night_toggle fun(cb: fun(is_day))
utils.runners = {}

---@class ColorTools
utils.colors = {}

return utils
