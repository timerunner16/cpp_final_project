local timer = 0.0
local prefix = "observer.lua: "

function init()
	print("hi my name is " .. Engine.CurrentGameObject:GetName())
end

function process(delta)
	local camera = Engine.Workspace:GetCamera()
	
	local average_position = Vector3.new()
	local counted = 0
	for i,v in pairs(Engine.Workspace:GetGameObject("root"):GetChild("suzanne"):GetChildren()) do
		average_position = average_position:add(v.global_transform.position)
		counted = counted + 1
	end
	average_position = average_position:divf(counted)

	camera.transform.rotation.x = -math.atan((average_position.y-camera.transform.position.y)/(average_position.z-camera.transform.position.z))
	camera.transform.rotation.y = math.atan((average_position.x-camera.transform.position.x)/(average_position.z-camera.transform.position.z))

	timer = timer + delta
end
