#pragma once
#include <Re\ReEngine.h>
#include <Re\Game\Efect\efects.h>


class WarriorPlayer : public Game::Actor
{
public:

	void onInit() override;
	void onUpdate(sf::Time dt) override;

	/// animations 
	Graphics::AnimationController* slash{ nullptr };
	Graphics::AnimationController* greatSlash{ nullptr };
	Graphics::AnimationController* walk{ nullptr };
	Graphics::AnimationController* push{ nullptr };
	Graphics::AnimationController* pull{ nullptr };

	/// 
	Efect::MouseMovement *efMovement{nullptr};
};