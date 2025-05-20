local timer = 0
local timer_delay = 0.1

local input
local workspace

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
end

function process(delta)
	timer = timer + delta
	local q = input:QueryKey(Keys.Q)
	if (q.Pressed and timer > timer_delay) then
		timer = 0
		local particle_info = ParticleSystemCreateInfo.new()
		particle_info.Size = Vector2.new(0.1,0.1)
		particle_info.Randomization = Vector2.new(0.5,0.5)
		particle_info.R = math.random(0,255)
		particle_info.G = math.random(0,255)
		particle_info.B = math.random(0,255)
		workspace:CreateParticleSystem(particle_info)
	end
end
