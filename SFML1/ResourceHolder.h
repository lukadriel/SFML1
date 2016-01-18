#pragma once
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <SFML/Graphics.hpp>

//namespace Textures
//{
//	enum ID
//	{
//		Main,
//		Enemy,
//		Ground
//	};
//}

template <typename Resource,typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename);
	template<typename Parameter>
	void load(Identifier id,const std::string& filename,const Parameter& secondParam);
	void load(Identifier id, const std::string& path,const std::string& filename);
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
private:


private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
	
};

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string & filename)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);

}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string & path, const std::string & filename)
{
	std::unique_ptr<Resource> resource = new Resource(path);
	if (!resource->Load(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + path + " " + filename);
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template<typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
	// TODO: insert return statement here
}

template<typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
	// TODO: insert return statement here
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string & filename, const Parameter & secondParam)
{
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}




