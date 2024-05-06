#pragma once

namespace LightmapBaker
{
namespace Utility
{
	template<class T>
	class Singleton
	{
	private:
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	protected:
		Singleton() = default;
		~Singleton() = default;

	public:
		static T& GetInstance() {
			static T s;
			return s;
		}
	};
}
}