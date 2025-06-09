-- constants
local SCRIPT = "assets/fleshwizard.lua"
local MESH = "FLESHWZD"
local MATERIAL = "FLESHWZD"
local BOUNDS = Vector2.new(0.5,0.5)
local HEIGHT = 2.3
local NAME = "E_FleshWizard"
local NUM_PER_BATCH = 4
local NUM_BATCHES = 1--8
local BATCH_DELAY = 4

-- properties
local num_created = 0
local timer = 0
local num_killed = 0
local spawnpoints = {}

-- object references
local workspace
local resource_manager
local window
local pdata

function killed()
	num_killed = num_killed + 1
	pdata:SetValue("mana", pdata:GetValue("mana") + 50)
	if (num_killed >= NUM_BATCHES * NUM_PER_BATCH) then
		local exit = workspace:GetGameObject("Exit")
		exit:GetEvent("OpenExit"):Fire()
	end
end

local function random_name()
	return NAME .. "_" .. tostring(math.random(0,2147483657))
end

local function create_enemy()
	num_created = num_created + 1
	local name = random_name()

	local spawnpoint = spawnpoints[math.random(#spawnpoints)]

	local new = workspace:CreateGameObject(
		name, nil,
		SCRIPT, resource_manager:GetMesh(MESH), resource_manager:GetMaterial(MATERIAL),
		spawnpoint, BOUNDS, HEIGHT
	)
	new:GetEvent("Destroyed"):Connect("killed")

	local particle_info = ParticleSystemCreateInfo.new()
	particle_info.Position = spawnpoint.Position * 2
	particle_info.Direction = Vector3.new(0,1,0)
	particle_info.Size = Vector2.new(0.2, 0.2)
	particle_info.Lifetime = 0.8
	particle_info.Gravity = -9.81
	particle_info.Randomization = math.pi/4
	particle_info.NumParticles = 32
	particle_info.NumLaunches = 3
	particle_info.LaunchInterval = 0.2
	particle_info.R = 200
	particle_info.G = 30
	particle_info.B = 30
	particle_info.FadeOut = true
	workspace:CreateParticleSystem(particle_info)
end

function init()
	workspace = Engine.Workspace
	resource_manager = Engine.ResourceManager
	window = Engine.Window
	pdata = Engine.PDataManager
end

function process(delta)
	timer = timer + delta
	if (#spawnpoints == 0) then
		for _,v in pairs(workspace:GetGameObjects()) do
			if (string.find(v:GetName(), "SP_")) then
				table.insert(spawnpoints, v.Transform)
			end
		end
	end

	if (num_created < NUM_BATCHES*NUM_PER_BATCH and timer > BATCH_DELAY) then
		for _=1,NUM_PER_BATCH do
			create_enemy()
		end
		timer = 0
	end

	if (num_killed < NUM_BATCHES*NUM_PER_BATCH) then
		window:DrawString(2,2, 80,80,80,255, tostring(num_killed) .. "/" .. tostring(NUM_BATCHES*NUM_PER_BATCH) .. " vanquished")
		window:DrawString(1,1, 255,255,255,255, tostring(num_killed) .. "/" .. tostring(NUM_BATCHES*NUM_PER_BATCH) .. " vanquished")
	else
		window:DrawString(2,2, 80, 10, 10, 255, "The exit is open!")
		window:DrawString(1,1, 255, 30, 30, 255, "The exit is open!")
	end
end
