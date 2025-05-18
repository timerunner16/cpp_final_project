local input
local workspace

function init()
	input = Engine.InputManager
	workspace = Engine.Workspace
end

function process(delta)
	local q = input:QueryKey(Keys.Q)
	if (q.Pressed and q.OnEdge) then
		local particle_info = ParticleSystemCreateInfo.new()
		particle_info.R = math.random(0,255)
		particle_info.G = math.random(0,255)
		particle_info.B = math.random(0,255)
		workspace:CreateParticleSystem(particle_info)
	end
end
