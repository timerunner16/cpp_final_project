local timer = 0.0
local prefix = "signal_test.lua: "

function init()
	print("hi ima signal")
end

function process(delta)
	local suzanne = GetGameObject("root"):GetChild("suzanne")
	if (suzanne.transform.position.y < 0) then
		print(prefix .. "suzanne is GOING DOWN!!!!!!!!!!")
	else
		print(prefix .. "suzanne is GOING UP!!!!!!!!!!!!")
	end

	timer = timer + delta
end
