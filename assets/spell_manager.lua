-- constants
local SPAWNOFF = 0.5

-- properties
local spells = {
	{
		name = "Damage",
		material = "ORB",
		mesh = "ORB",
		color = Vector3.new(255,0,0),
		damage = 60,
		speed = 16,
		size = 0.1,
		spread = math.pi/4
	}
}
local current_spell_i = 0

local active_spells = {}

-- object references
local player
local camera
local workspace
local window
local input
local resource_manager

local function create_spell()
	local current_spell = spells[current_spell_i + 1]

	local lookvec = -camera.Transform.LookVector

	local angle = math.atan(lookvec.z, lookvec.x)
	angle = angle + (math.random() * 2.0 - 1.0) * current_spell.spread
	local direction = Vector3.new(math.cos(angle), 0, math.sin(angle))

	local position = camera.Transform.Position + lookvec * SPAWNOFF
	local scale = Vector3.new(current_spell.size)
	local transform = Transform.new(position, Vector3.new(), scale)
	local spell = workspace:CreateGameObject(
		"_SPELL_" .. current_spell.name .. "_" .. tostring(math.random(0,2147483647)), nil,
		"", resource_manager:GetMesh(current_spell.mesh), resource_manager:GetMaterial(current_spell.material),
		transform, Vector2.new(), 0
	)
	spell.Velocity = direction:withY(0) + lookvec:withY(0).unit * current_spell.speed

	table.insert(active_spells, spell)
end

function init()
	workspace = Engine.Workspace
	window = Engine.Window
	input = Engine.InputManager
	resource_manager = Engine.ResourceManager
	camera = workspace:GetCamera()
end

function process(delta)
	if (not player) then
		player = workspace:GetGameObject("Observer")
		return
	end

	local fire = input:QueryKey(Keys.F)
	if (fire.Pressed and fire.OnEdge) then
		current_spell_i = (current_spell_i + 1)%(#spells)
		create_spell()
	end

	for i,v in pairs(active_spells) do
		local origin = v.Transform.Position
		local endpoint_v3 = origin + v.Velocity.unit
		local endpoint = Vector2.new(endpoint_v3.x, endpoint_v3.z)
		local filter = {v, player}
		for _,w in pairs(active_spells) do table.insert(filter, w) end
		local result = v:Raycast(origin, endpoint, filter)
		if (result.Hit) then
			print("Hit!", result.Position * 2)
			v:QueueFree()
			table.remove(active_spells, i)

			local particle_info = ParticleSystemCreateInfo.new()
			particle_info.Position = origin * 2
			particle_info.Direction = Vector3.new(result.Normal.x, 0, result.Normal.y)
			particle_info.NumLaunches = 1
			particle_info.NumParticles = 32
			particle_info.LaunchInterval = 1
			particle_info.Lifetime = 0.2
			particle_info.Size = Vector2.new(0.1,0.1)
			workspace:CreateParticleSystem(particle_info)
		end
	end

	--local current_spell = spells[current_spell_i + 1]
	--window:DrawString(0, 0,  255, 255, 0,   255,  "Spell: " .. current_spell.name);
end
