local timer = 0.0

local models = {
	"ORB",
}
local materials = {
	"ORB",
}

local current
local model = 1
local material = 1

local resource_manager

function init()
	current = Engine.CurrentGameObject
	resource_manager = Engine.ResourceManager
end

function process(delta)
	current.Transform.Position.y = 1.0 + math.sin(timer * math.pi * 2.0) * 0.2
	timer = timer + delta
	if (timer >= 1.0) then
		timer = 0.0
		model = model + 1
		material = material + 1
		if (model > #models) then model = 1 end
		if (material > #materials) then material = 1 end
	end
	current:SetMesh(resource_manager:GetMesh(models[model]))
	current:SetMaterial(resource_manager:GetMaterial(materials[material]))
end
