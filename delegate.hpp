#include <vector>
#include <functional>

namespace CSharpWrapper
{
	class Delegate
	{
		std::vector<std::function<void()>> handlers;

		void AddListener(std::function<void()> func)
		{
			handlers.push_back(func);
		};

		void Invoke()
		{
			for (auto& func : handlers)
				func();
		}
	};
}