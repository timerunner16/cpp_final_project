local timer = 0.0
local prefix = "suzanne.lua: "

function init()
	print("hi ima suzanne")
end

function process(delta)
	timer = timer + delta
	_GameObject.transform.position.y = math.sin(timer)
	_GameObject.transform.rotation.z = timer
	if (_GameObject.transform.position.y < 0) then
		print(prefix .. "im GOING DOWN!!!!!!!!!!!")
	else
		print(prefix .. "im GOING UP!!!!!!!!!!!!!")
	end
end
