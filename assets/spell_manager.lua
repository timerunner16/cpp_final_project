-- constants
local SPAWNOFF = 1.5

-- properties
local spells = {
	{
		name = "Damage",
		material = "ORB",
		mesh = "ORB",
		color = Vector3.new(255,0,0),
		damage = 60,
		speed = 16,
		size = 0.5
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
	local position = camera.Transform.Position + lookvec * SPAWNOFF
	local direction = camera.Transform.LookVector
	local scale = Vector3.new(current_spell.size)
	local transform = Transform.new(position, direction, scale)
	local spell = workspace:CreateGameObject(
		"_SPELL_" .. current_spell.name .. "_" .. tostring(math.random(0,2147483647)), nil,
		"", resource_manager:GetMesh(current_spell.mesh), resource_manager:GetMaterial(current_spell.material),
		transform, Vector2.new(0.5,0.5)*current_spell.size, current_spell.size
	)
	spell.Velocity = Vector3.new(lookvec.x, 0, lookvec.z).unit * current_spell.speed
	
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

	for _,v in pairs(active_spells) do
		local origin = v.Transform.Position
		local endpoint_v3 = origin + v.Velocity.unit
		local endpoint = Vector2.new(endpoint_v3.x, endpoint_v3.z)
		local result = v:Raycast(origin, endpoint, {v, player})
		if (result.Hit) then
			print("Hit!", result.Position * 2)
		end
	end

	local current_spell = spells[current_spell_i + 1]
	window:DrawString(0, 0,  255, 255, 0,   255,  "Spell: " .. current_spell.name);
end
