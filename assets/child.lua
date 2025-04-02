local timer = 0.0
local offset = Vector3.new(math.random(),math.random(),math.random())
local prefix = "child.lua: "

function init()
	print("hi my name is " .. Engine.CurrentGameObject:GetName())
end

function process(delta)
	local self = Engine.CurrentGameObject
	self.transform.rotation = Vector3.new(timer):add(offset)

	timer = timer + delta
end
