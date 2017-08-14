#pragma once
#include <Re\Game\Efect\EfectBase.h>
#include <Re\Graphics\ResourceManager.h>

namespace Efect
{
	/// efect which plays sounds and music from the actor position
	class SoundSource : public Base
	{
	public:
		~SoundSource();

		virtual void onUpdate(sf::Time dt) override;

		SoundSource* addSound(ResId soundId, float volume = 100.f, float attenuation = 1.f, float minDistance = 100.f, bool loop = false, float pitch = 1.f)
		{
			sounds.push_back(new sf::Sound(soundInst[soundId]) );
			sounds.back()->setRelativeToListener(true);
			sounds.back()->setLoop(loop);
			sounds.back()->setVolume(loop);
			sounds.back()->setAttenuation(loop);
			sounds.back()->setMinDistance(loop);
			sounds.back()->setPitch(loop);
			return this;
		}
		SoundSource* addMusic(ResId musicPathId, float volume = 100.f, float attenuation = 1.f, float minDistance = 100.f, bool loop = false, float pitch = 1.f)
		{
			musics.push_back(new sf::Music);
			musics.back()->openFromFile(pathInst[musicPathId]);
			musics.back()->setRelativeToListener(true);

			musics.back()->setLoop(loop);
			musics.back()->setVolume(loop);
			musics.back()->setAttenuation(loop);
			musics.back()->setMinDistance(loop);
			musics.back()->setPitch(loop);
			return this;
		}
		SoundSource* addMusic(const string& path, float volume = 100.f, float attenuation = 1.f, float minDistance = 100.f, bool loop = false, float pitch = 1.f)
		{
			musics.push_back(new sf::Music);
			musics.back()->openFromFile(path);
			musics.back()->setRelativeToListener(true);
			
			musics.back()->setLoop(loop);
			musics.back()->setVolume(loop);
			musics.back()->setAttenuation(loop);
			musics.back()->setMinDistance(loop);
			musics.back()->setPitch(loop);
			return this;
		}


		vector<sf::Sound*> sounds;
		vector<sf::Music*> musics;
	};

	/// updates position of sound listener to Game::Actor
	class SoundListener : public Base
	{
	public:

		virtual void onUpdate(sf::Time dt) override;


		SoundListener* setDirection(const Vector2D& s)
		{
			sf::Listener::setDirection(s.x, s.y, 0);
			return this;
		}
		SoundListener* setGlobalVolume(float s = 100.f)
		{
			sf::Listener::setGlobalVolume(s);
			return this;
		}
	};

}