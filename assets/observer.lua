local x = 0.0
local velocity = Vector3.new(0.0)
local lean = 0.2
local speed = 3.0
local jumppower = 6.0
local gravity = 20.0
local sensitivity = 3.14

local current
local camera

function init()
	current = Engine.CurrentGameObject
	camera = Engine.Workspace:GetCamera()
end

function process(delta)
	for i,v in pairs(Keys) do
		print(i,v)
	end

	local input = Engine.InputManager
	local input_vector = Vector2.new()
	if (input:QueryKey(Keys.A).Pressed) then
		input_vector.x = input_vector.x - 1.0
	end
	if (input:QueryKey(Keys.D).Pressed) then
		input_vector.x = input_vector.x + 1.0
	end
	if (input:QueryKey(Keys.W).Pressed) then
		input_vector.y = input_vector.y - 1.0
	end
	if (input:QueryKey(Keys.S).Pressed) then
		input_vector.y = input_vector.y + 1.0
	end

	if (input:QueryKey(Keys.Left).Pressed) then
		current.transform.rotation.y = current.transform.rotation.y - delta
	end
	if (input:QueryKey(Keys.Right).Pressed) then
		current.transform.rotation.y = current.transform.rotation.y + delta
	end
	if (input:QueryKey(Keys.Up).Pressed) then
		camera.transform.rotation.x = camera.transform.rotation.x - delta
	end
	if (input:QueryKey(Keys.Down).Pressed) then
		camera.transform.rotation.x = camera.transform.rotation.x + delta
	end

	input:SetMouseCaptured(true)
	local mouse_delta = Vector2.new(input:GetMouseDelta())
	local window_size = Vector2.new(Engine.Window.Width, Engine.Window.Height)
	mouse_delta = mouse_delta:div(window_size)
	current.transform.rotation.y = current.transform.rotation.y + mouse_delta.x * sensitivity
	
	camera.transform.rotation.x = camera.transform.rotation.x + mouse_delta.y * sensitivity
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
	velocity = velocity:plus(current.transform.RightVector:times(input_vector.x):times(speed))
	velocity = velocity:plus(current.transform.LookVector:times(input_vector.y):times(speed))

	current.transform.position = current.transform.position:plus(velocity:times(delta))

	x = x + (input_vector.x - x) * delta * 4.0
	camera.transform.position = current.transform.position
	camera.transform.rotation.y = current.transform.rotation.y
	camera.transform.rotation.z = x * lean
end
