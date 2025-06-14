-- constants
local SPAWNOFF = 0.5

-- properties
local spells = {
	{
		name = "Damage",
		material = "ORB",
		mesh = "ORB",
		color = Vector3.new(255,120,20),
		damage = 60,
		range = 2,
		speed = 16,
		size = 0.2,
		force = 16.0,
		spread = 10,
		manacost = 20
	},
	{
		name = "Light Damage",
		material = "ORB",
		mesh = "ORB",
		color = Vector3.new(20,120,255),
		damage = 20,
		range = 1,
		speed = 16,
		size = 0.1,
		force = 8.0,
		spread = 3,
		manacost = 5
	},
	{
		name = "Wind Blast",
		material = "ORB",
		mesh = "ORB",
		color = Vector3.new(200,200,255),
		damage = 0,
		range = 3,
		speed = 10,
		size = 0.15,
		force = 60.0,
		spread = 6,
		manacost = 10
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
local pdata
local spawnpoints = {}

local function create_spell(down)
	local current_spell = spells[current_spell_i + 1]

	if (pdata:GetValue("mana") < current_spell.manacost) then
		return
	end

	pdata:SetValue("mana", pdata:GetValue("mana") - current_spell.manacost)

	local lookvec = -camera.Transform.LookVector

	local position
	local direction
	if (down) then
		direction = Vector3.new(0, -1, 0)
		position = camera.Transform.Position
	else
		local angle = math.atan(lookvec.z, lookvec.x)
		angle = -(angle + (math.random() * 2.0 - 1.0) * math.rad(current_spell.spread))
        direction = Vector3.new(math.cos(angle), 0, -math.sin(angle))
		position = camera.Transform.Position + lookvec * SPAWNOFF
	end

	local scale = Vector3.new(current_spell.size)
	local transform = Transform.new(position, Vector3.new(), scale)
	local spell = workspace:CreateGameObject(
		"SPELL_" .. current_spell.name .. "_" .. tostring(math.random(0,2147483647)), nil,
		"", resource_manager:GetMesh(current_spell.mesh), resource_manager:GetMaterial(current_spell.material),
		transform, Vector2.new(), 0
	)
	spell.Velocity = direction * current_spell.speed
	spell:SetUniform(Uniform.new("modulate", Vector4.new(current_spell.color.x/255.0, current_spell.color.y/255.0, current_spell.color.z/255.0, 1)))

	local active_spell = {
		color = current_spell.color,
		size = current_spell.size,
		speed = current_spell.speed,
		damage = current_spell.damage,
		force = current_spell.force,
		range = current_spell.range,
		object = spell,
		down = down
	}
	table.insert(active_spells, active_spell)
end

function init()
	workspace = Engine.Workspace
	window = Engine.Window
	input = Engine.InputManager
	resource_manager = Engine.ResourceManager
	pdata = Engine.PDataManager
	if (not pdata:GetValue("mana")) then
		pdata:SetValue("mana", 100)
	end
	camera = workspace:GetCamera()
end

function process(delta)
	if (#spawnpoints == 0) then
		for _,v in pairs(workspace:GetGameObjects()) do
			if (string.find(v:GetName(), "SP_")) then
				table.insert(spawnpoints, v)
			end
		end
	end

	player = workspace:GetGameObject("Observer")
	if (not player) then
		return
	end

	local switch = input:QueryKey(Keys.Q)
	if (switch.Pressed and switch.OnEdge) then
		current_spell_i = (current_spell_i + 1)%(#spells)
	end

	local fire = input:QueryKey(Keys.F)
	if (fire.Pressed and fire.OnEdge) then
		create_spell(false)
	end

	local fire_down = input:QueryKey(Keys.G)
	if (fire_down.Pressed and fire_down.OnEdge) then
		create_spell(true)
	end

	for i,v in pairs(active_spells) do
		local origin = v.object.Transform.Position
		local hit = false
		local main_hit_instance
		if (v.down and v.object.Transform.Position.y <= Engine.GetHighestOverlappingSector(v.object).HeightFloor) then
			local particle_info = ParticleSystemCreateInfo.new()
			particle_info.Position = origin * 2
			particle_info.Direction = Vector3.new(0, 1, 0)
			particle_info.NumLaunches = 1
			particle_info.NumParticles = 32
			particle_info.LaunchInterval = 1
			particle_info.Lifetime = 0.2
			particle_info.Speed = v.speed
			particle_info.Size = Vector2.new(v.size,v.size)
			particle_info.R = math.floor(v.color.x)
			particle_info.G = math.floor(v.color.y)
			particle_info.B = math.floor(v.color.z)
			particle_info.FadeOut = true
			workspace:CreateParticleSystem(particle_info)

			v.object:QueueFree()
			table.remove(active_spells, i)

			hit = true
		else
			local endpoint_v3 = origin + v.object.Velocity.unit
			local endpoint = Vector2.new(endpoint_v3.x, endpoint_v3.z)
			local filter = {v.object, player}
			for _,w in pairs(active_spells) do table.insert(filter, w.object) end
			for _,w in pairs(spawnpoints) do table.insert(filter, w) end
			local result = v.object:Raycast(origin, endpoint, filter)
			if (result.Hit) then
				v.object:QueueFree()
				table.remove(active_spells, i)

				local particle_info = ParticleSystemCreateInfo.new()
				particle_info.Position = origin * 2
				particle_info.Direction = Vector3.new(result.Normal.x, 0, result.Normal.y)
				particle_info.NumLaunches = 1
				particle_info.NumParticles = 32
				particle_info.LaunchInterval = 1
				particle_info.Lifetime = 0.2
				particle_info.Speed = v.speed
				particle_info.Size = Vector2.new(v.size,v.size)
				particle_info.R = math.floor(v.color.x)
				particle_info.G = math.floor(v.color.y)
				particle_info.B = math.floor(v.color.z)
				particle_info.FadeOut = true
				workspace:CreateParticleSystem(particle_info)

				hit = true

				if (result.Instance and string.find(result.Instance:GetName(), "E_")) then
					main_hit_instance = result.Instance
				end
			end
		end

		if (hit) then
			local targets = {}
			for _,w in pairs(workspace:GetGameObjects()) do
				if (main_hit_instance ~= w and string.find(w:GetName(), "E_") or string.find(w:GetName(), "Observer")) then
					if ((v.object.Transform.Position-w.Transform.Position).length < v.range) then
						table.insert(targets, w)
					end
				end
			end
			for _,w in pairs(targets) do
				local distance = (origin-w.Transform.Position:withY(origin.y)).length
				local impact = math.sqrt(v.range-distance)/math.sqrt(v.range)

				local event = w:GetEvent("TakeDamage")
				local direction = (origin - w.Transform.Position + Vector3.new(0,w.Height/2,0)).unit
				event:SetValue("damage", v.damage * impact)
				event:SetValue("position", origin)
				event:SetValue("direction", direction)
				event:SetValue("force", -direction * v.force * impact)
				event:Fire()
			end

			if (main_hit_instance) then
				local event = main_hit_instance:GetEvent("TakeDamage")
				local direction = (origin - main_hit_instance.Transform.Position + Vector3.new(0,main_hit_instance.Height/2,0)).unit
				event:SetValue("damage", v.damage)
				event:SetValue("position", origin)
				event:SetValue("direction", direction)
				event:SetValue("force", -direction * v.force)
				event:Fire()
			end
		end
	end

	local current_spell = spells[current_spell_i + 1]
	local spelltext = "Spell: " .. current_spell.name
	window:DrawString(math.floor(window.Width/window.Downscale)-1-string.len(spelltext)*8, 2,
					  80, 80, 0, 255,
					  spelltext)
	window:DrawString(math.floor(window.Width/window.Downscale)-2-string.len(spelltext)*8, 1,
					  255, 255, 0, 255,
					  spelltext)
	local costtext = "Cost: " .. tostring(current_spell.manacost)
	window:DrawString(math.floor(window.Width/window.Downscale)-1-string.len(costtext)*8, 10,
					  80, 80, 0, 255,
					  costtext)
	window:DrawString(math.floor(window.Width/window.Downscale)-2-string.len(costtext)*8, 9,
					  255, 255, 0, 255,
					  costtext)
	local manatext = "X: " .. pdata:GetValue("mana")
	window:DrawString(math.floor(window.Width/window.Downscale)-1-string.len(manatext)*8,
					  math.floor(window.Height/window.Downscale)-9,
					  80, 10, 80, 255,
					  manatext)
	window:DrawString(math.floor(window.Width/window.Downscale)-2-string.len(manatext)*8,
					  math.floor(window.Height/window.Downscale)-10,
					  255, 30, 255, 255,
					  manatext)
end
