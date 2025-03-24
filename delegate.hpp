#include <vector>
#include <functional>

namespace CSharpWrapper
{
	class Delegate
	{
	public:
		void AddListener(std::function<void()> func)
		{
			m_handlers.push_back(func);
		};

		void Invoke()
		{
			for (auto& func : m_handlers)
				func();
		}
	private:
		std::vector<std::function<void()>> m_handlers;
	};
}