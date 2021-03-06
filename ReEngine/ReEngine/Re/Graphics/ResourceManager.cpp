#include <Re\Graphics\ResourceManager.h>

Image ResourceManager::im;
Color ResourceManager::clTransparent(255, 255, 255);

ResourceManager res;

ResourceManager::ResourceManager()
{
}

void ResourceManager::createTexture(Texture& ts, const std::string & path)
{
	if (im.loadFromFile(path))
	{
		im.createMaskFromColor(clTransparent);
		ts.loadFromImage(im);
	}
	else
	{
		std::cout << "Failed to open the atlas: \"" << path << "\"" << std::endl;
	}
}

string ResourceManager::createTranscription(const std::string & path)
{
	fstream file(path);
	string s;
	char c;
	while (file>>c)
	{
		s.push_back(c);
	}

	return s;
}

void ResourceManager::serialiseF(std::ostream& file, Res::DataScriptSaver& saver) const
{
	// does not save resources
}
void ResourceManager::deserialiseF(std::istream& file, Res::DataScriptLoader& loader)
{
	// heder line
	std::string prepathGraphics = loader.loadRaw("prepathGraphics", "");
	std::string prepathFont = loader.loadRaw("prepathFont", "");
	std::string prepathSound = loader.loadRaw("prepathSound", "");
	std::string prepathScript = loader.loadRaw("prepathScript", "");
	std::string prepathResource = loader.loadRaw("prepathResource", "");
	std::string prepathPath = loader.loadRaw("prepathPath", "");

	/// 0 index always means empty resource
	res.atlases[0] = Texture();
	res.textures[0] = TextureInstance();
	res.fonts[0] = Font();
	res.cashedScript[0] = string();
	res.sounds[0] = sf::SoundBuffer();

	DATA_SCRIPT_MULTILINE(file, loader)
	{
		string type = loader.load<string>("type", "UNKNOWN");

		/// load texture from file
		if (type == "atlas")
		{
			std::string path = prepathGraphics + loader.loadRaw("path", "non_written_path_error");

			ResId atlasId = loader.load("atlasId", 0u);
			ResId textureId = loader.load("textureId", atlasId);

			if (path == prepathGraphics + "non_written_path_error")
			{
				cerr << "ResourceManager: in type = " << type << "; path = " << path << "\n";
			}else
			if (atlasId != 0u)
			{
				createTexture(atlases[atlasId], path);
				if (textureId != 0u)
				{
					textures[textureId].bounds.width = atlases[atlasId].getSize().x;
					textures[textureId].bounds.height = atlases[atlasId].getSize().y;
					textures[textureId].atlasId = atlasId;
				}
			}
			else
			{
				cerr << "ResourceManager: in " << type << "; atlas id not set up\n";
			}
		}
		/// set up texture bounds for one texture placed in atlas
		else if (type == "textureSingle")
		{
			ResId atlasId = loader.load("atlasId", 0u);

			/// id from which texture instances will be added
			ResId textureId = loader.load("textureId", 0u);


			if (atlasId != 0u && textureId != 0u)
			{
				textures[textureId].bounds.left = loader.load("posX", 0u);
				textures[textureId].bounds.top = loader.load("posY", 0u);

				textures[textureId].bounds.width = loader.load("width", atlases[atlasId].getSize().x);
				textures[textureId].bounds.height = loader.load("height", atlases[atlasId].getSize().y);

				textures[textureId].atlasId = atlasId;
			}
			else
			{
				cerr << "ResourceManager: in " << type << "; atlas or texture id not set up\n";
			}

		}
		/// set up texture bounds with constant grid
		/// its like every picture in texture has the same size
		else if (type == "textureGridX")
		{
			ResId atlasId = loader.load("atlasId", 0u);

			/// id from which texture instances will be added
			ResId textureId = loader.load("textureFromId", 0u);

			size_t n = loader.load("n", 1u);

			if (atlasId && textureId != 0u)
			{
				unsigned int posX = loader.load("posX", 0u);
				unsigned int posY = loader.load("posY", 0u);

				const unsigned int w = loader.load("width", 0u);
				const unsigned int h = loader.load("height", 0u);

				for (int i = 0; i < n; ++i)
				{
					textures[textureId].bounds.left = posX;
					textures[textureId].bounds.top = posY;
					posX += w;
					//posY += h;

					textures[textureId].bounds.width = w;
					textures[textureId].bounds.height = h;
				}
				textures[textureId].atlasId = atlasId;
			}
			else
			{
				cerr << "ResourceManager: in " << type << " atlas or texture id not set up\n";
			}
		}
		else if (type == "textureGridY")
		{
			ResId atlasId = loader.load("atlasId", 0u);

			/// id from which texture instances will be added
			ResId textureId = loader.load("textureFromId", 0u);

			size_t n = loader.load("n", 1u);

			if (atlasId && textureId != 0u)
			{
				unsigned int posX = loader.load("posX", 0u);
				unsigned int posY = loader.load("posY", 0u);

				const unsigned int w = loader.load("width", 0u);
				const unsigned int h = loader.load("height", 0u);

				for (int i = 0; i < n; ++i)
				{
					textures[textureId].bounds.left = posX;
					textures[textureId].bounds.top = posY;
					//posX += w;
					posY += h;

					textures[textureId].bounds.width = w;
					textures[textureId].bounds.height = h;
				}
				textures[textureId].atlasId = atlasId;
			}
			else
			{
				cerr << "ResourceManager: in " << type << " atlas or texture id not set up\n";
			}
		}
		/// cashe scripts in memory for faster loading
		else if (type == "script")
		{
			std::string path = prepathScript + loader.loadRaw("path", "non_written_path_error");
			ResId id = loader.load("id", 0u);
			if (id == 0u)
				cerr << "RespurceManager: in " << type << "id not set or set to 0\n";
			else
			{
				cashedScript[id] = createTranscription(path);
			}
		}
		/// load sounds from files
		else if (type == "sound")
		{
			std::string path = prepathSound + loader.loadRaw("path", "non_written_path_error");

			ResId id = loader.load("id", 0u);
			if (id == 0u)
				std::cerr << "ResourceManager: in type = " << type << 
					"; id = nonon_named\n"
					"\tPath = " << path << "\n";


			sounds[id].loadFromFile(path);
		}
		/// load fonts from files
		else if (type == "font")
		{
			std::string path = prepathFont + loader.loadRaw("path", "non_written_path_error");

			ResId id = loader.load("id", 0u);
			if (id == 0u)
				std::cerr << "ResourceManager: in type = " << type << "; id = nonon_named\n"
					"\tPath = " << path << "\n";


			fonts[id].loadFromFile(path);
		}
		else if (type == "path")
		{
			std::string path = prepathPath + loader.loadRaw("path", "non_written_path_error");

			ResId id = loader.load("id", 0u);
			if (id == 0u)
				std::cerr << "ResourceManager: in type = " << type << "; id = nonon_named\n"
				"\tPath = " << path << "\n";

			paths[id] = path;
		}
		else if (type == "rigidbody")
		{
			ResId id = loader.load("id", 0u);

			b2BodyDef def;

			def.allowSleep = loader.load<bool>("allowSlep", true);
			def.active = loader.load<bool>("active", true);
			def.angularDamping = loader.load<float32>("angularDamping", 10.f);
			def.angularVelocity = Degree(loader.load<float32>("angularVelocity", 0.f)).asRadian();
			def.awake = loader.load<bool>("awake", true);
			def.bullet = loader.load<bool>("bullet", false);
			def.fixedRotation = loader.load<bool>("fixedRotation", false);
			def.linearDamping= loader.load<float32>("linearDamping", 10.f);
			def.linearVelocity = {
				loader.load<float32>("linearVelocityX", 0.f),
				loader.load<float32>("linearVelocityY", 0.f)
			};
			std::string bodyType = loader.load<string>("bodyType", "dynamic");
			if (bodyType == "dynamic")
				def.type = b2_dynamicBody;
			else if (bodyType == "kinematic")
				def.type = b2_kinematicBody;
			else if (bodyType == "static")
				def.type = b2_staticBody;
			else
				cerr << "ResourceManager: wrong body type, which is = " << bodyType << endl;

			bodyDefs[id] = def;
		}
		else if (type == "fixture")
		{
			ResId id = loader.load("id", 0u);
			
			b2FixtureDef def;

			def.density = loader.load<float32>("density", 1.f);
			def.isSensor = loader.load<bool>("sensor", false);
			def.restitution = loader.load<float32>("restitution", 0.3f);
			
			fixtureDefs[id] = def;
		} else if ( type == "circleShape")
		{
			ResId id = loader.load("id", 0u);

			b2CircleShape shape;
			shape.m_p = {
				loader.load<float32>("shPosX", 0.f),
				loader.load<float32>("shPosY", 0.f),
			};
			shape.m_radius = loader.load<float32>("radius", 50.f ) * toB2Position;

			circleShapes[id] = shape;
		}
		else if (type == "polygonShape")
		{
			ResId id = loader.load("id", 0u);

			b2PolygonShape shape;
			
			vector<b2Vec2> points;

			DATA_SCRIPT_MULTILINE(file, loader)
			{
				points.push_back({ 
					loader.load<float32>("posX", 0.f)*toSfPosition, 
					loader.load<float32>("posY", 0.f)*toSfPosition
				} );
			}

			shape.Set(points.data(), points.size());

			polygonShapes[id] = shape;
		}
		else if (type == "boxShape")
		{
			ResId id = loader.load("id", 0u);

			b2PolygonShape shape;
			shape.SetAsBox(
				loader.load<float32>("width", 0.f)*toSfPosition,
				loader.load<float32>("height", 0.f)*toSfPosition,
				{
					loader.load<float32>("posX", 0.f)*toSfPosition,
					loader.load<float32>("posY", 0.f)*toSfPosition
				},
				Degree(loader.load<float32>("rot", 0.f)).asRadian()
			);
			
			polygonShapes[id] = shape;
		}
		else if (type == "chainShape")
		{
			ResId id = loader.load("id", 0u);

			/// TODO - do not use this type yet - not implemented
			assert(false);
			b2ChainShape shape;
			//shape.

			chainShapes[id] = shape;
		}
		/// load resources from another file similar to this
		else if (type == "resource")
		{
			std::string path = prepathFont + loader.loadRaw("path", "non_written_path_error");

			deserialise(path);
		}
		else
		{
			std::cerr << "ResourceManager: in type = undefined\n";
		}
	}
	
}

Texture * ResourceManager::TextureInstance::getTexture() const
{
	return &atlasInst[atlasId];
}
