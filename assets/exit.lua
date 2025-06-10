-- constants
local AVAILABLE_MAPS = {
	"MAP01",
	"MAP02"
}
local PROXIMITY = 1.5

-- properties
local open
local timer = 0

-- object references
local workspace
local input
local window
local current
local event

function openexit()
	open = true
	current.Visible = true
end

function init()
	workspace = Engine.Workspace
	input = Engine.InputManager
	window = Engine.Window
	current = Engine.CurrentGameObject
	event = workspace:CreateEvent("OpenExit", current)
	event:Connect("openexit")
	current.Visible = false
end

function process(delta)
	if (not open) then return end

	timer = timer + delta

	if (timer > 0.1) then
		timer = 0
		local particle_info = ParticleSystemCreateInfo.new()
		particle_info.Position = current.Transform.Position * 2
		particle_info.Direction = Vector3.new(0,1,0)
		particle_info.NumLaunches = 1
		particle_info.NumParticles = 32
		particle_info.Size = Vector2.new(0.05,0.05)
		particle_info.R = 200
		particle_info.G = 80
		particle_info.B = 255
		particle_info.FadeOut = true
		particle_info.Gravity = 10
		particle_info.Speed = 6
		particle_info.Lifetime = .5
		workspace:CreateParticleSystem(particle_info)
		particle_info.Position = (current.Transform.Position + Vector3.new(0,2.5,0)) * 2
		particle_info.Direction = Vector3.new(0,-1,0)
		particle_info.Gravity = -10
		workspace:CreateParticleSystem(particle_info)
	end

	local player = workspace:GetGameObject("Observer")
	if (not player) then return end

	local distance = (player.Transform.Position - current.Transform.Position).length
	if (distance < PROXIMITY) then
		local text = "Press E to exit."
		window:DrawString(
			math.floor(window.Width/window.Downscale/2) - string.len(text)*4 + 1,
			math.floor(window.Height/window.Downscale/2) - 4 + 1,
			80, 80, 80, 255,
			text
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2) - string.len(text)*4,
			math.floor(window.Height/window.Downscale/2) - 4,
			255, 255, 255, 255,
			text
		)

		if (input:QueryKey(Keys.E).Pressed) then
			Engine.ChangeMap(AVAILABLE_MAPS[math.random(#AVAILABLE_MAPS)])
		end
	end
end
