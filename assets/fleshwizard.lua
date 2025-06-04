-- constants
local MAX_GROUND_SPEED = 3.0
local MAX_AIR_SPEED = 1.0
local FRICTION = 8
local MAX_ACCEL = MAX_GROUND_SPEED * 10
local GRAVITY = -30.0

-- properties
local velocity = Vector3.new()

-- object references
local workspace

local current
local player

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

local function atan2(y, x)
	local angle = math.atan(y/x)
	if (y < 0 and x < 0) then
		angle = angle - math.pi
	elseif (y > 0 and x < 0) then
		angle = angle + math.pi
	end
	return angle
end

function init()
	workspace = Engine.Workspace
	current = Engine.CurrentGameObject
end

function process(delta)
	if (not player) then
		player = workspace:GetGameObject("Observer")
		return
	end

	local offset = player.Transform.Position - current.Transform.Position
	local wishdir = Vector3.new(offset.x, 0, offset.z).unit
	local current_speed = velocity:dot(wishdir)

	if (current:IsOnFloor()) then
		friction(delta)
		velocity.y = 0

		local add_speed = clamp(MAX_GROUND_SPEED - current_speed,  0, MAX_ACCEL * delta)
		local new_velocity = wishdir * add_speed
		add_velocity(new_velocity)
	else
		local add_speed = clamp(MAX_AIR_SPEED - current_speed,  0, MAX_ACCEL * delta)
		local new_velocity = wishdir * add_speed + Vector3.new(0,1,0) * GRAVITY * delta
		add_velocity(new_velocity)
	end

	current.Transform.Rotation.y = -math.pi/2 + math.pi - atan2(velocity.z, velocity.x)
	current.Velocity = velocity
end
