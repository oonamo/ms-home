--- File containing basic lua api's to mmh

local mmh = {}

mmh.home_builder = {}

function mmh.test(lhs, rhs)
	return lhs == rhs
end

local home_builder = { font = "Home" }
function home_builder:new()
	setmetatable({}, self)
	self.__index = self
	return self
end

function home_builder:display()
	print("this is home builder")
end

mmh.home_builder = home_builder

return mmh
