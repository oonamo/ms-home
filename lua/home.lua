--- File containing basic lua api's to mmh

---@class Runner
---@field name string
---@field command string

---@class Home
---@field new fun(self, data: table): self
---@field display fun(self)
---@field runners Runner[]
---@field system fun(command: string)
---@field data table|nil
---@field execute_runner fun(self, name: string)
local home = {}

function home:new(data)
	data = data or {}
	setmetatable(data, self)
	self.__index = self
	return self
end

function home:create_runner(name, command)
	if not self.runners then
		self.runners = {}
	end
	table.insert(self.runners, { command = command, name = name })
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

return home
