-- constants
local lean = 0.15
local leanspeed = 8.0
local speed = 5.0
local jumppower = 6.0
local gravity = 20.0
local k_sensitivity = 2.0
local m_sensitivity = 3.14

-- properties
local timer = 0.0
local velocity = Vector3.new(0.0)
local bobstrength = 0.0
local x = 0.0
local wireframe = false

-- object references
local current
local camera
local input
local window

function init()
	current = Engine.CurrentGameObject
	camera = Engine.Workspace:GetCamera()
	input = Engine.InputManager
	window = Engine.Window
end

function process(delta)
	timer = timer + delta

	for i,v in pairs(Keys) do
		print(i,v)
	end

	if (input:SomethingPressed()) then input:SetMouseCaptured(true) end

	local tab = input:QueryKey(Keys.Tab)
	if (tab.Pressed and tab.OnEdge) then
		wireframe = not wireframe
		Engine.Window:SetWireframeEnabled(wireframe)
	end

	local input_vector = input:GetVector(Keys.A, Keys.D, Keys.W, Keys.S)

	local key_Rotation_vector = input:GetVector(Keys.Left, Keys.Right, Keys.Up, Keys.Down)
	key_Rotation_vector = key_Rotation_vector:times(delta):times(k_sensitivity)
	current.Transform.Rotation.y = current.Transform.Rotation.y + key_Rotation_vector.x
	camera.Transform.Rotation.x = camera.Transform.Rotation.x + key_Rotation_vector.y

	if (window.Focused) then
		local mouse_delta = Vector2.new(input:GetMouseDelta())
		local window_size = Vector2.new(window.Width, window.Height)
		mouse_delta = mouse_delta:div(window_size)
		current.Transform.Rotation.y = current.Transform.Rotation.y + mouse_delta.x * m_sensitivity	
		camera.Transform.Rotation.x = camera.Transform.Rotation.x + mouse_delta.y * m_sensitivity
		camera.Transform.Rotation.x = math.min(math.max(camera.Transform.Rotation.x, -math.pi/2), math.pi/2)
	else
		input:SetMouseCaptured(false)
	end

	if (current.Transform.Position.y <= 0) then
		current.Transform.Position.y = 0
		velocity.y = 0
	end

	local space = input:QueryKey(Keys.Space)
	if (space.Pressed and current.Transform.Position.y == 0) then
		velocity.y = velocity.y + jumppower
	end
	velocity.y = velocity.y - delta * gravity

	velocity.x = 0
	velocity.z = 0
	local lookvec = current.Transform.LookVector
	local rightvec = current.Transform.RightVector
	velocity = velocity:plus(rightvec:times(input_vector.x):times(speed))
	velocity = velocity:plus(lookvec:times(input_vector.y):times(speed))

	if (not wireframe) then
		current.Velocity = velocity
	else
		current.Velocity = Vector3.new(0,0,0)
		current.Transform.Position = current.Transform.Position:plus(velocity:times(delta))
	end

	x = x + (input_vector.x - x) * delta * leanspeed
	camera.Transform.Position = current.Transform.Position
	camera.Transform.Rotation.y = current.Transform.Rotation.y
	camera.Transform.Rotation.z = x * lean

	local target_bobstrength = math.min(math.max(Vector2.new(velocity.x, velocity.z).length,0.0),1.0)
	local velocity_flat = Vector2.new(velocity.x, velocity.z).unit
	local lookvec_flat = Vector2.new(lookvec.x, lookvec.z).unit
	local alignment = velocity_flat:dot(lookvec_flat)
	target_bobstrength = target_bobstrength * alignment
	bobstrength = bobstrength + (target_bobstrength - bobstrength) * delta * 4.0
	camera.Transform.Position.y = camera.Transform.Position.y + math.sin(timer * 8.0) * 0.05 * bobstrength + 1.0

	Engine.Window:DrawString(0, 0,  255, 255, 0,   255,  "Look, I'm yellow!");
	Engine.Window:DrawString(0, 8,  255, 255, 255, 255,  string.format("FPS: %.2f", tostring(1.0/delta)));
end
