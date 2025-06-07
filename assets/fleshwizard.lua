-- constants
local MAX_GROUND_SPEED = 6.0
local MAX_AIR_SPEED = 2.0
local FRICTION = 8
local MAX_ACCEL = MAX_GROUND_SPEED * 10
local GRAVITY = -30.0
local DAMAGE_FLASH_COLOR = Vector4.new(1.0, 0.2, 0.2, 1.0)
local DAMAGE_FLASH_TIME = 0.2
local ATTACK_RANGE = 3.0
local HIT_RANGE = 2.0
local ATTACK_COOLDOWN = 1.5
local ATTACK_DELAY = 0.8
local ATTACK_DAMAGE = 45
local ATTACK_FORCE = 20.0
local LUNGE_DELAY = 0.4
local LUNGE_FLASH_COLOR = Vector4.new(0.9, 1.0, 0.2, 0.7)
local ACTIVATION_RANGE = 16.0
local STATES = {
	IDLE = "idle",
	MOVING = "moving",
	ATTACKING = "attacking",
	LUNGING = "lunging"
}

-- properties
local velocity = Vector3.new()
local health = 100
local state_timer = 0
local global_timer = 0
local last_hit_time = -DAMAGE_FLASH_TIME
local state = STATES.IDLE
local hit_player_in_lunge = false
local friction_mod = 1.0

-- object references
local workspace
local current
local event
local destroyed

local function friction(delta)
	local mag = Vector2.new(velocity.x, velocity.z).length
	if (mag > 0) then
		local drop = mag * FRICTION * friction_mod * delta
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

local function process_movement(delta, wishdir)
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
end

function takedamage()
	state = STATES.MOVING
	state_timer = 0
	last_hit_time = global_timer
	local damage = event:GetValue("damage")
	local force = event:GetValue("force")
	add_velocity(force)
	health = health - damage

	local position = event:GetValue("position")
	local direction = event:GetValue("direction")
	local hit_particle_info = ParticleSystemCreateInfo.new()
	hit_particle_info.Position = position * 2
	hit_particle_info.Direction = direction
	hit_particle_info.Size = Vector2.new(0.05, 0.05)
	hit_particle_info.Lifetime = 0.4
	hit_particle_info.Gravity = -9.81
	hit_particle_info.Randomization = math.pi/8
	hit_particle_info.NumParticles = 64
	hit_particle_info.NumLaunches = 1
	hit_particle_info.R = 150
	hit_particle_info.G = 30
	hit_particle_info.B = 150
	hit_particle_info.FadeOut = true
	workspace:CreateParticleSystem(hit_particle_info)

	if (health <= 0) then
		local dead_particle_info = ParticleSystemCreateInfo.new()
		dead_particle_info.Position = current.Transform.Position * 2 + Vector3.new(0,1,0)
		dead_particle_info.Direction = Vector3.new(0,1,0)
		dead_particle_info.Size = Vector2.new(0.05, 0.05)
		dead_particle_info.Lifetime = 0.6
		dead_particle_info.Gravity = -9.81
		dead_particle_info.Randomization = math.pi/4
		dead_particle_info.NumParticles = 32
		dead_particle_info.NumLaunches = 4
		dead_particle_info.LaunchInterval = 0.05
		dead_particle_info.R = 150
		dead_particle_info.G = 30
		dead_particle_info.B = 150
		dead_particle_info.FadeOut = true
		workspace:CreateParticleSystem(dead_particle_info)

		current:QueueFree()
	end
end

function init()
	workspace = Engine.Workspace
	current = Engine.CurrentGameObject
	event = workspace:CreateEvent("TakeDamage", current)
	event:Connect("takedamage")
	destroyed = workspace:CreateEvent("Destroyed", current)
end

function process(delta)
	local player = workspace:GetGameObject("Observer")
	if (not player) then
		return
	end

	state_timer = state_timer + delta
	global_timer = global_timer + delta

	if (state == STATES.IDLE) then
		wishdir = Vector3.new(0,0,0)
		if ((player.Transform.Position - current.Transform.Position).length < ACTIVATION_RANGE) then
			state = STATES.MOVING
			state_timer = 0
		end
	elseif (state == STATES.MOVING) then
		local offset = player.Transform.Position - current.Transform.Position
		wishdir = Vector3.new(offset.x, 0, offset.z).unit

		if ((current.Transform.Position - player.Transform.Position).length < ATTACK_RANGE and state_timer > ATTACK_COOLDOWN) then
			state = STATES.ATTACKING
			state_timer = 0
		end
	elseif (state == STATES.ATTACKING) then
		wishdir = Vector3.new(0,0,0)

		if (state_timer > ATTACK_DELAY) then
			local offset = (player.Transform.Position:withY(current.Transform.Position.y) - current.Transform.Position)
			local force = offset.unit * ATTACK_FORCE
			add_velocity(force)

			state = STATES.LUNGING
			state_timer = 0
			hit_player_in_lunge = false
		end
	elseif (state == STATES.LUNGING) then
		friction_mod = 0.35
		local offset = player.Transform.Position - current.Transform.Position
		if (not hit_player_in_lunge and offset.length < HIT_RANGE and offset.unit:dot(velocity.unit) > 0.5) then
			local attack_event = player:GetEvent("TakeDamage")
			local force = offset.unit * ATTACK_FORCE
			attack_event:SetValue("damage", ATTACK_DAMAGE)
			attack_event:SetValue("force", force)
			attack_event:Fire()
			hit_player_in_lunge = true
		end
		if (state_timer > LUNGE_DELAY) then
			state = STATES.IDLE
			state_timer = 0
			friction_mod = 1.0
		end
	end

	process_movement(delta, wishdir)

	current.Transform.Rotation.y = math.pi/2 - atan2(velocity.z, velocity.x)
	current.Velocity = velocity

	if (global_timer - last_hit_time < DAMAGE_FLASH_TIME) then
		current:SetUniform(Uniform.new("modulate", DAMAGE_FLASH_COLOR))
	elseif (state == STATES.LUNGING) then
		current:SetUniform(Uniform.new("modulate", LUNGE_FLASH_COLOR))
	else
		current:RemoveUniform("modulate")
	end
end

function on_destruct()
	destroyed:Fire()
end
