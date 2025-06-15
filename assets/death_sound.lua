-- constants
local LIFETIME = 1.0
-- properties
local timer = 0.0
-- object references
local current

function init()
	current = Engine.CurrentGameObject
	local workspace = Engine.Workspace
	local audio_instance = workspace:CreateAudioInstance("DEATH_SOUND", "AAHH", current)
	audio_instance:Play()
end

function process(delta)
	timer = timer + delta

	if (timer > LIFETIME) then
		current:QueueFree()
	end
end
