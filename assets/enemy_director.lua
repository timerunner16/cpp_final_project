-- constants
local SCRIPT = "assets/fleshwizard.lua"
local MESH = "FLESHWZD"
local MATERIAL = "FLESHWZD"
local BOUNDS = Vector2.new(0.5,0.5)
local HEIGHT = 2.3
local NAME = "E_FleshWizard"
local BATCH_DELAY = 4
local DEAD_TEXT = "You Died!"

-- properties
local num_per_batch = math.random(2,5)
local num_batches = math.random(3,7)
local num_created = 0
local timer = 0
local num_killed = 0
local spawnpoints = {}
local time_since_player_alive = 0

-- object references
local workspace
local pdata
local resource_manager
local window

function killed()
	num_killed = num_killed + 1
	pdata:SetValue("mana", pdata:GetValue("mana") + 50)
	if (num_killed >= num_batches*num_per_batch) then
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
	local audio_instance = spawnpoint:GetAudioInstance("SPAWN_SOUND")
	audio_instance.TrackPosition = 0
	audio_instance:Play()

	local new = workspace:CreateGameObject(
		name, nil,
		SCRIPT, resource_manager:GetMesh(MESH), resource_manager:GetMaterial(MATERIAL),
		spawnpoint.Transform, BOUNDS, HEIGHT
	)
	new:GetEvent("Destroyed"):Connect("killed")

	local particle_info = ParticleSystemCreateInfo.new()
	particle_info.Position = spawnpoint.Transform.Position * 2
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
	if (not workspace:GetGameObject("Observer")) then
		time_since_player_alive = time_since_player_alive + delta

		local d_width = string.len(DEAD_TEXT)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-d_width*4+1,
			math.floor(window.Height/window.Downscale/2)-3,
			80, 80, 80, 255, DEAD_TEXT
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-d_width*4,
			math.floor(window.Height/window.Downscale/2)-4,
			255, 255, 255, 255, DEAD_TEXT
		)

		local score_text = "SCORE: " .. tostring(pdata:GetValue("score"))
		local s_width = string.len(score_text)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-s_width*4+1,
			math.floor(window.Height/window.Downscale/2)+5,
			80, 80, 80, 255, score_text
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-s_width*4,
			math.floor(window.Height/window.Downscale/2)+4,
			255, 255, 255, 255, score_text
		)

		if (time_since_player_alive > 5.0) then
			Engine.ChangeMap("MENU")
		end
		return
	end

	if (#spawnpoints == 0) then
		for _,v in pairs(workspace:GetGameObjects()) do
			if (string.find(v:GetName(), "SP_")) then
				table.insert(spawnpoints, v)
				workspace:CreateAudioInstance("SPAWN_SOUND", "SPAWN", v)
			end
		end
	end

	if (num_created < num_batches*num_per_batch and timer > BATCH_DELAY) then
		for _=1,num_per_batch do
			create_enemy()
		end
		timer = 0
	end

	if (num_killed < num_batches*num_per_batch) then
		window:DrawString(2,2, 80,80,80,255, tostring(num_killed) .. "/" .. tostring(num_batches*num_per_batch) .. " vanquished")
		window:DrawString(1,1, 255,255,255,255, tostring(num_killed) .. "/" .. tostring(num_batches*num_per_batch) .. " vanquished")
	else
		window:DrawString(2,2, 80, 10, 10, 255, "The exit is open!")
		window:DrawString(1,1, 255, 30, 30, 255, "The exit is open!")
	end
end
