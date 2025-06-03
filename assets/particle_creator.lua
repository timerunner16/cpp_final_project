local workspace
local pdata

local event

function init()
	workspace = Engine.Workspace
	pdata = Engine.PDataManager
	event = nil
end

function process(delta)
	if (event) then return end

	local host = workspace:GetGameObject("ParticleHost")
	if (not host) then return end

	event = host:GetEvent("CreateParticles")
	if (not event) then return end

	event:Connect("create_particles")
end

function create_particles()
	local position = event:GetValue("position")
	local direction = event:GetValue("direction")
	local color = pdata:GetValue("particle_color")
	position = position * 2 -- WHY DO I NEED TO DO THIS?!?!

	local particle_info = ParticleSystemCreateInfo.new()
	particle_info.Position = position
	particle_info.Direction = direction
	particle_info.Size = Vector2.new(0.1,0.1)
	particle_info.Lifetime = 0.4
	particle_info.LaunchInterval = 0.2/8
	particle_info.NumLaunches = 8
	particle_info.NumParticles = 8
	particle_info.R = math.floor(color.x)
	particle_info.G = math.floor(color.y)
	particle_info.B = math.floor(color.z)
	workspace:CreateParticleSystem(particle_info)
end
