local timer = 0
local timer_delay = 0.1

local input
local workspace

local event
local player
local camera

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
	camera = Engine.Workspace:GetCamera()
	event = workspace:CreateEvent("CreateParticles", Engine.CurrentGameObject)
end

function process(delta)
	if (not player) then
		player = workspace:GetGameObject("Observer")
		return
	end

	timer = timer + delta
	local q = input:QueryKey(Keys.Q)
	if (q.Pressed and timer > timer_delay) then
		timer = 0
		local position = camera.Transform.Position:plus(camera.Transform.LookVector:times(-1))
		local direction = camera.Transform.LookVector:times(-1)
		event:SetValue("position", position)
		event:SetValue("direction", direction)
		event:Fire()
	end
end
