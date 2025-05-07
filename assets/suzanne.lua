local timer = 0.0

local models = {
	"CUBE",
	"SUZANNE"
}
local materials = {
	"TEST0",
	"TEST1"
}

local current
local model = 1
local material = 1

function init()
	current = Engine.CurrentGameObject
end

function process(delta)
	timer = timer + delta
	if (timer >= 1.0) then
		timer = 0.0
		model = model + 1
		material = material + 1
		if (model > #models) then model = 1 end
		if (material > #materials) then material = 1 end
	end
	current:SetMesh(Engine.ResourceManager:GetMesh(models[model]))
	current:SetMaterial(Engine.ResourceManager:GetMaterial(materials[material]))
end
