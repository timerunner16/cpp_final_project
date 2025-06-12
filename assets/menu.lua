-- constants
local START_INPUT_TEXT = "Press [SPACE] to Begin."
local CREDIT_INPUT_TEXT = "Press [F] to see Credits."
local TITLE_INPUT_TEXT = "Press [F] to see Title."
local HIGHSCORE_TEXT = "High Score: "
-- properties
local yaw = 0.0
local timer = 0.0
local credits = false
-- object references
local input
local window
local camera
local resource_manager
local pdata

local function lerp(a, b, t)
	return a + (b - a) * t
end

function init()
	input = Engine.InputManager
	window = Engine.Window
	camera = Engine.Workspace:GetCamera()
	resource_manager = Engine.ResourceManager
	resource_manager:GetMaterial("PPDITHER"):SetUniform(Uniform.new("modulate", Vector4.new(1.0)))
	pdata = Engine.PDataManager
	if (pdata:GetValue("score")) then
		if (pdata:GetValue("highscore") and pdata:GetValue("highscore") < pdata:GetValue("score")) then
			pdata:SetValue("highscore", pdata:GetValue("score"))
		end
	end
	if (not pdata:GetValue("highscore")) then
		pdata:SetValue("highscore", 0)
	end
	pdata:SetValue("score", 0)
end

function process(delta)
	timer = timer + delta
	if (not credits) then
		local s_width = string.len(START_INPUT_TEXT)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-s_width*4+1,
			math.floor(7*window.Height/window.Downscale/8)-3,
			80, 80, 80, 255, START_INPUT_TEXT
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-s_width*4,
			math.floor(7*window.Height/window.Downscale/8)-4,
			255, 255, 255, 255, START_INPUT_TEXT
		)

		local c_width = string.len(CREDIT_INPUT_TEXT)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-c_width*4+1,
			math.floor(7*window.Height/window.Downscale/8)+5,
			80, 80, 80, 255, CREDIT_INPUT_TEXT
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-c_width*4,
			math.floor(7*window.Height/window.Downscale/8)+4,
			255, 255, 255, 255, CREDIT_INPUT_TEXT
		)

		local hs_text_formatted = HIGHSCORE_TEXT .. tostring(pdata:GetValue("highscore"))
		local h_width = string.len(hs_text_formatted)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-h_width*4+1,
			math.floor(window.Height/window.Downscale/8)+5,
			80, 10, 30, 255, hs_text_formatted
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-h_width*4,
			math.floor(window.Height/window.Downscale/8)+4,
			255, 30, 255, 255, hs_text_formatted
		)

		if (input:QueryKey(Keys.Space).Pressed) then
			Engine.ChangeMap("MAP01")
		end
	else
		local t_width = string.len(TITLE_INPUT_TEXT)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-t_width*4+1,
			math.floor(7*window.Height/window.Downscale/8)-3,
			80, 80, 80, 255, TITLE_INPUT_TEXT
		)
		window:DrawString(
			math.floor(window.Width/window.Downscale/2)-t_width*4,
			math.floor(7*window.Height/window.Downscale/8)-4,
			255, 255, 255, 255, TITLE_INPUT_TEXT
		)
	end

	local f = input:QueryKey(Keys.F)
	if (f.Pressed and f.OnEdge) then
		credits = not credits
	end
	yaw = lerp(yaw, credits and -math.pi/2 or 0, delta * 4)

	camera.Transform.Rotation = Vector3.new(0, yaw, 0)
	camera.Transform.Position = Vector3.new(0, math.sin(timer*4.0) * 0.05 + 2.0, 0)
	camera.FOV = math.rad(80)
end
