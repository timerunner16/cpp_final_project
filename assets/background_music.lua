-- constants
local SOUND_NAME = "A_Background_Music"
local MUSIC_NAME = "BACKGRND"

function init()
	local workspace = Engine.Workspace
	workspace:CreateAudioInstance(SOUND_NAME, MUSIC_NAME, nil)
end
