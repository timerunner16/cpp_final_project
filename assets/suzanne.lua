local timer = 0.0

function init()
	print("hi my name is " .. Engine.CurrentGameObject:GetName())
end

function process(delta)
	timer = timer + delta
end
