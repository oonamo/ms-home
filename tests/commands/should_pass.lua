-- runner should be created, then executed
-- command line flag -e should point to this file
home:new()
home:create_runner("t1", function() end, "test_runner")
home:execute_runner("t1")
