local test = "echo done"

home:new()
home:create_runner("runner", test)
home:create_runner("Create Something", "echo something")
home:execute_runner("runner")

home:create_runner("functional", function()
	print("functional")
end)
