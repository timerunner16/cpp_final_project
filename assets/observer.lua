local timer = 0.0
local prefix = "observer.lua: "

function init()
	print("hi my name is " .. Engine.CurrentGameObject:GetName())
end

function process(delta)
	for i,v in pairs(Engine.Workspace:GetGameObject("root"):GetChild("suzanne"):GetChildren()) do
		print(prefix .. tostring(i) .. ": " .. v:GetName())
	end

	timer = timer + delta
end
