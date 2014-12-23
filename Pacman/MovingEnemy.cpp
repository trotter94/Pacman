#include "MovingEnemy.h"


MovingEnemy::MovingEnemy() : _cEnemyMinDirectionTime(100), _cEnemyDistanceStartChase(200.0f), _cEnemyDistanceStopChase(250.0f)
{
	_direction = 0;
	_directionTime = (rand() % 1500) + 1000;
	_currentDirectionTime = 0;
	_speed = 0.2f;
	_behaviour = static_cast<EnemyBehaviour>((rand() % 2) + 1);
	//_behaviour = EnemyBehaviour::CHASE;
}


MovingEnemy::~MovingEnemy()
{
}


void MovingEnemy::Load(Texture2D* texture)
{
	_texture = texture;
	_posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
	_sourceRect = new Rect(0.0f, 0.0f, 20, 20);
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

	if (_behaviour == EnemyBehaviour::MOVE_RANDOM)
	{
		_speed = 0.1f;
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

		if (Vector2::Distance(_pacman->GetPosition().Center(), _posRect->Center()) < _cEnemyDistanceStartChase)
		{
			_previousBehaviour = _behaviour;
			_behaviour = EnemyBehaviour::CHASE;
			//
		}
	}
	else if (_behaviour == EnemyBehaviour::CHASE)
	{
		if (!_pacman->IsDead())
		{
			_speed = 0.15f;
			_sourceRect->X = 40.0f;
			if (_currentDirectionTime > _cEnemyMinDirectionTime)
			{
				if (abs(_posRect->X - _pacman->GetPosition().X) > abs(_posRect->Y - _pacman->GetPosition().Y))
				{
					if (_posRect->X > _pacman->GetPosition().X)
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
					if (_posRect->Y > _pacman->GetPosition().Y)
					{
						_direction = 3;
					}
					else
					{
						_direction = 1;
					}
				}
			}
			if (Vector2::Distance(_pacman->GetPosition().Center(), _posRect->Center()) > _cEnemyDistanceStopChase)
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
		_speed = 0.1f;
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
		if (Vector2::Distance(_pacman->GetPosition().Center(), _posRect->Center()) < _cEnemyDistanceStartChase)
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


void MovingEnemy::SetPlayer(Player* player)
{
	_pacman = player;
}


const Rect& MovingEnemy::GetBoundingRect()
{
	return *_posRect;
}