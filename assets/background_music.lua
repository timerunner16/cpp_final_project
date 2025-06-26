-- object references
local sound
local pdata
local input

function init()
	pdata = Engine.PDataManager
	input = Engine.InputManager
end

function process(delta)
	if (not sound) then
		sound = Engine.Workspace:CreateAudioInstance("BACKGROUND_MUSIC", "BACKGRND", nil)
		sound.Looping = true
		if (not pdata:GetValue("bgm_muted")) then
			sound:Play()
		end
	end

	local switch = input:QueryKey(Keys.M)
	if (switch.Pressed and switch.OnEdge) then
		pdata:SetValue("bgm_muted", not pdata:GetValue("bgm_muted"))
		if (pdata:GetValue("bgm_muted")) then
			sound:Pause()
		else
			sound:Play()
		end
	end
end
