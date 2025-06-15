-- object references
local sound

function process(delta)
	if (not sound) then
		sound = Engine.Workspace:CreateAudioInstance("BACKGROUND_MUSIC", "BACKGRND", nil)
		sound.Looping = true
		sound:Play()
	end
end
