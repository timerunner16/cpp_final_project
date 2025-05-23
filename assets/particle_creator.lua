local workspace
local event

function init()
	workspace = Engine.Workspace
	event = nil
end

function process(delta)
	if (event) then return end

	local root = workspace:GetGameObject("root")
	if (not root) then return end

	local host = root:GetChild("ParticleHost")
	if (not host) then return end

	event = host:GetEvent("CreateParticles")
	if (not event) then return end

	event:Connect("create_particles")
end

function create_particles()
	local position = event:GetValue("position")
	position = position:times(2)

	local particle_info = ParticleSystemCreateInfo.new()
	particle_info.Position = position
	particle_info.Size = Vector2.new(0.1,0.1)
	particle_info.Randomization = Vector2.new(0.5,0.5)
	particle_info.Lifetime = 0.4
	particle_info.LaunchInterval = 0.2/8
	particle_info.NumLaunches = 8
	particle_info.NumParticles = 8
	particle_info.R = math.random(0,255)
	particle_info.G = math.random(0,255)
	particle_info.B = math.random(0,255)
	workspace:CreateParticleSystem(particle_info)
end
