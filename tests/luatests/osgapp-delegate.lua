print("Loading lua file")

testapp = {

	initScene = function (self)
		print("In initScene")
		print("Stopping kernel")
		vrjKernel.stop()
	end,

	preFrame = function (self)
		print("In preFrame")
	end
}


print("App delegate defined, now creating OsgAppProxy")
app = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
app:setAppDelegate(testapp)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

print("Setting kernel application")
app:setActiveApplication()

print("Starting kernel")
vrjKernel.start()