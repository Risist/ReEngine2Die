#include "WarriorPlayer.h"
#include "../Layers.h"



void WarriorPlayer::onInit()
{
	Game::Actor::onInit();
	
	/// graphics
	auto efModel = addEfect(new Efect::Model("model_reaper.txt"));
	auto efAnimation = addEfect(new Efect::AnimationManager(efModel->modelsUpdate));
	
				  &efAnimation->addAnimation("anim_reaperPose.txt")->getAnimation();
	slash		= &efAnimation->addAnimation("anim_reaperSlash.txt")->getAnimation();
	pull		= &efAnimation->addAnimation("anim_reaperPull.txt")->getAnimation();
	push		= &efAnimation->addAnimation("anim_reaperPush.txt")->getAnimation();
	walk		= &efAnimation->addAnimation("anim_reaperWalk.txt")->getAnimation();
	greatSlash	= &efAnimation->addAnimation("anim_reaperGreatSlash.txt")->getAnimation();/**/

	/// physics
	addEfect(new Efect::Rigidbody(25, 20));
	addEfect(new Efect::ColliderCircle(50.f));
	addEfect(new Efect::UpdateTransform());

	efMovement = addEfect(new Efect::MouseMovement(25, new Efect::RotateToDirection(Efect::RotateToDirection::smoothPhysics, 0.005) ));
	addEfect(new Efect::FollowCamera(Efect::FollowCamera::positionOnlySmooth))->setLerpPosition(0.125);
}

void WarriorPlayer::onUpdate(sf::Time dt)
{
	Actor::onUpdate(dt);
	
	/**
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		push.updateInRange(push.speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		push.updateInRange(-push.speed);
	/**/

	/**/
	{
		//static bool onSlash = false, onSlashBack = false;
		//static float slashLoadedStep = 0;
		
		if (onSlashBack )
		{
			if (slash->updateTowards(0, 0.75*(0.25 + slashLoadedStep*0.75) ) )
				onSlashBack = onSlash = false;
		}else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			onSlash = true;
			slash->updateInRange(-slash->getSpeedSign()*0.85);
			slashLoadedStep = slash->step/ slash->stepMin;
		}
		else if (!onSlashBack && onSlash && 
			slash->updateTowards((0.3 + slashLoadedStep*0.7)* slash->stepMax,
				slash->getSpeedSign()*2.25*
				(0.25 + slashLoadedStep*slashLoadedStep*0.75)
				) )
			onSlashBack = true;
		
	}
	

	if(efMovement->getArrived() == false)
		walk->updateReturn();

	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			onGreatSlash = true;
		if (onGreatSlash && greatSlash->updateRestart())
			onGreatSlash = false;
	}

	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			onPull = true;
		if (onPull && pull->updateRestart())
			onPull = false;
	}
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			onPush = true;
		if (onPush && push->updateRestart())
			onPush = false;
	}

	if (onSlash || onPull || onPull || onGreatSlash)
		efMovement->reset();
	/**/
}
