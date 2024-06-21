--- File containing basic lua api's to mmh

---@private
---@class Homes
Homes = {}

local function home_is_uniq(name)
	for _, h in ipairs(Homes) do
		print(h.name)
		if h.name == name then
			return false
		end
	end
	return true
end

---@class Runner
---@field name string
---@field command string
---@field tag string

---@class Home
---@field name string name of homme
---@field data table|nil
---@field new fun(self, name: string|nil, data: table|nil): self
---@field display fun(self)
---@field system fun(command: string)
---@field create_runner fun(self, name: string, command: fun()|string, tag: string|nil) Create runner for your home with a name and command to be executed. Optionally, add a tag to group similar runners together
---@field execute_runner fun(self, name: string)
---@field execute_tag fun(self, tag: string)
local home = {}

function home:new(name, data)
	name = name or "home"
	if not home_is_uniq(name) then
		error("home " .. name .. " is not unique, consider changing it's name, or add to the existing home.")
	end
	data = data or {}
	setmetatable(data, self)
	self.__index = self
	table.insert(Homes, data)
	home.name = name
	return data
end

function home:create_runner(name, command, tag)
	if not self.runners then
		self.runners = {}
	end
	table.insert(self.runners, { command = command, name = name, tag = tag })
end

function home:execute_runner(name)
	for _, v in ipairs(self.runners) do
		if v.name == name then
			if type(v.command) == "function" then
				v.command()
			elseif type(v.command) == "string" then
				home.system(v.command)
			else
				error("unsupported type for execute_runner")
			end
			return
		end
	end
	error("could not find runner " .. name)
end

function home:execute_tag(tag)
	local count = 0
	for _, v in ipairs(self.runners) do
		if v.tag and v.tag == tag then
			if type(v.command) == "function" then
				v.command()
			elseif type(v.command) == "string" then
				home.system(v.command)
			end
			count = count + 1
		end
	end
	if count == 0 then
		error("did not find tag " .. tag)
	end
end

return home
