--- File containing basic lua api's to mmh

---@class Runner
---@field name string
---@field command string
---@field tag string

---@class Home
---@field new fun(self, data: table): self
---@field display fun(self)
---@field runners Runner[]
---@field system fun(command: string)
---@field data table|nil
---@field create_runner fun(self, name: string, command: fun()|string, tag: string|nil)
---@field execute_runner fun(self, name: string)
local home = {}

function home:new(data)
	data = data or {}
	setmetatable(data, self)
	self.__index = self
	return self
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
