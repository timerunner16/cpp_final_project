local timer = 0.0

function init()
	print("Initialized...")
end

function process(delta)
	timer = timer + delta
	_GameObject.transform.scale.x = math.abs(math.sin(timer))
	_GameObject.transform.rotation.x = timer
	_GameObject.transform.position.y = math.sin(timer)
end
