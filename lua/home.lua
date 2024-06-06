--- File containing basic lua api's to mmh

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

return home
