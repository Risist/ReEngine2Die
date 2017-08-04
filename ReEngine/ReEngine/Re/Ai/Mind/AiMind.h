#pragma once
#include <Re\Common\utility.h>

namespace Ai
{
	class Mind;
	using Utility_t = float;

	/// class for representation singular behaviours
	/// they are choosen randomly with possibility to determine probability of choose each
	class BehaviourBase : public Res::ISerialisable
	{
	public:
		/// static BehaviourBase* createBehaviour(const string& type);
	public:
		/// called when the behaviour is choosen to be executed
		/// there should go all initialisation code
		virtual void onStart() {};
		/// executes the behaviour;
		/// @param:dt - delta of time elapsed between @onExecute calls
		/// @return - execution of the behaviour has ended?
		virtual bool onExecute(sf::Time dt) = 0;
		/// called when behaviour ends its execution
		/// there should go all clean up code
		virtual void onExit() {};
		/// tells the system how good is the behaviour in the current context
		virtual Utility_t getUtility() const = 0;


	public:
		/// name of behaviour used for identification at runtime
		string name;
	protected:
		Mind* getOwner() { return owner; };
		const Mind* getOwner() const { return owner; };
	private:
		Mind *owner;

		/// mind is friend to be able to initialise owner ptr;
		friend class Mind;


	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

	/// Main class that controlls data flow of behaviour system
	/// 
	class Mind : public Res::ISerialisable
	{
	public:
		Mind();
		~Mind();

		void onUpdate(sf::Time dt);

		/// adds a new possible behaviour to the mind
		template<typename Behaviour>
		Behaviour* addBehaviour(Behaviour* newBehaviour, const string& name = "")
		{
			if (!newBehaviour)
				return newBehaviour;

			behaviours.push_back(newBehaviour);
			newBehaviour->owner = this;
			newBehaviour->name += name;
			chance.chances.push_back(0);

			return newBehaviour;
		}

		void setBehaviour(BehaviourBase* s);
		void setBehaviour(size_t id);
		
		template<typename Behaviour>
		Behaviour* setNewBehaviour(Behaviour* s, const string& name = "")
		{
			setBehaviour(addBehaviour(s, name));
			return s;
		}


		/// allows to sign how big utility has to be to consider 
		/// a behaviour to choose as actual executed;
		/// each behaviour with utility less than treshold has 0 probability of being choosen
		Utility_t treshold;

	public:
		/// memory allows to hold history of behaviour executions

		/// returns how much behaviours are remembered
		size_t getMemorySize() const
		{
			return MEMORY_SIZE;
		}

		/// @param:pastTime - how many behaviours in past to go
		/// @return - a behaviour which was executed @pastTime cycles ago
		BehaviourBase* getFromMemory(size_t pastTime)
		{
			return behaviourMemory[pastTime];
		}
		const BehaviourBase* getFromMemory(size_t pastTime) const
		{
			return behaviourMemory[pastTime];
		}

	private:
		vector<BehaviourBase*> behaviours;
		RandomChance chance;

		/// actually executed behaviour, if equal to nullptr no behaviour is being execued
		BehaviourBase* actualBehaviour{nullptr};
		
		static const size_t MEMORY_SIZE = 5;
		BehaviourBase* behaviourMemory[MEMORY_SIZE];

	protected:
		virtual void serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const override;
		virtual void deserialiseF(std::istream& file, Res::DataScriptLoader& loader) override;
	};

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


	/// behaviour which does nothing for certain amount of time
	class BehaviourWait : public BehaviourBase
	{
	public:
		BehaviourWait(sf::Time _waitTime, Utility_t _utility)
			:waitTime(_waitTime), utility(_utility)
		{

		}

		virtual void onStart() { clock.restart();  }
		virtual bool onExecute() { return clock.getElapsedTime() < waitTime; }
		virtual Utility_t getUtility() const override { return utility; }
		
		Utility_t utility;
		sf::Time waitTime;
	private:
		Clock clock;

		///TODO serialisation
	};
}