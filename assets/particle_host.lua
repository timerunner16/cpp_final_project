local timer = 0
local timer_delay = 0.1

local input
local workspace

local event
local player

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
	event = workspace:CreateEvent("CreateParticles", Engine.CurrentGameObject)
end

function process(delta)
	if (not player) then
		local root = workspace:GetGameObject("root")
		player = root:GetChild("Observer")
		return
	end

	timer = timer + delta
	local q = input:QueryKey(Keys.Q)
	if (q.Pressed and timer > timer_delay) then
		timer = 0
		local position = player.Transform.Position
		event:SetValue("position", position)
		event:Fire()
	end
end
