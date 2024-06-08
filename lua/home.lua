--- File containing basic lua api's to mmh

---@class Runner
---@field name string
---@field command string
---@field args string[]

---@class Home
---@field new fun(self, data: table): self
---@field display fun(self)
---@field runners Runner[]
---@field run fun(command: string, args: string[])
local home = {}

function home:new(data)
	data = data or {}
	setmetatable(data, self)
	self.__index = self
	return self
end

function home:display()
	print("from home display")
end

function home.test(lhs, rhs)
	return lhs == rhs
end

function home:create_runner(name, command, args)
	if not self.runners then
		self.runners = {}
	end
	table.insert(self.runners, { command = command, args = args, name = name })
end

function home:execute_runner(name)
	for _, v in ipairs(self.runners) do
		if v.name == name then
			print("found runner " .. name)
			home.run(v.command, v.args)
			return
		end
	end
	error("could not find runner " .. name)
end

return home
