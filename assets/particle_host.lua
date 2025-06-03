local timer = 0
local timer_delay = 0.1

local input
local workspace
local pdata

local event
local player
local camera

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
	pdata = Engine.PDataManager
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
		local origin_v3 = camera.Transform.Position;
		local direction_v3 = -camera.Transform.LookVector;
		local origin = Vector2.new(origin_v3.x, origin_v3.z);
		local direction = Vector2.new(direction_v3.x, direction_v3.z).unit;
		local result = player:Raycast(origin, origin + direction * 32)
		if (result.Hit) then
			local position_v2 = result.Position
			local direction_v2 = result.Normal
			local position = Vector3.new(position_v2.x, camera.Transform.Position.y, position_v2.y)
			local direction = Vector3.new(direction_v2.x, 0, direction_v2.y);
			pdata:SetValue("particle_color", Vector3.new(math.random(128,255),math.random(128,255),math.random(0,30)))
			event:SetValue("position", position)
			event:SetValue("direction", direction)
			event:Fire()
		end
	end
end
