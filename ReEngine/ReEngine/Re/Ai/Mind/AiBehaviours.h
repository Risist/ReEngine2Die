#pragma once
#include <Re\Ai\AiEmotionDef.h>
#include <Re\Ai\Mind\AiMind.h>

namespace Ai
{
	////////////////////////////////////////////////////////////////////////////////
	/// simplified behaviour to fullfil emotional behaviours purpose 
	class BehaviourEmotion : public BehaviourBase
	{
	public:
		virtual Utility_t getUtility() const override;

		EmotionDef *ownersMood;
		EmotionDef behaviourEmotion;
		Utility_t baseUtility{ 0 }, scaleUtility{ 1 };


	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

	//////////////////////////////////////////////////
	/// behaviour with lambda functions on events
	class BehaviourLambda : public BehaviourBase
	{
	public:

		typedef function<void()> onStart_t;
		typedef function<bool(sf::Time)> onExecute_t;
		typedef function<void()> onExit_t;
		typedef function<Utility_t()> getUtility_t;

		BehaviourLambda* setOnStart(onStart_t __onStart)
		{
			_onStart = __onStart;
			return this;
		}
		BehaviourLambda* setOnExit(onExit_t __onExit)
		{
			_onExit = __onExit;
			return this;
		}
		BehaviourLambda* setOnExecute(onExecute_t __onExecute)
		{
			_onExecute = __onExecute;
			return this;
		}
		BehaviourLambda* setGetUtility(getUtility_t __getUtility)
		{
			_getUtility = __getUtility;
			return this;
		}
		BehaviourLambda* setGetUtility(Utility_t s)
		{
			_getUtility = [=]() {return s; };
			return this;
		}

		virtual void onStart() override { if (_onStart) _onStart(); }
		virtual bool onExecute(sf::Time dt)override { if (_onExecute) return _onExecute(dt); else return false; }
		virtual void onExit() override { if (_onExit) _onExit(); }
		virtual Utility_t getUtility() const override { if (_getUtility) return _getUtility(); }

		onStart_t _onStart{ []() {} };
		onExecute_t _onExecute{ [](sf::Time dt) { return true; } };
		onExit_t _onExit{ []() {} };
		getUtility_t _getUtility{ []() {return 1.f; } };
	};

	///////////////////////////////////////////////////////////////////////
	/// behaviour which does nothing for certain amount of time
	class BehaviourWait : public BehaviourBase
	{
	public:
		BehaviourWait(sf::Time _waitTime, Utility_t _utility)
			:waitTime(_waitTime), utility(_utility)
		{

		}

		virtual void onStart() { clock.restart(); }
		virtual bool onExecute() { return clock.getElapsedTime() < waitTime; }
		virtual Utility_t getUtility() const override { return utility; }

		Utility_t utility;
		sf::Time waitTime;
	private:
		Clock clock;

	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

}