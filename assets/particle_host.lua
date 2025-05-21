local timer = 0
local timer_delay = 0.1

local input
local workspace

local event

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
	event = workspace:CreateEvent("CreateParticles", Engine.CurrentGameObject)
end

function process(delta)
	timer = timer + delta
	local q = input:QueryKey(Keys.Q)
	if (q.Pressed and timer > timer_delay) then
		timer = 0
		event:Fire()
	end
end
