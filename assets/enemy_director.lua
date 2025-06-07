-- constants
local SCRIPT = "assets/fleshwizard.lua"
local MESH = "FLESHWZD"
local MATERIAL = "FLESHWZD"
local BOUNDS = Vector2.new(0.5,0.5)
local HEIGHT = 2.3
local NAME = "E_FleshWizard"
local NUM_PER_BATCH = 4
local NUM_BATCHES = 8
local BATCH_DELAY = 4

-- properties
local num_created = 0
local timer = 0
local num_killed = 0

-- object references
local workspace
local resource_manager
local window
local pdata
local spawnpoints = {}

function killed()
	num_killed = num_killed + 1
	pdata:SetValue("mana", pdata:GetValue("mana") + 50)
end

local function random_name()
	return NAME .. "_" .. tostring(math.random(0,2147483657))
end

local function create_enemy()
	num_created = num_created + 1
	local name = random_name()

	local new = workspace:CreateGameObject(
		name, nil,
		SCRIPT, resource_manager:GetMesh(MESH), resource_manager:GetMaterial(MATERIAL),
		spawnpoints[math.random(#spawnpoints)], BOUNDS, HEIGHT
	)
	new:GetEvent("Destroyed"):Connect("killed")
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
