#include "MovingEnemy.h"


MovingEnemy::MovingEnemy(Map& map) : _cEnemyMinDirectionTime(100), _cSightDistance(200.0f), _map(map)
{
	_direction = 0;
	_directionTime = (rand() % 1500) + 1000;
	_currentDirectionTime = 0;
	_speed = 0.1f;
	_behaviour = static_cast<EnemyBehaviour>((rand() % 2) + 1);
	//_behaviour = EnemyBehaviour::CHASE;
}


MovingEnemy::~MovingEnemy()
{
}


void MovingEnemy::Load(Texture2D* texture, Player* pacman)
{
	_texture = texture;
	_posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
	_sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	_pacman = pacman;
	_lastKnownPlayerPos = _pacman->GetPosition().Center();
}


void MovingEnemy::Update(int elapsedTime)
{
	int startDirection = _direction;

	if (_direction == 0)
	{
		//move right
		_posRect->X += _speed * elapsedTime;
	}
	else if (_direction == 2)
	{
		//move left
		_posRect->X -= _speed * elapsedTime;
	}
	else if (_direction == 1)
	{
		//move down
		_posRect->Y += _speed * elapsedTime;
	}
	else if (_direction == 3)
	{
		//move up
		_posRect->Y -= _speed * elapsedTime;
	}

	bool playerInSight = Vector2::Distance(_pacman->GetPosition().Center(), this->GetPosition().Center()) < _cSightDistance && _map.InLineOfSight(_pacman->GetPosition().Center(), this->GetPosition().Center());
	
	if (playerInSight)
		_lastKnownPlayerPos = _pacman->GetPosition().Center();

	if (_behaviour == EnemyBehaviour::MOVE_RANDOM)
	{
		_sourceRect->X = 0;

		if (_posRect->X + _posRect->Width >= Graphics::GetViewportWidth())
		{
			_direction = 2;
		}
		else if (_posRect->X <= 0)
		{
			_direction = 0;
		}
		else if (_posRect->Y + _posRect->Height >= Graphics::GetViewportHeight())
		{
			_direction = 3;
		}
		else if (_posRect->Y <= 0)
		{
			_direction = 1;
		}

		if (_currentDirectionTime >= _directionTime)
		{
			_direction = rand() % 4;
		}

		if (playerInSight)
		{
			_previousBehaviour = _behaviour;
			_behaviour = EnemyBehaviour::CHASE;
		}
	}
	else if (_behaviour == EnemyBehaviour::CHASE)
	{
		if (!_pacman->IsDead())
		{
			_sourceRect->X = 40.0f;

			MoveTowards(_lastKnownPlayerPos);
			
			if (this->GetPosition().Contains(_lastKnownPlayerPos))
			{
				_behaviour = _previousBehaviour;
				_previousBehaviour = EnemyBehaviour::CHASE;
			}

		}
		else
		{
			_behaviour = _previousBehaviour;
			_previousBehaviour = EnemyBehaviour::CHASE;
		}
	}
	else if (_behaviour == EnemyBehaviour::PATROL)
	{
		_sourceRect->X = 20.0f;
		if (_direction == 0)
		{
			if (_posRect->X + _posRect->Width >= Graphics::GetViewportWidth())
			{
				_direction = 3;
			}
		}
		if (_direction == 2)
		{
			if (_posRect->X <= 0)
			{
				_direction = 1;
			}
		}
		if (_direction == 1)
		{
			if (_posRect->Y + _posRect->Height >= Graphics::GetViewportHeight())
			{
				_direction = 0;
			}
		}
		if (_direction == 3)
		{
			if (_posRect->Y <= 0)
			{
				_direction = 2;
			}
		}
		if (playerInSight)
		{
			_previousBehaviour = _behaviour;
			_behaviour = EnemyBehaviour::CHASE;
		}
	}

	if (startDirection != _direction)
	{
		_currentDirectionTime = 0;
		_directionTime = (rand() % 1500) + 1000;
	}
	else
	{
		_currentDirectionTime += elapsedTime;
	}

	_sourceRect->Y = _sourceRect->Height * _direction;
}


void MovingEnemy::MoveTowards(const Vector2& position)
{
	if (_currentDirectionTime > _cEnemyMinDirectionTime)
	{
		if (abs(_posRect->X - position.X) > abs(_posRect->Y - position.Y))
		{
			if (_posRect->X > position.X)
			{
				_direction = 2;
			}
			else
			{
				_direction = 0;
			}
		}
		else
		{
			if (_posRect->Y > position.Y)
			{
				_direction = 3;
			}
			else
			{
				_direction = 1;
			}
		}
	}
}


const Rect& MovingEnemy::GetBoundingRect()
{
	return *_posRect;
}