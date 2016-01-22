#include "World.h"

World::World(sf::RenderWindow & window):mWindow(window),mWorldView(window.getDefaultView()),
mWorldBounds(0.f,0.f,mWorldView.getSize().x,2000.f),
mSpawnPosition(mWorldView.getSize().x/2.f,mWorldBounds.height-mWorldView.getSize().y),
mPlayerCharacter(nullptr),mScrollSpeed(-50.f)
{
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	mWorldView.move(0.f, mScrollSpeed*dt.asSeconds());
	mPlayerCharacter->setVelocity(0.f, 0.f);
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(),dt);
	}
	sf::Vector2f velocity = mPlayerCharacter->getVelocity();
	//mScrollSpeed = velocity.y;
	if (velocity.x != 0 && velocity.y != 0)
		mPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));
	mPlayerCharacter->accelerate(0.f, mScrollSpeed);
	mSceneGraph.update(dt);
	adaptPlayerPosition();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Main, "characters/vlad.png");
	mTextures.load(Textures::Enemy, "characters/cloud.png");
	mTextures.load(Textures::Ground, "maps/Desert.png");
	mTextures.load(Textures::ButtonNormal, "images/ButtonNormal.png");
	mTextures.load(Textures::ButtonPressed, "images/ButtonPressed.png");
	mTextures.load(Textures::ButtonSelected, "images/ButtonSelected.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}
	sf::Texture& texture = mTextures.get(Textures::Ground);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Character> leader(new Character(Character::Main, mTextures));
	mPlayerCharacter = leader.get();
	mPlayerCharacter->setPosition(mSpawnPosition);
	mSceneLayers[Object]->attachChild(std::move(leader));
	/*std::unique_ptr<Character> leftEscort(new Character(Character::Enemy, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerCharacter->attachChild(std::move(leftEscort));
	std::unique_ptr<Character> rightEscort(new Character(Character::Enemy, mTextures));
	rightEscort->setPosition(80.f, 50.f);
	mPlayerCharacter->attachChild(std::move(rightEscort));*/
}

void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;
	sf::Vector2f position = mPlayerCharacter->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerCharacter->setPosition(position);
}
