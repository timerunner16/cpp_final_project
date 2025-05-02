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

function init()
	current = Engine.CurrentGameObject
	camera = Engine.Workspace:GetCamera()
end

function process(delta)
	timer = timer + delta

	for i,v in pairs(Keys) do
		print(i,v)
	end

	local input = Engine.InputManager
	local tab = input:QueryKey(Keys.Tab)
	if (tab.Pressed and tab.OnEdge) then
		wireframe = not wireframe
		Engine.Window:SetWireframeEnabled(wireframe)
	end

	local input_vector = input:GetVector(Keys.A, Keys.D, Keys.W, Keys.S)

	local key_rotation_vector = input:GetVector(Keys.Left, Keys.Right, Keys.Up, Keys.Down)
	key_rotation_vector = key_rotation_vector:times(delta):times(k_sensitivity)
	current.transform.rotation.y = current.transform.rotation.y + key_rotation_vector.x
	camera.transform.rotation.x = camera.transform.rotation.x + key_rotation_vector.y

	input:SetMouseCaptured(true)
	local mouse_delta = Vector2.new(input:GetMouseDelta())
	local window_size = Vector2.new(Engine.Window.Width, Engine.Window.Height)
	mouse_delta = mouse_delta:div(window_size)
	current.transform.rotation.y = current.transform.rotation.y + mouse_delta.x * m_sensitivity	
	camera.transform.rotation.x = camera.transform.rotation.x + mouse_delta.y * m_sensitivity
	camera.transform.rotation.x = math.min(math.max(camera.transform.rotation.x, -math.pi/2), math.pi/2)

	if (current.transform.position.y <= 0) then
		current.transform.position.y = 0
		velocity.y = 0
	end

	local space = input:QueryKey(Keys.Space)
	if (space.Pressed and current.transform.position.y == 0) then
		velocity.y = velocity.y + jumppower
	end
	velocity.y = velocity.y - delta * gravity

	velocity.x = 0
	velocity.z = 0
	local lookvec = current.transform.LookVector
	local rightvec = current.transform.RightVector
	velocity = velocity:plus(rightvec:times(input_vector.x):times(speed))
	velocity = velocity:plus(lookvec:times(input_vector.y):times(speed))

	current.transform.position = current.transform.position:plus(velocity:times(delta))

	x = x + (input_vector.x - x) * delta * leanspeed
	camera.transform.position = current.transform.position
	camera.transform.rotation.y = current.transform.rotation.y
	camera.transform.rotation.z = x * lean

	local target_bobstrength = math.min(math.max(Vector2.new(velocity.x, velocity.z).length,0.0),1.0)
	local velocity_flat = Vector2.new(velocity.x, velocity.z).unit
	local lookvec_flat = Vector2.new(lookvec.x, lookvec.z).unit
	local alignment = velocity_flat:dot(lookvec_flat)
	target_bobstrength = target_bobstrength * alignment
	bobstrength = bobstrength + (target_bobstrength - bobstrength) * delta * 4.0
	camera.transform.position.y = camera.transform.position.y + math.sin(timer * 8.0) * 0.05 * bobstrength + 1.0
end
