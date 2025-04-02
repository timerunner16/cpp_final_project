local timer = 0.0

function init()
	print("hi my name is " .. Engine.CurrentGameObject:GetName())
end

function process(delta)
	timer = timer + delta
	local self = Engine.CurrentGameObject;
	self.transform.position.y = math.sin(timer)
	self.transform.rotation.z = timer
end
