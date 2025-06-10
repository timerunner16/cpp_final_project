-- constants
local LEAN = 0.15
local LEANSPEED = 8.0
local MAX_GROUND_SPEED = 8.0
local MAX_AIR_SPEED = 3.0
local FRICTION = 8
local MAX_ACCEL = MAX_GROUND_SPEED * 10
local JUMPPOWER = 10.0
local GRAVITY = -30.0
local K_SENSITIVITY = 2.0
local M_SENSITIVITY = 3.14
local COYOTE_TIME_BASE = 0.2
local INPUT_BUFFER_BASE = 0.2
local DAMAGE_FLASH_COLOR = Vector4.new(1.0, 0.2, 0.2, 1.0)
local DAMAGE_FLASH_TIME = 0.2

-- properties
local timer = 0.0
local velocity = Vector3.new(0.0)
local bobstrength = 0.0
local x = 0.0
local wireframe = false
local coyote_time = 0.0
local input_buffer = 0.0
local last_hit_time = -DAMAGE_FLASH_TIME

-- object references
local current
local camera
local workspace
local input
local resource_manager
local window
local pdata
local event

local function friction(delta)
	local mag = Vector2.new(velocity.x, velocity.z).length
	if (mag > 0) then
		local drop = mag * FRICTION * delta
		velocity.x = velocity.x * math.max(mag-drop, 0) / mag
		velocity.z = velocity.z * math.max(mag-drop, 0) / mag
	end
end

local function add_velocity(new_velocity)
	velocity = velocity + new_velocity
end

local function clamp(n, min, max)
	if (n < min) then n = min end
	if (n > max) then n = max end
	return n
end

function takedamage()
	last_hit_time = timer
	pdata:SetValue("health", pdata:GetValue("health") - event:GetValue("damage"))
	add_velocity(event:GetValue("force"))
end

function init()
	workspace = Engine.Workspace
	current = Engine.CurrentGameObject
	camera = workspace:GetCamera()
	input = Engine.InputManager
	resource_manager = Engine.ResourceManager
	window = Engine.Window
	pdata = Engine.PDataManager
	event = workspace:CreateEvent("TakeDamage", current)
	event:Connect("takedamage")

	if (not pdata:GetValue("health") or pdata:GetValue("health") <= 0) then
		pdata:SetValue("health", 100)
	end
end

function process(delta)
	input_buffer = input_buffer - delta
	coyote_time = coyote_time - delta

	timer = timer + delta

	if (input:SomethingPressed()) then input:SetMouseCaptured(true) end

	local tab = input:QueryKey(Keys.Tab)
	if (tab.Pressed and tab.OnEdge) then
		wireframe = not wireframe
		Engine.Window:SetWireframeEnabled(wireframe)
	end

	local key_rotation_axis = input:GetAxis(Keys.Left, Keys.Right)
	key_rotation_axis = key_rotation_axis * delta * K_SENSITIVITY
	current.Transform.Rotation.y = current.Transform.Rotation.y + key_rotation_axis

	if (window.Focused) then
		local mouse_delta = Vector2.new(input:GetMouseDelta())
		local window_size = Vector2.new(window.Width, window.Height)
		mouse_delta = mouse_delta / window_size
		current.Transform.Rotation.y = current.Transform.Rotation.y + mouse_delta.x * M_SENSITIVITY
	else
		input:SetMouseCaptured(false)
	end

	local input_vector = input:GetVector(Keys.A, Keys.D, Keys.W, Keys.S)
	local lookvec = current.Transform.LookVector
	local rightvec = current.Transform.RightVector
	local wishdir = lookvec * input_vector.y + rightvec * input_vector.x
	local current_speed = velocity:dot(wishdir)

	local space = input:QueryKey(Keys.Space)
	if (current:IsOnFloor()) then
		if ((space.Pressed and space.OnEdge) or input_buffer > 0) then
			velocity.y = JUMPPOWER
			input_buffer = 0
			coyote_time = 0
		else
			friction(delta)
			coyote_time = COYOTE_TIME_BASE
			velocity.y = 0
		end

		local add_speed = clamp(MAX_GROUND_SPEED - current_speed,  0, MAX_ACCEL * delta)
		local new_velocity = wishdir * add_speed
		add_velocity(new_velocity)
	else
		if (space.Pressed and space.OnEdge) then
			if (coyote_time > 0) then
				velocity.y = JUMPPOWER
				coyote_time = 0
			else
				input_buffer = INPUT_BUFFER_BASE
			end
		end

		local add_speed = clamp(MAX_AIR_SPEED - current_speed,  0, MAX_ACCEL * delta)
		local new_velocity = wishdir * add_speed + Vector3.new(0,1,0) * GRAVITY * delta
		add_velocity(new_velocity)
	end

	current.Velocity = velocity

	x = x + (input_vector.x - x) * delta * LEANSPEED
	camera.Transform.Position = current.Transform.Position
	camera.Transform.Rotation.y = current.Transform.Rotation.y
	camera.Transform.Rotation.z = x * LEAN

	local target_bobstrength = math.min(math.max(Vector2.new(velocity.x, velocity.z).length,0.0),1.0)
	local velocity_flat = Vector2.new(velocity.x, velocity.z).unit
	local lookvec_flat = Vector2.new(lookvec.x, lookvec.z).unit
	local alignment = velocity_flat:dot(lookvec_flat)
	target_bobstrength = target_bobstrength * alignment
	bobstrength = bobstrength + (target_bobstrength - bobstrength) * delta * 4.0
	camera.Transform.Position.y = camera.Transform.Position.y + math.sin(timer * 8.0) * 0.05 * bobstrength + 1.0

	if (timer - last_hit_time < DAMAGE_FLASH_TIME or pdata:GetValue("health") <= 0) then
		resource_manager:GetMaterial("PPDITHER"):SetUniform(Uniform.new("modulate", DAMAGE_FLASH_COLOR))
	else
		resource_manager:GetMaterial("PPDITHER"):SetUniform(Uniform.new("modulate", Vector4.new(1.0)))
	end

	if (pdata:GetValue("health") <= 0) then
		current:QueueFree()
		camera.Transform.Position = current.Transform.Position:withY(current.Transform.Position.y + 0.1)
		camera.Transform.Rotation.z = 0
	end

	window:DrawString(2, math.floor(window.Height/window.Downscale)-9,
					  10, 80, 10, 255,
					  "+ " .. tostring(math.ceil(pdata:GetValue("health"))));
	window:DrawString(1, math.floor(window.Height/window.Downscale)-10,
					  30, 255, 30, 255,
					  "+ " .. tostring(math.ceil(pdata:GetValue("health"))));
end
